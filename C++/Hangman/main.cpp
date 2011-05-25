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

char FindMostCommonLetter( const std::vector<std::string> &sublist, const std::string &exclude )
{
  unsigned letters[26] = {0};

  for ( std::vector<std::string>::const_iterator i = sublist.begin(); i != sublist.end(); ++i )
  {
    for ( std::string::const_iterator j = i->begin(); j != i->end(); ++j )
    {
      if (  exclude.find( char( *j ) ) == std::string::npos )
        ++letters[tolower(*j)-'a'];
    }
  }

  //unsigned max = 0;
  unsigned index = 0;
  for ( unsigned i = 0; i < 26; ++i )
  {
    if ( letters[i] > letters[index] )
      index = i;
  }

  return 'a' + char( index );
}

std::vector<unsigned> GuessLetter( char c, const std::string &str )
{
  std::vector<unsigned> pos;
  int found = 0;
  int offset = 0;
  std::string cstr = str;

  for( int i = 0; cstr[i] != '\0'; ++i )
  {
    cstr[i] = tolower( cstr[i] );
  }

  while ( ( found = cstr.find_first_of( c ) ) != std::string::npos )
  {
    pos.push_back( found + offset );
    cstr = cstr.substr( found + 1 );
    offset += found + 1;
  }

  return pos;
}

void RemoveWordsWith( const char c, std::vector<std::string> &sublist )
{
  std::vector<std::string> temp;
  for ( std::vector<std::string>::const_iterator i = sublist.begin(); i != sublist.end(); ++i )
  {
    if ( GuessLetter( c, *i ).empty() )
      temp.push_back( *i );
  }
  sublist = temp;
}


void RemoveWordsWithout( const char c, std::vector<std::string> &sublist, std::vector<unsigned> &pos )
{
  std::vector<std::string> temp;
  for ( std::vector<std::string>::const_iterator i = sublist.begin(); i != sublist.end(); ++i )
  {
    if ( GuessLetter( c, *i ) == pos )
      temp.push_back( *i );
  }
  sublist = temp;
}

std::vector<unsigned> ConvertInput( const std::string &input )
{
  std::vector<unsigned> pos;
  //false = empty
  //true = 1 2 4

  if ( input == "0" )
    return pos;

  std::stringstream ss( input );

  long temp;
  while ( ss >> temp )
  {
    pos.push_back( temp - 1 );
  }

  return pos;
}

int main( )
{
  std::string words;
  std::vector<std::string> subwords;
  LoadFile( "web2.txt", words );
  SplitWords( words, subwords );

  int size;
  std::cout << "Input length: ";
  std::cin >> size;
  std::cin.ignore();

  std::vector<std::string> sublist;

  for ( int i = 0; i < subwords.size(); ++i )
  {
    if ( size == subwords[i].size() )
      sublist.push_back( subwords[i] );
  }

  char gl;
  bool lf;
  std::string exclude;
  std::string input;

  for ( int i = 0; i < 8; )
  {
    gl = FindMostCommonLetter( sublist, exclude );

    std::cout << "Guessing " << gl << std::endl;
    std::getline( std::cin, input );
    std::vector<unsigned> pos = ConvertInput( input );

    if ( pos.empty() )
    {
      RemoveWordsWith( gl, sublist );
      ++i;
    }
    else
    {
      RemoveWordsWithout( gl, sublist, pos );
      exclude += gl;
    }

    if ( sublist.size() == 1 )
    {
      std::cout << "It won. The Word is " << sublist[0] << std::endl;
      break;
    }
  }

  if ( sublist.size() != 1 )
  {
    std::cout << "It lost." << std::endl;
  }

  Output( "output.txt", sublist );

  return 0;
}