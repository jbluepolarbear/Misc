#include "WordStuff.h"
#include <cstdlib>
#include "TimeClass.h"
#include <cctype>

std::string cubes[] =
{
  "A","E","A","N","E","G",
  "A","H","S","P","C","O",
  "A","S","P","F","F","K",
  "O","B","J","O","A","B",
  "I","O","T","M","U","C",
  "R","Y","V","D","E","L",
  "L","R","E","I","X","D",
  "E","I","U","N","E","S",
  "W","N","G","E","E","H",
  "L","N","H","N","R","Z",
  "T","S","T","I","Y","D",
  "O","W","T","O","A","T",
  "E","R","T","T","Y","L",
  "T","O","E","S","S","I",
  "T","E","R","W","H","V",
  "N","U","I","H","M","QU"
};

std::string &IndexCubes( std::string *cube, int index, int face )
{
  return cube[index*6+face];
}

std::string grid[16];

void BuildGrid( std::string *cubes, std::string *newgrid )
{
  int choosen[16] = {0};

  for ( int i = 0; i < 16; ++i )
  {
    int index = rand()%16;
    while ( choosen[index] != 0 )
    {
      index = rand()%16;
    }
    choosen[index] = 1;
    newgrid[index] = IndexCubes( cubes, index, rand()%6 );
  }
}

std::string &IndexCube( std::string *pgrid, int x, int y )
{
  return pgrid[y*4+x];
}

void PrintGrid( std::string *pgrid )
{
  for ( int i = 0; i < 16; ++i )
  {
    if ( i > 0 && i % 4 == 0 )
      std::cout << std::endl;
    std::cout << pgrid[i] << ' ';
  }
  std::cout << std::endl;
}

struct point
{
  point ( int x = 0, int y = 0 )
  {
    this->x = x;
    this->y = y;
  }

  bool operator==( const point &rhs )
  {
    if ( x == rhs.x && y == rhs.y )
      return true;
    return false;
  }

  int x, y;
};

bool ValidIndex( int x, int y )
{
  if ( x < 0 || x > 3 )
    return false;
  if ( y < 0 || y > 3 )
    return false;
  return true;
}

#include <algorithm>
bool IsValid( int x, int y, std::vector<point> &pc )
{
  std::vector<point>::const_iterator it = std::find( pc.begin(), pc.end(), point( x, y ) );
  if ( ValidIndex( x, y ) && it == pc.end() )
    return true;
  return false;
}

std::vector<std::string> MakeWordVector( std::string word )
{
  char prev = 0;
  std::vector<std::string> temp;
  std::string str;
  for ( std::string::iterator it = word.begin(); it != word.end(); ++it )
  {
    str.push_back(*it);
    if ( *it == 'Q' )
      continue;
    temp.push_back(str);
    str.clear();
  }
  return temp;
}

bool MatchWordToSearched( std::string *pgrid, std::vector<std::string> word, std::vector<point> previousChoices )
{
  if ( word.size() != previousChoices.size() )
    return false;

  for ( int i = 0; i < word.size(); ++i )
    if ( word[i] != pgrid[previousChoices[i].y*4+previousChoices[i].x] )
      return false;
  return true;
}

bool FindWord( std::string *pgrid, std::vector<std::string> word, int wpos, std::vector<point> previousChoices )
{
  if ( MatchWordToSearched( pgrid, word, previousChoices ) )
    return true;

  point pos = previousChoices.back();

  point points[8] = 
  {
    point(pos.x, pos.y+1),
    point(pos.x+1, pos.y+1),
    point(pos.x+1, pos.y),
    point(pos.x+1, pos.y-1),
    point(pos.x, pos.y-1),
    point(pos.x-1, pos.y-1),
    point(pos.x-1, pos.y),
    point(pos.x-1, pos.y+1)
  };

  for ( int i = 0; i < 8; ++i )
  {
    if ( IsValid( points[i].x, points[i].y, previousChoices ) )
    {
      if ( IndexCube(pgrid, points[i].x, points[i].y) == word[wpos] )
      {
        std::vector<point> pc = previousChoices;
        pc.push_back(points[i]);
        return FindWord( pgrid, word, wpos+1, pc );
      }
    }
  }

  return false;
}

bool FindWord( std::string *pgrid, const std::string &word )
{
  std::vector<std::string> wlist = MakeWordVector(word);
  int wsize = wlist.size();
  if ( wsize < 3 )
    return 0;
  for ( int i = 0; i < 16; ++i )
  {
    if ( pgrid[i] == wlist[0] )
    {
      std::vector<point> pc;
      pc.push_back(point(i%4, i/4));
      return FindWord( pgrid, wlist, 1, pc );
    }
  }
  return false;
}

void stoupper(std::string& s)
{
  std::string::iterator i = s.begin();
  std::string::iterator end = s.end();

  while (i != end) {
    *i = std::toupper((unsigned char)*i);
    ++i;
  }
}


int main()
{
  srand(Time::Clock());
  BuildGrid( cubes, grid );
  PrintGrid( grid );
  if ( FindWord( grid, "GOALIE" ) )
    std::cout << "yeah" << std::endl;

  std::string words;
  std::vector<std::string> subwords;
  LoadFile( "web2.txt", words );
  SplitWords( words, subwords );

  std::vector<std::string> sublist;

  for ( int i = 0; i < subwords.size(); ++i )
  {
    stoupper(subwords[i]);
    if ( FindWord( grid, subwords[i] ) )
      sublist.push_back( subwords[i] );
  }

  Output( "output.txt", sublist );

  return 0;
}