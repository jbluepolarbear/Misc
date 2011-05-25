/* Start Header -------------------------------------------------------
Copyright (C) 2009 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector.h
Purpose: Vector
Language: C++, Visual Studio 2005
Platform: Visual Studio 8, Windows
Project: janders2_CS250_2
Author: Jeremy Anderson, janders2, 40000706
Creation date: Monday, January 20, 2009
- End Header --------------------------------------------------------*/
#pragma once

#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"

class Vector
{
  public:
    //Vector(){}
    Vector( const float x = 0.0f, const float y = 0.0f, const float z = 0.0f, const float w = 0.0f );
    Vector( float *rhs );

    Vector &operator =( const Vector &rhs );

    //operator float *();

    Vector operator -() const;
    Vector operator +( const Vector &rhs ) const;
    Vector operator +( const float rhs ) const;
    Vector operator -( const Vector &rhs ) const;
    Vector &operator +=( const Vector &rhs );
    Vector &operator -=( const Vector &rhs );
    Vector &operator /=( const float rhs );
    Vector operator /( const float rhs ) const;
    float operator *( const Vector &rhs ) const;
    Vector operator *( const float s ) const;

    Vector Normalize() const;
    Matrix Skew() const;

    Vector cross( const Vector& vector ) const;
    float dot( const Vector& vector ) const;
    float length( void ) const;
    float lengthSquared( void ) const;

    float x, y, z, w;
};

typedef Vector Vec4;

Vector operator *( const Matrix &lhs, const Vector &rhs );
Vector MatrixMulVector( const Matrix &lhs, const Vector &rhs );

Vector CrossProduct( const Vector &v1, const Vector &v2 );

float Distance( const Vector &lhs, const Vector &rhs );
float DistanceSquared( const Vector &lhs, const Vector &rhs );

#endif
