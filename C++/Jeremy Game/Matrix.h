/* Start Header -------------------------------------------------------
Copyright (C) 2009 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix.h
Purpose: Matrix
Language: C++, Visual Studio 2005
Platform: Visual Studio 8, Windows
Project: janders2_CS250_2
Author: Jeremy Anderson, janders2, 40000706
Creation date: Monday, January 20, 2009
- End Header --------------------------------------------------------*/
#pragma once

#ifndef MATRIX_H
#define MATRIX_H

inline float Deg2Rad( float x ){ return 0.017453f * x; }
inline float Rad2Deg( float x ){ return x * 57.295779f; }

class Matrix
{
  public:
    Matrix( void );
    Matrix( const float _11, const float _12, const float _13, const float _14,
            const float _21, const float _22, const float _23, const float _24,
            const float _31, const float _32, const float _33, const float _34,
            const float _41, const float _42, const float _43, const float _44 );
    Matrix::Matrix( const float *i, const float *j, const float *k, const float *O );

    operator float *( void );

    Matrix operator *( const Matrix &Value ) const;
    Matrix operator +( const Matrix &Value ) const;
    const float &operator []( const int Index ) const;
    float &operator []( const int Index );

    void GetColomn( unsigned colomn, float *vec );
    void GetRow( unsigned row, float *vec );

    Matrix &SetIdentity( void );

    float get( const int Index )
    {
      return Data[Index];
    }

    void set( const int Index, float val )
    {
      Data[Index] = val;
    }

    Matrix Transpose() const;

  private:
  #pragma  warning( disable : 4201 )
    union
    {
      struct
      {
        float
          _11, _12, _13, _14,
          _21, _22, _23, _24,
          _31, _32, _33, _34,
          _41, _42, _43, _44;
      };
      float Data[16];
    };
};

//creates the translations matrix
Matrix Translate( const float x, const float y, const float z = 0.0f );
Matrix Translate( const float *vec );
//creates the scale matrix
Matrix Scale( const float x, const float y, const float z = 1.0f );
//rotate x
Matrix RotateX( const float angle );
//rotate y
Matrix RotateY( const float angle );
//rotate z
Matrix RotateZ( const float angle );
// identity
Matrix Identity();

#endif
