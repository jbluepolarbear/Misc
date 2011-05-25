#pragma once
#include <string>
#include <winsock2.h>
#include <process.h>
#include "socket_base.h"
#include "Mutex.h"
#include "NetObject.h"
#include <map>

class Client : public NetObject, public ConnectionInfo
{
  public:
    Client();
    bool FoundServer();
    virtual void Logic();
    void HandleMessage( MessagePacket &msg );
    virtual void ClearReceive();
    unsigned GetId();
    void FindServer( SOCKET bsock, char *message, int portsize, int keysize );
    virtual bool Connected();
    virtual void SendPackets();
    virtual void ReceivePackets();

  private:
    bool ServerFound;
    bool connected;
    unsigned short serid;
    sockaddr_in clientsin;
    sockaddr_in serversin;
};

