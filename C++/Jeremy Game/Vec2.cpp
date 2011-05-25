/* Start Header -------------------------------------------------------
File Name: Vector.cpp
Purpose: Vector
Language: C++, Visual Studio 2005
Platform: Visual Studio 8, Windows
Author: Jeremy Anderson
Creation date: Monday, January 20, 2009
- End Header --------------------------------------------------------*/
#include "Vec2.h"
#include <cmath>

Vec2::Vec2( const float x, const float y ) : x( x ), y( y )
{
}

Vec2::Vec2( float *rhs ) : x( rhs[0] ), y( rhs[1] )
{
}

Vec2 &Vec2::operator =( const Vec2 &rhs )
{
  //if ( rhs.w == 0.0f || rhs.w == 1.0f )
  //{
  x = rhs.x; y = rhs.y;
  //}
  //else
  //{
  //  x = rhs.x / rhs.w; y = rhs.y / rhs.w; z = rhs.z / rhs.w; w = 1.0f;
  //}

  return *this;
}


//Vec2::operator float *()
//{
//  return Data;
//}

Vec2 Vec2::operator -() const
{
  return Vec2( -x, -y );
}

Vec2 Vec2::operator +( const Vec2 &rhs ) const
{
  return Vec2( *this ) += rhs;
}

Vec2 Vec2::operator +( const float rhs ) const
{
  return Vec2( x + rhs, y + rhs );
}

Vec2 Vec2::operator -( const Vec2 &rhs ) const
{
  return Vec2( *this ) -= rhs;
}

Vec2 &Vec2::operator +=( const Vec2 &rhs )
{
  x += rhs.x; y += rhs.y;
  return *this; 
}

Vec2 &Vec2::operator -=( const Vec2 &rhs )
{
  x -= rhs.x; y -= rhs.y;
  return *this; 
}

Vec2 &Vec2::operator /=( const float rhs )
{
  x /= rhs; y /= rhs;
  return *this; 
}

Vec2 Vec2::operator /( const float rhs ) const
{
  Vec2 temp = *this;
  temp /= rhs;
  return temp; 
}

float Vec2::operator *( const Vec2 &rhs ) const
{
  return x * rhs.x + y * rhs.y;
}

Vec2 Vec2::operator *( const float s ) const
{
  return Vec2( x * s, y * s );
}


Vec2 Vec2::Normalize() const
{
  return *this / length();
}

float Vec2::dot( const Vec2& rhs ) const
{
  return x * rhs.x + y * rhs.y;
}

float Vec2::length( void ) const
{
  return sqrt( lengthSquared() );
}

float Vec2::lengthSquared( void ) const
{
  return this->dot( *this );
}

float Distance( const Vec2 &lhs, const Vec2 &rhs )
{
  Vec2 temp = lhs - rhs;
  return temp.length();
}

float DistanceSquared( const Vec2 &lhs, const Vec2 &rhs )
{
  Vec2 temp = lhs - rhs;
  return temp.lengthSquared();
}
