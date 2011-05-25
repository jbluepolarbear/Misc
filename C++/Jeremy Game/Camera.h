/* Start Header -------------------------------------------------------
File Name: Camera.h
Purpose: Camera Class
Language: C++, Visual Studio 2008
Platform: Visual Studio 9, Windows
Author: Jeremy Anderson
Creation date: Friday, September 26, 2009
- End Header --------------------------------------------------------*/
#pragma once
#include "Vec2.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

#define PI M_PI

class Camera
{
  public:
    Camera( const Vec2 &pos = Vec2( 0.0f, 0.0f ), int width = 0, int height = 0 );

    void SetScreenSize( int width, int height );
    void SetView();

    void Move( const Vec2 &amount );

    void MoveVertical( float amount );
    void MoveHorizontal( float amount );

    void SetRotate( float angle );
    float GetRotate();

    void SetPosition( const Vec2 &pos );
    Vec2 GetPosition();

    void SetDistance( float dist );
    float GetDistance();

  private:
    int width;
    int height;
    Vec2 position;
    float curangle;
    float distance;
};