#include "NetObject.h"
#include <cstdlib>

NetObject::NetObject() : port( 0 ), running( true ), ClearRecv( false ), msguse( false ), recvuse( false )
{
  BandWidth = 10240;
}

NetObject::~NetObject()
{
}

unsigned NetObject::GetMaxPackSend()
{
  return ( BandWidth - currentdatasent ) / MaxPacketSize;
}


void NetObject::initialize( unsigned Port, const std::string &Key )
{
  port = Port;
  key = Key;
  sin = sockbase.Connect( port );
  sock = sockbase.CreateSocket();
  sockbase.SetNonBlocking( true );
  sockbase.Bind();

  thread = (HANDLE)_beginthreadex(  NULL, 0, NetThread, ( void * )this, 0, &threadid );
  if( !thread )
  {
    printf( "_beginthreadex error: %d\n", GetLastError() );
  }
}

void NetObject::HandlePackets()
{
  for ( std::list<MessagePacket>::iterator it = handlemsgs.begin(), it2 = handlemsgs.end(); it != it2; ++it )
  {
    HandleMessage( *it );
  }
  handlemsgs.clear();
}

void NetObject::Close()
{
  running = false;
  WaitForMultipleObjects( 1, &thread, TRUE, INFINITE );
  CloseHandle( thread );
}

void NetObject::Send( const char *buff, unsigned size, unsigned char priority )
{
  if ( priority < 1 )
    priority = 1;
  MessagePacket msg;
  msg.msgId = MessagePacket::MESSAGE;
  msg.from = 0;
  memcpy( msg.msg.str, buff, size );
  msguse.Synchronize();
  messages.push_back( PrioritizedMessage( msg, priority + 1 ) );
  msguse.Release();
}

void NetObject::Recv( char *buff, unsigned size )
{
  recvuse.Synchronize();
  if ( !received.empty() )
  {
    MessagePacket msg = received.front();
    received.pop_front();
    memcpy( buff, msg.msg.str, size );
  }
  recvuse.Release();
}

unsigned __stdcall NetObject::NetThread( void *param )
{
  NetObject &pthis = *( NetObject * )param;
  pthis.Logic();
  return TRUE;
}


