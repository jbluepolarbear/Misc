/* Start Header -------------------------------------------------------
Copyright (C) 2009 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix.cpp
Purpose: Matrix
Language: C++, Visual Studio 2005
Platform: Visual Studio 8, Windows
Project: janders2_CS250_2
Author: Jeremy Anderson, janders2, 40000706
Creation date: Monday, January 20, 2009
- End Header --------------------------------------------------------*/
#include "Matrix.h"
#include <cmath>

Matrix::Matrix( void )
{
}

Matrix::Matrix( const float _11, const float _12, const float _13, const float _14,
                const float _21, const float _22, const float _23, const float _24,
                const float _31, const float _32, const float _33, const float _34,
                const float _41, const float _42, const float _43, const float _44 ) :
_11( _11 ), _12( _12 ), _13( _13 ), _14( _14 ),
_21( _21 ), _22( _22 ), _23( _23 ), _24( _24 ),
_31( _31 ), _32( _32 ), _33( _33 ), _34( _34 ),
_41( _41 ), _42( _42 ), _43( _43 ), _44( _44 )
{
}

Matrix::Matrix( const float *i, const float *j, const float *k, const float *O ) :
_11( i[0] ), _12( j[0] ), _13( k[0] ), _14( O[0] ),
_21( i[1] ), _22( j[1] ), _23( k[1] ), _24( O[1] ),
_31( i[2] ), _32( j[2] ), _33( k[2] ), _34( O[2] ),
_41( i[3] ), _42( j[3] ), _43( k[3] ), _44( O[3] )
{
}

Matrix::operator float *( void )
{
  return Data;
}

Matrix Matrix::operator *( const Matrix &Value ) const
{
  return Matrix(
    _11  * Value._11 + _12  * Value._21 + _13  * Value._31 + _14  * Value._41,
    _11  * Value._12 + _12  * Value._22 + _13  * Value._32 + _14  * Value._42,
    _11  * Value._13 + _12  * Value._23 + _13  * Value._33 + _14  * Value._43,
    _11  * Value._14 + _12  * Value._24 + _13  * Value._34 + _14  * Value._44,
    _21  * Value._11 + _22  * Value._21 + _23  * Value._31 + _24  * Value._41,
    _21  * Value._12 + _22  * Value._22 + _23  * Value._32 + _24  * Value._42,
    _21  * Value._13 + _22  * Value._23 + _23  * Value._33 + _24  * Value._43,
    _21  * Value._14 + _22  * Value._24 + _23  * Value._34 + _24  * Value._44,
    _31  * Value._11 + _32  * Value._21 + _33  * Value._31 + _34  * Value._41,
    _31  * Value._12 + _32  * Value._22 + _33  * Value._32 + _34  * Value._42,
    _31  * Value._13 + _32  * Value._23 + _33  * Value._33 + _34  * Value._43,
    _31  * Value._14 + _32  * Value._24 + _33  * Value._34 + _34  * Value._44,
    _41  * Value._11 + _42  * Value._21 + _43  * Value._31 + _44  * Value._41,
    _41  * Value._12 + _42  * Value._22 + _43  * Value._32 + _44  * Value._42,
    _41  * Value._13 + _42  * Value._23 + _43  * Value._33 + _44  * Value._43,
    _41  * Value._14 + _42  * Value._24 + _43  * Value._34 + _44  * Value._44 );
}

Matrix Matrix::operator +( const Matrix &Value ) const
{
  return Matrix
  (
    _11 + Value._11, _12 + Value._12, _13 + Value._13, _14 + Value._14,
    _21 + Value._21, _22 + Value._22, _23 + Value._23, _24 + Value._24,
    _31 + Value._31, _32 + Value._32, _33 + Value._33, _34 + Value._34,
    _41 + Value._41, _42 + Value._42, _43 + Value._43, _44 + Value._44
  );
}

const float &Matrix::operator []( const int Index ) const
{
  return Data[Index];
}

float &Matrix::operator []( const int Index )
{
  return Data[Index];
}

void Matrix::GetColomn( unsigned colomn, float *vec )
{
  vec[0] = Data[ 0 * 4 + colomn ];
  vec[1] = Data[ 1 * 4 + colomn ];
  vec[2] = Data[ 2 * 4 + colomn ];
  vec[3] = Data[ 3 * 4 + colomn ];
}
void Matrix::GetRow( unsigned row, float *vec )
{
  vec[0] = Data[ row * 4 + 0 ];
  vec[1] = Data[ row * 4 + 1 ];
  vec[2] = Data[ row * 4 + 2 ];
  vec[3] = Data[ row * 4 + 3 ];
}

Matrix Matrix::Transpose() const
{
  return Matrix(
    _11, _21, _31, _41,
    _12, _22, _32, _42,
    _13, _23, _33, _43,
    _14, _24, _34, _44 );
}

Matrix &Matrix::SetIdentity( void )
{
  *this = Matrix( 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f );
  return *this;
}

//creates the translation matrix
Matrix Translate( const float x, const float y, const float z )
{
  return Matrix( 
    1.0, 0.0, 0.0, x,
    0.0, 1.0, 0.0, y,
    0.0, 0.0, 1.0, z,
    0.0, 0.0, 0.0, 1.0 );
}

Matrix Translate( const float *vec )
{
  return Translate( vec[0], vec[1], vec[2] );
}


//creates the scale matrix
Matrix Scale( const float x, const float y, const float z )
{
  return Matrix( 
    x, 0.0, 0.0, 0.0,
    0.0, y, 0.0, 0.0,
    0.0, 0.0, z, 0.0,
    0.0, 0.0, 0.0, 1.0 );
}

//rotate x
Matrix RotateX( const float angle )
{
  return Matrix( 
    1, 0.0, 0.0, 0.0,
    0.0, cos( Deg2Rad( angle ) ), -sin( Deg2Rad( angle ) ), 0.0,
    0.0, sin( Deg2Rad( angle ) ), cos( Deg2Rad( angle ) ), 0.0,
    0.0, 0.0, 0.0, 1.0 );
}

//rotate y
Matrix RotateY( const float angle )
{
  return Matrix( 
    cos( Deg2Rad( angle ) ), 0.0, sin( Deg2Rad( angle ) ), 0.0,
    0.0, 1.0, 0.0, 0.0,
    -sin( Deg2Rad( angle ) ), 0.0, cos( Deg2Rad( angle ) ), 0.0,
    0.0, 0.0, 0.0, 1.0 );
}

//rotate z
Matrix RotateZ( const float angle )
{
  return Matrix( 
    cos( Deg2Rad( angle ) ), -sin( Deg2Rad( angle ) ), 0.0, 0.0,
    sin( Deg2Rad( angle ) ), cos( Deg2Rad( angle ) ), 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0 );
}

Matrix Identity()
{
  return Matrix(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0 );
}
