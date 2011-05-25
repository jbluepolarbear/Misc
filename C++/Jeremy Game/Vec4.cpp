/* Start Header -------------------------------------------------------
Copyright (C) 2009 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector.cpp
Purpose: Vector
Language: C++, Visual Studio 2005
Platform: Visual Studio 8, Windows
Project: janders2_CS250_2
Author: Jeremy Anderson, janders2, 40000706
Creation date: Monday, January 20, 2009
- End Header --------------------------------------------------------*/
#include "Vec4.h"
#include <cmath>

Vector::Vector( const float x, const float y, const float z, const float w ) : x( x ), y( y ), z( z ), w( w )
{
}

Vector::Vector( float *rhs ) : x( rhs[0] ), y( rhs[1] ), z( rhs[2] ), w( rhs[3] )
{
}

Vector &Vector::operator =( const Vector &rhs )
{
  //if ( rhs.w == 0.0f || rhs.w == 1.0f )
  //{
    x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
  //}
  //else
  //{
  //  x = rhs.x / rhs.w; y = rhs.y / rhs.w; z = rhs.z / rhs.w; w = 1.0f;
  //}

  return *this;
}


//Vector::operator float *()
//{
//  return Data;
//}

Vector Vector::operator -() const
{
  return Vector( -x, -y, -z, -w );
}

Vector Vector::operator +( const Vector &rhs ) const
{
  return Vector( *this ) += rhs;
}

Vector Vector::operator +( const float rhs ) const
{
  return Vector( x + rhs, y + rhs, z + rhs );
}

Vector Vector::operator -( const Vector &rhs ) const
{
  return Vector( *this ) -= rhs;
}

Vector &Vector::operator +=( const Vector &rhs )
{
  x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
  return *this; 
}

Vector &Vector::operator -=( const Vector &rhs )
{
  x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
  return *this; 
}

Vector &Vector::operator /=( const float rhs )
{
  x /= rhs; y /= rhs; z /= rhs; w /= rhs;
  return *this; 
}

Vector Vector::operator /( const float rhs ) const
{
  Vector temp = *this;
  temp /= rhs;
  return temp; 
}

float Vector::operator *( const Vector &rhs ) const
{
  return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

Vector Vector::operator *( const float s ) const
{
  return Vector( x * s, y * s, z * s, w * s );
}


Vector Vector::Normalize() const
{
  return *this / length();
}

Matrix Vector::Skew() const
{
  return Matrix( 0,-z, y, 0,
                 z, 0,-x, 0,
                -y, x, 0, 0,
                 0, 0, 0, w );
}

Vector operator *( const Matrix &lhs, const Vector &rhs )
{
  return Vector(
    rhs.x * lhs._11 + rhs.y * lhs._12 + rhs.z * lhs._13 + rhs.w * lhs._14,
    rhs.x * lhs._21 + rhs.y * lhs._22 + rhs.z * lhs._23 + rhs.w * lhs._24,
    rhs.x * lhs._31 + rhs.y * lhs._32 + rhs.z * lhs._33 + rhs.w * lhs._34,
    rhs.x * lhs._41 + rhs.y * lhs._42 + rhs.z * lhs._43 + rhs.w * lhs._44 );
}

Vector MatrixMulVector( const Matrix &lhs, const Vector &rhs )
{
  return lhs * rhs;
}

Vector CrossProduct( const Vector &v1, const Vector &v2 )
{
  return Vector( v1.y * v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x, 1.0f );
}

Vector Vector::cross( const Vector& vector ) const
{
  return Vector( y * vector.z - z * vector.y,
                 z * vector.x - x * vector.z,
                 x * vector.y - y * vector.x, 1.0f );
}

float Vector::dot( const Vector& vector ) const
{
  return x * vector.x + y * vector.y + z * vector.z;
}

float Vector::length( void ) const
{
  return sqrt( lengthSquared() );
}

float Vector::lengthSquared( void ) const
{
  return this->dot( *this );
}

float Distance( const Vector &lhs, const Vector &rhs )
{
  Vector temp = lhs - rhs;
  return temp.length();
}

float DistanceSquared( const Vector &lhs, const Vector &rhs )
{
  Vector temp = lhs - rhs;
  return temp.lengthSquared();
}
