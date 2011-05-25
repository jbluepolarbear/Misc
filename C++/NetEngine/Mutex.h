#pragma once

#ifndef HANDLE
  typedef void *HANDLE;
#endif

class Mutex
{
  public:
    Mutex( bool Owned = false );

    ~Mutex();

    bool Synchronize();

    bool Release();

  private:
    HANDLE mutex;
    bool owned;
};
