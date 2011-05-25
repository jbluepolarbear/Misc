#include <windows.h>
#include <cstdio>
#include "Mutex.h"

Mutex::Mutex( bool Owned ) : owned( Owned )
{
  mutex = CreateMutex( NULL, owned, NULL );
  if ( !mutex )
  {
    printf( "CreateMutex error: %d\n", GetLastError() );
  }
}

Mutex::~Mutex()
{
  if( !CloseHandle( mutex ) )
    printf( "CloseHandle error: %d\n", GetLastError() );
}

bool Mutex::Synchronize()
{
  DWORD waitresult = WaitForSingleObject( mutex, INFINITE );

  switch ( waitresult )
  {
    case WAIT_OBJECT_0:
      return true;
    case WAIT_TIMEOUT:
    case WAIT_ABANDONED:
      return false;
    case WAIT_FAILED:
      printf( "WaitForSingleObject error: %d\n", GetLastError() );
      return false;
  }
  return false;
}

bool Mutex::Release()
{
  if ( !ReleaseMutex( mutex ) )
  {
    printf( "ReleaseMutex error: %d\n", GetLastError() );
    return false;
  }
  return true;
}