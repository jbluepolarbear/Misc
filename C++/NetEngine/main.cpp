// lol
#if 1
#include "server.h"
#include <windows.h>
#include <sstream>
#include <iostream>

int main()
{
  printf( "I'm the Server.\n" );
  WSAData data;
  WSAStartup( MAKEWORD( 2, 2 ), &data );

  Server server;
  //server.SetMaxClients( 1 );
  server.initialize( PORT_SERVER, "CW" );

  while ( !server.Connected() );

  int i = 0;
  while ( !GetAsyncKeyState( VK_ESCAPE ) )
  {
    int n = 0;
    do 
    {
      char *msg = new char[256];
      memset( msg, 0, 256 );
      server.Recv( msg, 256 );

      if ( msg[0] != 0 )
      {
        puts( msg );
        std::cout << server.GetRTT( 1 ) << "\n";
      }
    } while ( n > 0 );

    std::string str( "Hello from the server" );
    std::stringstream ss;
    ss << str << " " << i++ << "\n";
    server.Send( ss.str().c_str(), ss.str().length() + 1,1 + i % 4 );
    Sleep(10);
  }

  WSACleanup();
}

#else

#include "client.h"
#include <windows.h>
#include <sstream>
#include <iostream>

int main()
{
  printf( "I'm the Client.\n" );
  WSAData data;
  WSAStartup( MAKEWORD( 2, 2 ), &data );

  Client client;
  client.initialize( PORT_CLIENT, "CW" );

  while ( !client.FoundServer() );

  int i = 0;
  while ( !GetAsyncKeyState( VK_ESCAPE ) )
  {
    int n = 0;
    do 
    {
      char *msg = new char[256];
      memset( msg, 0, 256 );
      client.Recv( msg, 256 );

      if ( msg[0] != 0 )
      {
        puts( msg );
        std::cout << client.GetRTT() << '\n';
      }
      Sleep(10);
    } while ( n > 0 );

    std::string str( "Hello from the client" );
    std::stringstream ss;
    ss << str << " " << i++ << '\n';
    client.Send( ss.str().c_str(), ss.str().length() + 1, rand() % 5 + 1 );
    Sleep( 10 );
  }

  client.Close();

  WSACleanup();
}

#endif
