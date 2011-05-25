#include "socket_base.h"
#include "Clock.h"
#include <list>
#include <map>
typedef std::pair< MessagePacket, unsigned char > PrioritizedMessage;

struct ConnectionInfo
{
  ConnectionInfo() : TimeofLastUpdate(0), state(0), id( -1 ),
    MissedPackets( 0 ) 
  {
    RTT = 30LL;
    seqnum = GenerateSequenceNumber();
  }

  unsigned short seqnum;
  unsigned TimeofLastUpdate;
  unsigned state;
  unsigned id;
  unsigned short lastseqnum;
  unsigned short lastrecv;
  unsigned MissedPackets;
  unsigned RecvMissedPackets;
  typedef std::pair< unsigned short,LONGLONG > Tick;
  std::list< Tick > packetsent;

  bool IsMissed( unsigned check );
  void UpdateRTT();
  float GetRTT();
  bool GetNextMessage( MessagePacket &Next );
  void FillBuckets( const std::list< PrioritizedMessage > &messages );
  short GenerateSequenceNumber();
  void SendForRetransmit( UDPHeader &head );
  bool HandlePacket( UDPHeader &head );

  LONGLONG RTT;
  typedef std::map< unsigned short, std::list< PrioritizedMessage > > Retransmit;
  Retransmit IfMissed;
  typedef std::map< unsigned, std::list< MessagePacket > > Buckets;
  Buckets PriorityBuckets;
};

int GetDiff( unsigned short one, unsigned short two );
