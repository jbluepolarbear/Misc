/* Start Header -------------------------------------------------------
Copyright (C) 2009 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vec3.cpp
Purpose: Vec3
Language: C++, Visual Studio 2005
Platform: Visual Studio 8, Windows
Project: janders2_CS250_2
Author: Jeremy Anderson, janders2, 40000706
Creation date: Monday, January 20, 2009
- End Header --------------------------------------------------------*/
#include "Vec3.h"
#include <cmath>

Vec3::Vec3( const float x, const float y, const float z ) : x( x ), y( y ), z( z )
{
}

Vec3::Vec3( float *rhs ) : x( rhs[0] ), y( rhs[1] ), z( rhs[2] )
{
}

Vec3 &Vec3::operator =( const Vec3 &rhs )
{
  //if ( rhs.w == 0.0f || rhs.w == 1.0f )
  //{
  x = rhs.x; y = rhs.y; z = rhs.z;
  //}
  //else
  //{
  //  x = rhs.x / rhs.w; y = rhs.y / rhs.w; z = rhs.z / rhs.w; w = 1.0f;
  //}

  return *this;
}


//Vec3::operator float *()
//{
//  return Data;
//}

Vec3 Vec3::operator -() const
{
  return Vec3( -x, -y, -z );
}

Vec3 Vec3::operator +( const Vec3 &rhs ) const
{
  return Vec3( *this ) += rhs;
}

Vec3 Vec3::operator +( const float rhs ) const
{
  return Vec3( x + rhs, y + rhs, z + rhs );
}

Vec3 Vec3::operator -( const Vec3 &rhs ) const
{
  return Vec3( *this ) -= rhs;
}

Vec3 &Vec3::operator +=( const Vec3 &rhs )
{
  x += rhs.x; y += rhs.y; z += rhs.z;
  return *this; 
}

Vec3 &Vec3::operator -=( const Vec3 &rhs )
{
  x -= rhs.x; y -= rhs.y; z -= rhs.z;
  return *this; 
}

Vec3 &Vec3::operator /=( const float rhs )
{
  x /= rhs; y /= rhs; z /= rhs;
  return *this; 
}

Vec3 Vec3::operator /( const float rhs ) const
{
  Vec3 temp = *this;
  temp /= rhs;
  return temp; 
}

float Vec3::operator *( const Vec3 &rhs ) const
{
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vec3 Vec3::operator *( const float s ) const
{
  return Vec3( x * s, y * s, z * s );
}


Vec3 Vec3::Normalize() const
{
  return *this / length();
}

Matrix Vec3::Skew() const
{
  return Matrix( 0,-z, y, 0,
    z, 0,-x, 0,
    -y, x, 0, 0,
    0, 0, 0, 1.0f );
}

Vec3 operator *( const Matrix &lhs, const Vec3 &rhs )
{
  return Vec3(
    rhs.x * lhs._11 + rhs.y * lhs._12 + rhs.z * lhs._13,
    rhs.x * lhs._21 + rhs.y * lhs._22 + rhs.z * lhs._23,
    rhs.x * lhs._31 + rhs.y * lhs._32 + rhs.z * lhs._33 );
}


Vec3 MatrixMulVector( const Matrix &lhs, const Vec3 &rhs )
{
  return lhs * rhs;
}

Vec3 CrossProduct( const Vec3 &v1, const Vec3 &v2 )
{
  return Vec3( v1.y * v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x );
}

Vec3 Vec3::cross( const Vec3& rhs ) const
{
  return Vec3( y * rhs.z - z * rhs.y,
    z * rhs.x - x * rhs.z,
    x * rhs.y - y * rhs.x );
}

float Vec3::dot( const Vec3& rhs ) const
{
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

float Vec3::length( void ) const
{
  return sqrt( lengthSquared() );
}

float Vec3::lengthSquared( void ) const
{
  return this->dot( *this );
}

float Distance( const Vec3 &lhs, const Vec3 &rhs )
{
  Vec3 temp = lhs - rhs;
  return temp.length();
}

float DistanceSquared( const Vec3 &lhs, const Vec3 &rhs )
{
  Vec3 temp = lhs - rhs;
  return temp.lengthSquared();
}
