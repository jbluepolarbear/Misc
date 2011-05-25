#include "Serializer.h"
#include <iostream>

template <typename T>
void Serializer::Write( T &value )
{
  buff.append( reinterpret_cast< char * >( &value ), sizeof( T ) );
}

template <typename T>
void Serializer::Read( T &value )
{
  std::string obuff = buff.substr( 0, sizeof( T ) );
  buff.erase( 0, sizeof( T ) );
  value = *( T * )( obuff.data() );
}

void Serializer::Flush()
{

}