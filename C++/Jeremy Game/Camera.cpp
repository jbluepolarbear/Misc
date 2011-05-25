/* Start Header -------------------------------------------------------
File Name: Camera.cpp
Purpose: Camera Class
Language: C++, Visual Studio 2008
Platform: Visual Studio 9, Windows
Author: Jeremy Anderson
Creation date: Friday, September 26, 2009
- End Header --------------------------------------------------------*/
#include "SDL/SDL_opengl.h"
#include "Camera.h"
#include "Matrix.h"
#include "Vec4.h"

//////////////////////////////////////////////////////////////////////////

Camera::Camera( const Vec2 &pos, int width, int height )
{
  position = pos;
  this->width = width;
  this->height = height;
  curangle = 0.0f;
  distance = 1.0f;
}

void Camera::SetScreenSize( int width, int height )
{
  this->width = width;
  this->height = height;
}

void Camera::SetView()
{
  Matrix rot = RotateZ( curangle );
  Matrix scl = Scale( distance, distance );
  Matrix pos = Translate( position.x, position.y );
  Matrix offset = Translate( -width/2.0f, -height/2.0f );

  Matrix transform = pos * rot * scl * offset;

  glMultMatrixf( transform.Transpose() );
}

void Camera::Move( const Vec2 &amount )
{
  position += amount;
}

void Camera::MoveVertical( float amount )
{
  position.y += amount;
}

void Camera::MoveHorizontal( float amount )
{
  position.x += amount;
}

void Camera::SetRotate( float angle )
{
  curangle = angle;
}

float Camera::GetRotate()
{
  return curangle;
}

void Camera::SetPosition( const Vec2 &pos )
{
  position = pos;
}

Vec2 Camera::GetPosition()
{
  return position;
}

void Camera::SetDistance( float dist )
{
  distance = dist;

  if ( distance < 0.0f )
    distance = 0.0f;
}

float Camera::GetDistance()
{
  return distance;
}


