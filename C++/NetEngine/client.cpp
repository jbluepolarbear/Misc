#include <iostream>
#include "socket_debug.h"
#include "client.h"
#include <ctime>
#include <vector>

Client::Client() : ServerFound( false ), connected( false ), serid( 0 )
{
  lastrecv = seqnum;
}

bool Client::FoundServer()
{
  return ServerFound;
}

bool Client::Connected()
{
  return connected;
}

unsigned Client::GetId()
{
  return id;
}

void Client::FindServer( SOCKET bsock, char *message, int portsize, int keysize )
{
  int slen = sizeof( sockaddr_in );
  int n = recvfrom( bsock, message, portsize + keysize, 0, ( sockaddr * )&serversin, &slen );
  if ( n > 0 )
  {
    if ( ( char * )message == key )
    {
      MessagePacket msg;
      msg.msgId = MessagePacket::CONNECT_REQUEST;
      msg.msg.sin = sin;

      msguse.Synchronize();
      messages.push_back( PrioritizedMessage( msg, 1 ) );
      msguse.Release();
      unsigned sport = *( unsigned * )( message + keysize );
      serversin.sin_port = htons( sport );
    }
  }
}

static unsigned GetMaxPackSend()
{
  return 10;
}

void Client::SendPackets()
{
  msguse.Synchronize();
  FillBuckets( messages );
  messages.clear();
  msguse.Release();

  bool notdone = true;
  unsigned num = GetMaxPackSend();

  while ( notdone && --num )
  {
    unsigned char Buffer[MaxPacketSize];
    unsigned offset = sizeof( UDPHeader );
    while ( notdone = GetNextMessage( *reinterpret_cast<MessagePacket *>( Buffer + offset ) ) &&
            ( offset += sizeof( MessagePacket ) ) < MaxPacketSize );

    unsigned Size = offset - sizeof( UDPHeader );
    UDPHeader Header = { CW, GetId(), seqnum, lastseqnum, Size, MissedPackets };

    packetsent.push_back( Tick( seqnum++, Clock() ) );

    memcpy( Buffer, &Header, sizeof Header );

    currentdatasent += offset;
    int n = sendto( sock, ( char * )Buffer, offset, 0, ( sockaddr * )&serversin, sizeof( sockaddr_in ) );
    Check( n );
  }
}

void Client::ReceivePackets()
{
  recvuse.Synchronize();
  int n;
  do
  {
    //MessagePacket rmsg;
    //n = recv( sock, ( char * )&rmsg, sizeof rmsg, 0 );
    //Check(n);
    //if ( n > 0 )
    //  HandleMessage( rmsg );
    UDPHeader head;
    char rstr[MaxPacketSize] = {0};
    n = recv( sock, rstr, MaxPacketSize, 0 );
    if ( Check(n) > 0 )
    {
      memcpy( &head, rstr, sizeof head );
      if ( serid == 0 )
      {
        serid = head.UniqueID;
        lastseqnum = head.SequenceNumber - 1;
      }

      if ( HandlePacket( head ) )
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
  } while ( n > 0 );
  recvuse.Release();
}

void Client::Logic()
{
  clock_t time = clock();
  clock_t actime = time;

  SOCKET bsock;

  SocketBase bcomm;
  sockaddr_in &bsin = bcomm.Connect( PORT_BROADCAST );
  bsock = bcomm.CreateSocket();
  bcomm.Bind();
  bcomm.SetBroadcast( true );

  int keysize = int( key.size() + 1 );
  int portsize = sizeof port;

  char *message = new char[ keysize + portsize ];

  //memcpy( message, this.key.c_str(), keysize );
  //memcpy( message + keysize, &this.port, portsize );

  while ( running )
  {
    LONGLONG flowend = Clock() - flowstart;
    if ( flowend >  1000000LL )
    {
      flowstart = Clock();
      currentdatasent = 0;
      float l = GetRTT();
      if ( l < 0.0001f )
        BandWidth += 512;
      else if ( l > 0.001f )
        BandWidth -= 512;
    }

    if ( !ServerFound )
    {
      FindServer( bsock, message, portsize, keysize );
    }

    SendPackets();

    ReceivePackets();

    HandlePackets();

    UpdateRTT();

    Sleep( 10 );
  }
}

void Client::HandleMessage( MessagePacket &msg )
{
  switch ( msg.msgId )
  {
    case MessagePacket::PING:
    {
      msg.msg.sin = sin;
      msguse.Synchronize();
      messages.push_back( PrioritizedMessage( msg, 1 ) );
      msguse.Release();
      //int n = sendto( sock, ( char * )&msg, sizeof msg, 0, ( sockaddr * )&serversin, sizeof serversin );
      //if ( !( n > 0 ) )
      //{
      //  Check( n );
      //}
    }
      break;
    case MessagePacket::MESSAGE:
      recvuse.Synchronize();
      received.push_back( msg );
      recvuse.Release();
      break;
    case MessagePacket::CONNECT_CONFIRM:
    {
      ServerFound = true;
      connected = true;
      id = *( int * )msg.msg.str;
    }
      break;
    case MessagePacket::DISCONNECT:
      connected = false;
      break;
    default:
      break;
  }
}

void Client::ClearReceive()
{
  recvuse.Synchronize();
  if ( !received.empty() )
    ClearRecv = true;
  recvuse.Release();
}
