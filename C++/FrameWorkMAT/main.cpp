/* Start Header -------------------------------------------------------
File Name: main.cpp
Purpose: Implements the NLI, BB, and MidPoint alogrithms.
Language: C++, Visual Studio 2008
Platform: Visual Studio 9, Windows
Author: Jeremy Anderson
- End Header --------------------------------------------------------*/
#include "FrameWork.h"

extern const int MAX_DIMENSION = 64;

//Pascals triangle
float combo( int d, int i )
{
  static float n[MAX_DIMENSION][MAX_DIMENSION] = {0};

  i = i > d/2 ? d - i : i;

  if ( n[d][i] == 0 )
  {
    if ( d == 1 || i == 0 || d == i )
      n[d][i] = 1;
    else if ( i == 1 )
      n[d][i] = d;
    else if ( i > d )
      n[d][i] = 0;
    else
      n[d][i] = combo( d-1, i-1 ) + combo( d-1, i );
  }
  return n[d][i];
}

enum Modes
{
  NLI = 0,
  BB,
  MIDPOINT,
};

FrameWork fw;

// simple linear interpolation between two points
void lerp( FrameWork::Point &dest, FrameWork::Point &a, FrameWork::Point &b, float t )
{
  dest = a + ( b - a ) * t;
}

// evaluate a point on a bezier-curve. t goes from 0 to 1.0
void bezier( FrameWork::Point &dest, const std::vector< FrameWork::Point > &inpoints,  float t )
{
  std::vector< FrameWork::Point > points;

  points = inpoints;

  std::vector< FrameWork::Point > points2;

  while ( points.size() > 2 )
  {
    for ( unsigned i = 1; i < points.size(); ++i )
    {
      FrameWork::Point point;
      lerp( point, points[ i - 1 ], points[ i ], t );
      points2.push_back( point );
    }
    points = points2;
    points2.clear();
  }

  lerp( dest, points[0], points[1], t );
}

void DrawShell()
{
  if ( !fw.GetDrawShell() )
    return;

  std::vector< FrameWork::Point > points = fw.GetPoints();
  float t = fw.GetTvalue();

  std::vector< FrameWork::Point > points2;

  std::vector< FrameWork::Point > points3;

  while ( points.size() > 2 )
  {
    for ( unsigned i = 1; i < points.size(); ++i )
    {
      FrameWork::Point point;
      lerp( point, points[ i - 1 ], points[ i ], t );
      points2.push_back( point );
    }
    points = points2;
    fw.DrawLines( points, NULL );
    points3.insert( points3.end(), points2.begin(), points2.end() );
    points2.clear();
  }

  fw.DrawPoints( points3, NULL );
}

void DrawNLI()
{
  const std::vector< FrameWork::Point > &points = fw.GetPoints();

  if ( points.size() > 1 )
  {
    std::vector< FrameWork::Point > points2;
    float step = fw.GetStep();

    points2.push_back( points.front() );

    for ( float t = step; t < 1.0f; t += step )
    {
      FrameWork::Point point;
      bezier( point, points, t );
      points2.push_back( point );
    }

    points2.push_back( points.back() );

    float color[3] = { 0.0f, 0.0f, 1.0f };

    DrawShell();

    fw.DrawLines( points2, color );
  }
}

#include <cmath>

FrameWork::Point B( const std::vector< FrameWork::Point > &points, float t )
{
  FrameWork::Point point;

  int d = points.size() - 1;
  for ( int i = 0; i < points.size(); i++ )
  {
    point += points[i] * ( combo( d, i ) * pow( 1 - t, d - i ) * pow( t, i ) );
  }

  return point;
}

void DrawBB()
{
  const std::vector< FrameWork::Point > &points = fw.GetPoints();

  if ( points.size() > 1 )
  {
    std::vector< FrameWork::Point > points2;
    float step = fw.GetStep();

    points2.push_back( points.front() );

    for ( float t = step; t < 1.0f; t += step )
    {
      points2.push_back( B( points, t ) );
    }

    points2.push_back( points.back() );

    float color[3] = { 0.0f, 0.0f, 1.0f };

    fw.DrawLines( points2, color );
  }
}

#include <algorithm>
#include <list>

void DrawMidPoint( std::vector< FrameWork::Point > points, int depth )
{
  if ( points.size() > 1 )
  {
    std::vector< FrameWork::Point > points2;
    std::vector< FrameWork::Point > pointsf;
    std::vector< FrameWork::Point > pointsb;

    while ( points.size() > 1 )
    {
      pointsf.push_back( points.front() );
      pointsb.push_back( points.back() );
      for ( unsigned i = 1; i < points.size(); i++ )
      {
        FrameWork::Point point;
        lerp( point, points[i - 1], points[i], 0.5 );
        points2.push_back( point );
      }
      points = points2;
      points2.clear();
    }
    pointsf.push_back( points.back() );
    pointsb.push_back( points.back() );

    std::reverse( pointsb.begin(), pointsb.end() );

    float color[3] = { 0.0f, 0.0f, 1.0f };


    if ( depth == 0 )
    {
      fw.DrawLines( pointsf, color );
      fw.DrawLines( pointsb, color );
    }

    if ( depth > 0 )
    {
      DrawMidPoint( pointsf, depth - 1 );
      DrawMidPoint( pointsb, depth - 1 );
    }
  }
}

void DrawMidPoint()
{
  std::vector< FrameWork::Point > points( fw.GetPoints() );
  DrawMidPoint( points, 7 * fw.GetTvalue() );
}

int main( int argv, char *argc[] )
{
  TwEnumVal modes[] =
  { 
    { Modes::NLI, "NLI"}, 
    { Modes::BB, "BB" }, 
    { Modes::MIDPOINT, "MidPoint" }
  };

  fw.Init();

  fw.SetModes( modes, 3 );

  fw.SetDrawFunc( DrawNLI );

  int mode = fw.GetMode();

  FrameWork::Point p( 2.0f, 3.0f );
  p /= 3.0f;

  while ( fw.Running() )
  {
    fw.Update();

    int nmode = fw.GetMode();

    if ( mode != nmode )
    {
      mode = nmode;
      switch ( nmode )
      {
        case Modes::NLI:
          fw.SetDrawFunc( DrawNLI );
          break;
        case Modes::BB:
          fw.SetDrawFunc( DrawBB );
          break;
        case Modes::MIDPOINT:
          fw.SetDrawFunc( DrawMidPoint );
          break;
      }
    }

    fw.Draw();
  }

  fw.Exit();

  return 0;
}
