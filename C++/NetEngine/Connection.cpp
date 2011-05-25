#include "Connection.h"

int GetDiff( unsigned short one, unsigned short two )
{
  int diff = one - two;
  if ( diff > 0xff )
    diff = unsigned short( one + 0xff ) - unsigned short( two + 0xff );
  return diff;
}

bool ConnectionInfo::IsMissed( unsigned check )
{
  return ( RecvMissedPackets & ( 1 << GetDiff( lastrecv, check - 1 ) ) ) != 0;
}

void ConnectionInfo::UpdateRTT()
{
  std::list< Tick > newlist;

  for ( std::list< Tick >::iterator it = packetsent.begin(), it2 = packetsent.end(); it != it2; ++it )
  {
    if ( GetDiff( it->first, lastrecv ) > 32 ) // it's too old I care not
      continue;
    if ( Clock() - it->second > 1000000LL )
      continue;
    if ( !IsMissed( it->first ) )
    {
      LONGLONG nRTT = Clock() - it->second;
      RTT = ( nRTT + LONGLONG( RTT * 0.97 ) ) / 2;
    }
    else
      newlist.push_back( *it );
  }

  packetsent = newlist;
}

float ConnectionInfo::GetRTT()
{
  return RTT / 1000000.0f;
}

short ConnectionInfo::GenerateSequenceNumber()
{
  srand( static_cast<unsigned int>( Clock() ) );
  return rand();
}

bool ConnectionInfo::GetNextMessage( MessagePacket &Next )
{
  for ( Buckets::iterator it = PriorityBuckets.begin(), it2 = PriorityBuckets.end(); it != it2; ++it )
  {
    if ( !it->second.empty() )
    {
      Next = it->second.front();
      it->second.pop_front();
      if ( it->first - 2 < 4 )
      {
        IfMissed[seqnum].push_back( PrioritizedMessage( Next, it->first - 1 ) );
      }
      return true;
    }
  }
  return false;
}

void ConnectionInfo::FillBuckets( const std::list< PrioritizedMessage > &messages )
{
  for ( std::list< PrioritizedMessage >::const_iterator
  it = messages.begin(), it2 = messages.end();
  it != it2; ++it )
  {
    PriorityBuckets[it->second].push_back( it->first );
  }
}

void ConnectionInfo::SendForRetransmit( UDPHeader &head )
{
  int diff = GetDiff( head.Received, lastrecv );
  IfMissed.erase( diff + lastrecv );
  if ( diff < 32 && diff > 1 )
  {
    for ( unsigned short i = 1; i < diff; ++i )
    {
      std::list< PrioritizedMessage > & missedmessages = IfMissed[i + lastrecv];
      while ( !missedmessages.empty() )
      {
        PrioritizedMessage mpair( missedmessages.front() );
        MessagePacket New( mpair.first );
        int length = strlen( New.msg.str );
        New.msg.str[length] = ' ';
        New.msg.str[length + 1] = 'R';
        New.msg.str[length + 2] = 0;
        
        PriorityBuckets[mpair.second].push_back( New );
        missedmessages.pop_front();
      }
      IfMissed.erase( i + lastrecv );
      IfMissed.erase( i + lastrecv - 32 );
    }
    return;
  }
  IfMissed.clear();
}

bool ConnectionInfo::HandlePacket( UDPHeader &head )
{
  //lastseqnum = head.SequenceNumber;
  if ( lastseqnum == head.SequenceNumber )
    return false;

  int diff = GetDiff( head.SequenceNumber, lastseqnum );

  if ( diff > 0 )
  {
    SendForRetransmit( head );
    //int rin = rand() % 1000 + 1;
    //bool ri = rin < 100 ? true : false;
    //if ( ri )
    MissedPackets <<= 1;
    if ( diff > 1 )
    {
      //Some messages got lost
      int step1 = ~MissedPackets;
      int step2 = step1 << ( diff - 1 );
      int step3 = ~ step2;
      MissedPackets = step3;
    }
    lastseqnum = head.SequenceNumber;
    lastrecv = head.Received;
    RecvMissedPackets = head.MissedPackets;
  }
  else
  {
    unsigned char bitmask = 1 << -diff;
    if ( MissedPackets & bitmask )
    {
      MissedPackets &= ~bitmask;
    }
    else
      return false;
  }
  return true;
}
