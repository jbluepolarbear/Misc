#pragma once

#include <vector>
#include <string>
#include <list>
#include <winsock2.h>
#include <process.h>
#include "socket_base.h"
#include "Mutex.h"

#include "NetObject.h"

struct ClientInfo : public ConnectionInfo
{
  sockaddr_in sin;
};


class Server : public NetObject
{
  public:
    Server();
    virtual ~Server();
    virtual void ClearReceive();
    virtual void Logic();
    void RemoveClient( std::vector< ClientInfo >::iterator client );
    void HandleMessage( MessagePacket &msg );
    void SetMaxClients( unsigned MaxClients = (unsigned)(-1) );
    void BroadCast( SOCKET bsock, char *message, int portsize, int keysize, sockaddr_in bsin );
    virtual bool Connected();
    virtual void SendPackets();
    virtual void ReceivePackets();
    unsigned short GetId();
    void UpdateRTT();
    float GetRTT( int client );
    void SerdToClient( sockaddr_in &clientsin, MessagePacket msg );
    float GetLargestRTT();

  private:
    typedef std::vector< ClientInfo >::iterator ClientIterator;
    ClientIterator FindClient( sockaddr_in &clientsin );
    ClientIterator FindClient( int value );
    unsigned MaxNumberOfClients;
    std::vector< ClientInfo > clients;
    unsigned PingTimeWait;//seconds
    unsigned IntervalBetweenPings;//milliseconds
    unsigned CurrentNumberofClients;
    unsigned short id;
};
