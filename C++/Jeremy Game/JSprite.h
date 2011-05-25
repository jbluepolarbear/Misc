//-------------------------------------------------------------------------------
// Name:        JSprite.h
// Purpose:     Jeremy's sprite drawing made easy.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------
#ifndef JSPRITE_H
#define JSPRITE_H

#include <vector>
#include <map>
#include <list>
#include "Vec2.h"
#include "Vec3.h"
#include "Shader.h"

struct JRect
{
  JRect( float left = 0.0f, float right = 0.0f, float top = 0.0f, float bottom = 0.0f );
  bool operator ==( const JRect &rhs ) const;
  bool operator !=( const JRect &rhs ) const;
  float &operator []( const int Index );
  const float &operator []( const int Index ) const;
  float left;
  float right;
  float top;
  float bottom;
};

class JSprite
{
  public:
    JSprite();
    virtual ~JSprite();
    void Init();
    void Begin();
    void End();
    Shader shader;
    Matrix BuildTransformation( const Vec2 &position, const Vec2 &center, const Vec2 &scale = Vec2( 1.0f, 1.0f ), float direction = 0.0f );
    // sends information to be stored in objects for later drawing.
    void Draw( unsigned texture, const Matrix &transformation, const JRect &textureCoord, float zlevel = -1.0f, float alpha = 1.0f, bool useCamera = true, const Vec3 &color = Vec3( 1.0f, 0.0f, 1.0f ) );
    // draws all stored objects then clears it.
    void DrawBatch();
    struct JDrawObject
    {
      unsigned texture;
      Matrix transformation;
      JRect textureCoord;
      float zlevel;
      float alpha;
      bool useCamera;
      Vec3 color;

      bool operator <( const JDrawObject &rhs )
      {
        return zlevel < rhs.zlevel;
      }
      bool operator >( const JDrawObject &rhs )
      {
        return zlevel > rhs.zlevel;
      }
    };
  private:

    std::vector<JDrawObject> objects;
};

#endif /*JSPRITE_H*/
