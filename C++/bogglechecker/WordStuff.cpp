#include "WordStuff.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

void LoadFile( const std::string &filename, std::string &destination )
{
  std::fstream filestr;
  filestr.open( filename.c_str(), std::fstream::in );
  if ( filestr.is_open() )
  {
    filestr.seekg( 0, std::fstream::end );
    int count = filestr.tellg();
    filestr.seekg( 0, std::fstream::beg );
    char *str = new char[count];
    if ( str )
    {
      filestr.read( str, count );
      count = filestr.gcount();
      destination = std::string( str, count );
      destination += '\0';
      delete str;
    }
    filestr.close();
  }
}

void SplitWords( const std::string &words, std::vector<std::string> &subwords )
{
  std::string str;
  int size = words.size();

  for ( int i = 0; i < size; ++i )
  {
    if ( words[i] == '\n' )
    {
      subwords.push_back( str );
      str.clear();
    }
    else
      str += words[i];
  }
}

void Output( const std::string &filename, const std::vector<std::string> &subwords )
{
  std::fstream filestr;
  filestr.open( filename.c_str(), std::fstream::out );
  if ( filestr.is_open() )
  {
    for ( int i = 0; i < subwords.size(); ++i )
    {
      std::string str;
      str = subwords[i] + "\n";
      filestr << str.c_str();
    }

    filestr.close();
  }
}
