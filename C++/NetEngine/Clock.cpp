#include "Clock.h"

LONGLONG Clock()
{
  LARGE_INTEGER Frequency;
  LARGE_INTEGER Counter;
  if ( QueryPerformanceFrequency( &Frequency ) )
  {
    QueryPerformanceCounter( &Counter );
    return Counter.QuadPart * 1000000LL / Frequency.QuadPart;
  }
  return 0LL;
}