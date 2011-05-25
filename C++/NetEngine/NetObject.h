#pragma once
#include <string>
#include <winsock2.h>
#include <process.h>
#include "Mutex.h"
#include <ctime>
#include "Connection.h"

class NetObject
{
  public:
    NetObject();
    virtual ~NetObject();

    void initialize( unsigned Port, const std::string &Key );
    void Close();
    void Send( const char *buff, unsigned size, unsigned char priority = 5 );
    void Recv( char *buff, unsigned size );
    virtual void Logic() = 0;
    virtual void ClearReceive() = 0;
    static unsigned __stdcall NetThread( void *param );
    virtual void HandleMessage( MessagePacket &msg ) = 0;
    virtual bool Connected() = 0;
    virtual void SendPackets() = 0;
    virtual void ReceivePackets() = 0;
    //void HandlePacket( ConnectionInfo &info, UDPHeader &head, char *hstr );
    void HandlePackets();
    unsigned GetMaxPackSend();

  protected:
    SOCKET sock;
    SocketBase sockbase;
    sockaddr_in sin;
    unsigned port;
    std::list< PrioritizedMessage > messages;
    std::list< MessagePacket > received;
    std::list< MessagePacket > handlemsgs;
    std::string key;
    bool running;
    Mutex msguse;
    Mutex recvuse;
    //LONGLONG RTT;
    LONGLONG flowstart;
    unsigned currentdatasent;
    unsigned BandWidth;
    bool ClearRecv;
    HANDLE thread;
    unsigned threadid;
};
