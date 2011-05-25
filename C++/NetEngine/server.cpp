#include <vector>
#include <list>
#include <iostream>
#include "socket_debug.h"
#include "server.h"
#include <ctime>


Server::Server() : MaxNumberOfClients( ( unsigned )(-1) ), PingTimeWait( 1 ), IntervalBetweenPings( 500 ), CurrentNumberofClients( 0 ), id(-1)
{}

Server::~Server()
{
}

void Server::UpdateRTT()
{
  for ( ClientIterator it = clients.begin(), it2 = clients.end(); it != it2; ++it )
    it->UpdateRTT();
}

float Server::GetRTT( int client )
{
  ClientIterator ci = FindClient( client );
  if ( ci != clients.end() )
    return ci->GetRTT();
  return -1.0f;
}

bool Server::Connected()
{
  if ( CurrentNumberofClients > 0 )
    return true;
  return false;
}

void Server::ClearReceive()
{
  recvuse.Synchronize();
  if ( !received.empty() )
    ClearRecv = true;
  recvuse.Release();
}

void Server::BroadCast( SOCKET bsock, char *message, int portsize, int keysize, sockaddr_in bsin )
{
  if ( clients.size() < MaxNumberOfClients )
  {
    int n = sendto( bsock, message, keysize + portsize, 0, ( sockaddr * )&bsin, sizeof bsin );
    if ( n == SOCKET_ERROR )
      printf( "failed to send\n" );
  }
}

static unsigned GetMaxPackSend()
{
  return 10;
}

//void Server::SerdToClient( sockaddr_in &clientsin, MessagePacket msg )
//{
//  ClientIterator ci = FindClient( clientsin );
//  if ( ci != clients.end() )
//  {
//    int size = sizeof msg;
//    UDPHeader Header = { CW, GetId(), seqnum, 0, size, 0 };
//    char buff[sizeof( MessagePacket ) + sizeof( UDPHeader )] = {0};
//    memcpy( buff, &Header, sizeof Header );
//    memcpy( buff + sizeof Header, &msg, size );
//
//    Header.Received = ci->lastseqnum;
//    Header.MissedPackets = ci->MissedPackets;
//    ci->packetsent.push_back( ClientInfo::Tick( seqnum++, Clock() ) );
//
//    int n = sendto( sock, ( char * )buff, size + sizeof Header, 0, ( sockaddr * )&ci->sin, sizeof( sockaddr_in ) );
//    Check( n );
//  }
//}

void Server::SendPackets()
{
  msguse.Synchronize();
  for ( ClientIterator itc = clients.begin(); itc != clients.end(); ++itc )
    itc->FillBuckets( messages );
  messages.clear();
  msguse.Release();

  if ( clients.size() > 0 )
  {
    for ( ClientIterator itc = clients.begin(); itc != clients.end(); ++itc )
    {
      bool notdone = true;
      unsigned num = GetMaxPackSend();

      while ( notdone && --num )
      {
        unsigned char Buffer[MaxPacketSize];
        unsigned offset = sizeof( UDPHeader );
        while ( notdone = itc->GetNextMessage( *reinterpret_cast<MessagePacket *>( Buffer + offset ) ) &&
          ( offset += sizeof( MessagePacket ) ) < MaxPacketSize );

        unsigned Size = offset - sizeof( UDPHeader );
        UDPHeader Header = { CW, GetId(), itc->seqnum, itc->lastseqnum, Size, itc->MissedPackets };

        itc->packetsent.push_back( ClientInfo::Tick( itc->seqnum++, Clock() ) );

        memcpy( Buffer, &Header, sizeof Header );

        int n = sendto( sock, ( char * )Buffer, offset, 0, ( sockaddr * )&itc->sin, sizeof( sockaddr_in ) );
        Check( n );

      }
    }
  }
}

unsigned short Server::GetId()
{
  return id;
}

void Server::ReceivePackets()
{
  recvuse.Synchronize();
  int n;
  do 
  {
    UDPHeader head;
    char rstr[MaxPacketSize] = {0};
    n = recv( sock, rstr, MaxPacketSize, 0 );
    if ( Check(n) > 0 )
    {
      MessagePacket rmsg;

      memcpy( &head, rstr, sizeof head );
      if ( head.UniqueID == static_cast<unsigned short>(-1) )
      {
        memcpy( &rmsg, rstr + sizeof head, sizeof rmsg );
        handlemsgs.push_back( rmsg );

        ClientInfo ci;
        ci.sin = rmsg.msg.sin;
        ClientIterator client = FindClient( ci.sin );

        if ( client == clients.end() )
        {
          if ( clients.size() < MaxNumberOfClients )
          {
            ++CurrentNumberofClients;
            ci.TimeofLastUpdate = clock();
            ci.id = CurrentNumberofClients;
            id = ci.id;
            ci.lastseqnum = head.SequenceNumber - 1;
            clients.push_back( ci );
          }
        }
        continue;
      }

      ClientIterator it = FindClient( head.UniqueID );
      if ( it != clients.end() )
      {
        if ( it->HandlePacket( head ) )
        {
          int psize = head.DataSize / sizeof( MessagePacket );
          for ( int i = 0; i < psize; ++i )
          {
            MessagePacket rmsg;
            memcpy( &rmsg, rstr + sizeof head + i * sizeof rmsg, sizeof rmsg );
            handlemsgs.push_back( rmsg );
          }
        }
      }
    }

  } while ( n > 0 );
  
  recvuse.Release();
}

