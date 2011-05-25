#include <cstdio>
#include "socket_debug.h"
#include "socket_base.h"

SocketBase::SocketBase() : sock( INVALID_SOCKET ), ip( NULL )
{
}

SocketBase::~SocketBase()
{
  Disconnect();
}

sockaddr_in &SocketBase::Connect( unsigned port )
{
  WSAStartup( MAKEWORD( 2, 2 ), &data );

  sock = CreateSocket();

  localsin.sin_family = AF_INET;
  localsin.sin_port   = htons( port );
  localsin.sin_addr.s_addr = inet_addr( GetLocalAddress() );//INADDR_ANY;

  return localsin;
}

void SocketBase::Bind()
{
  int error = bind( sock, ( sockaddr * )&localsin, sizeof localsin );
  error = Check( error );
}

void SocketBase::GetHost()
{
  hostent *host = gethostbyname( "" );
  ip = inet_ntoa ( *( struct in_addr * )*host->h_addr_list );
}

SOCKET SocketBase::CreateSocket()
{
  if ( sock == INVALID_SOCKET )
    sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

  return sock;
}

void SocketBase::SetBroadcast( bool flag )
{
  int error = setsockopt( sock, SOL_SOCKET, SO_BROADCAST, ( char * )&flag, sizeof flag );
  if ( error == SOCKET_ERROR )
    printf( "failed to set sock option\n" );
  localsin.sin_port   = htons( PORT_BROADCAST );
  localsin.sin_addr.s_addr = INADDR_BROADCAST;
}

void SocketBase::SetNonBlocking( bool flag )
{
  ioctlsocket( sock, FIONBIO, ( unsigned long * )&flag );
}

char *SocketBase::GetLocalAddress()
{
  if ( ip )
    return ip;

  GetHost();
  return ip;
}

sockaddr_in &SocketBase::GetSockaddr()
{
  return localsin;
}

unsigned SocketBase::FindOpenPort()
{
  return 0;
}

// @note is it a good idea to call WSACleanup here?
void SocketBase::Disconnect()
{
  closesocket( sock );
  WSACleanup();
}
