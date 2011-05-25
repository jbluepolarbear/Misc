/* Start Header -------------------------------------------------------
Copyright (C) 2009 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vec2.h
Purpose: Vector 2
Language: C++, Visual Studio 2005
Platform: Visual Studio 8, Windows
Author: Jeremy Anderson, janders2, 40000706
- End Header --------------------------------------------------------*/
#pragma once

#ifndef VEC2_H
#define VEC2_H

class Vec2
{
  public:
    //Vec2(){}
    Vec2( const float x = 0.0f, const float y = 0.0f );
    Vec2( float *rhs );

    Vec2 &operator =( const Vec2 &rhs );

    //operator float *();

    Vec2 operator -() const;
    Vec2 operator +( const Vec2 &rhs ) const;
    Vec2 operator +( const float rhs ) const;
    Vec2 operator -( const Vec2 &rhs ) const;
    Vec2 &operator +=( const Vec2 &rhs );
    Vec2 &operator -=( const Vec2 &rhs );
    Vec2 &operator /=( const float rhs );
    Vec2 operator /( const float rhs ) const;
    float operator *( const Vec2 &rhs ) const;
    Vec2 operator *( const float s ) const;

    Vec2 Normalize() const;
    float dot( const Vec2& Vec2 ) const;
    float length( void ) const;
    float lengthSquared( void ) const;

    float x, y;
};

float Distance( const Vec2 &lhs, const Vec2 &rhs );
float DistanceSquared( const Vec2 &lhs, const Vec2 &rhs );

#endif