void Server::SetMaxClients( unsigned MaxClients )
{
  MaxNumberOfClients = MaxClients;
}

float Server::GetLargestRTT()
{
  float t = 0.0f;
  for ( ClientIterator it = clients.begin(), it2 = clients.end(); it != it2; ++it )
  {
    if ( it->GetRTT() > t )
      t = it->GetRTT();
  }
  return t;
}

void Server::Logic()
{
  clock_t time = clock();
  clock_t actime = time;

  SOCKET bsock;

  SocketBase bcomm;
  sockaddr_in &bsin = bcomm.Connect( PORT_BROADCAST );
  bsock = bcomm.CreateSocket();
  bcomm.SetBroadcast( true );

  int keysize = int( key.size() + 1 );
  int portsize = sizeof port;

  char *message = new char[ keysize + portsize ];

  memcpy( message, key.c_str(), keysize );
  memcpy( message + keysize, &port, portsize );

  while ( running )
  {
    LONGLONG flowend = Clock() - flowstart;
    if ( flowend >  1000000LL )
    {
      flowstart = Clock();
      currentdatasent = 0;
      float l = GetLargestRTT();
      if ( l < 0.0001f )
        BandWidth += 512;
      else if ( l > 0.001f )
        BandWidth -= 512;
    }

    BroadCast( bsock, message, portsize, keysize, bsin );

    SendPackets();

    ReceivePackets();

    time = clock();
    actime += time - actime;

    HandlePackets();

    if ( actime > clock_t( IntervalBetweenPings ) )
    {
      MessagePacket msg;
      msg.from = 0;
      msg.msgId = MessagePacket::PING;
      msguse.Synchronize();
      messages.push_back( PrioritizedMessage( msg, 1 ) );
      actime = time;
      msguse.Release();
    }

    // come back here later jeremy
    for ( ClientIterator it = clients.begin(); it < clients.end(); ++it )
    {
      if ( ( it->TimeofLastUpdate )  > time + PingTimeWait * CLOCKS_PER_SEC )
      {
        RemoveClient( it );
        --CurrentNumberofClients;
        break;
      }
    }
    UpdateRTT();

    Sleep( 10 );
  }
}

void Server::RemoveClient( std::vector< ClientInfo >::iterator client )
{
  clients.erase( client );

  MessagePacket msg;
  msg.from = 0;
  msg.msgId = MessagePacket::DISCONNECT;

  msguse.Synchronize();
  messages.push_back( PrioritizedMessage( msg, 1 ) );
  msguse.Release();
}

void Server::HandleMessage( MessagePacket &msg )
{
  switch ( msg.msgId )
  {
    case MessagePacket::PING:
    {
      ClientIterator client = FindClient( msg.msg.sin );
      if( client != clients.end() )
        client->TimeofLastUpdate = clock();
      //clients[msg.from].TimeofLastUpdate = clock();
    }
      break;
    case MessagePacket::MESSAGE:
      recvuse.Synchronize();
      received.push_back( msg );
      recvuse.Release();
      break;
    case MessagePacket::CONNECT_REQUEST:
    {
      ClientInfo ci;
      ci.sin = msg.msg.sin;

      MessagePacket tomsg;

      tomsg.from = 0;
      tomsg.msgId = MessagePacket::CONNECT_CONFIRM;
      *( int * )tomsg.msg.str = FindClient( ci.sin )->id;

      msguse.Synchronize();
      messages.push_back( PrioritizedMessage( tomsg, 1 ) );
      msguse.Release();

      /*int n = sendto( sock, ( char * )&tomsg, sizeof tomsg, 0, ( sockaddr * )&ci.sin, sizeof ci.sin );
      if ( !( n > 0 ) )
        printf( "Help confirm\n" );*/
    }
      break;
    default:
      break;
  }
}

Server::ClientIterator Server::FindClient( sockaddr_in &clientsin )
{
  for ( ClientIterator it = clients.begin(); it < clients.end(); ++it )
  {
    if ( clientsin.sin_addr.s_addr == it->sin.sin_addr.s_addr )
      return it;
  }

  return clients.end();
}

Server::ClientIterator Server::FindClient( int value )
{
  for ( ClientIterator it = clients.begin(); it < clients.end(); ++it )
  {
    if ( value == it->id )
      return it;
  }

  return clients.end();
}