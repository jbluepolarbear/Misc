#pragma once
#include <winsock2.h>
#include "message.h"

#pragma comment( lib, "ws2_32.lib" )


#define PORT_BROADCAST 8001
#define PORT_SERVER 8002
#define PORT_CLIENT 8003

#pragma pack(push,1)
struct MessagePacket
{
	enum Type
	{
	  CONNECT_REQUEST,
	  CONNECT_CONFIRM,
	  MESSAGE,
	  PING,
	  DISCONNECT
	};
  unsigned char msgId;
  unsigned short from;
  union
  {
    unsigned ui;
    sockaddr_in sin;
    char str[sizeof(Message)];
  } msg;
};
#pragma pack(pop)

struct UDPHeader
{
  unsigned short ProtocolID; // = "CW"
  unsigned short UniqueID;
  unsigned short SequenceNumber;
  unsigned short Received; // Highest Sequence number received
  unsigned char DataSize; // max of 256 - Header size
  unsigned MissedPackets; // Starting at Received - 1
};

const unsigned short CW = *(unsigned short *)"CW";
const int MaxPacketSize = sizeof( MessagePacket ) * 8 + sizeof( UDPHeader );

class SocketBase
{
  public:
            SocketBase();
    virtual ~SocketBase();

    sockaddr_in & Connect( unsigned port );
    void          Disconnect();
    void          Bind();
    void          GetHost();
    SOCKET        CreateSocket();
    void          SetBroadcast( bool flag );
    void          SetNonBlocking( bool flag );
    char *        GetLocalAddress();
    sockaddr_in & GetSockaddr();
    unsigned      FindOpenPort();

  private:
    WSAData      data;
    SOCKET       sock;
    sockaddr_in  localsin;
    char *ip;
};
