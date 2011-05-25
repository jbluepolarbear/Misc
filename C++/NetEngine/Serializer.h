#pragma once

#include "NetObject.h"
#include <string>

class Serializer
{
  public:
    Serializer( NetObject &Object ) : object( Object ) {}
    template <typename T>
    void Write( T &value );
    template <typename T>
    void Read( T &value );
    void Flush();
  private:
    NetObject &object;
    std::string buff;
};

#include "Serializer.cpp"