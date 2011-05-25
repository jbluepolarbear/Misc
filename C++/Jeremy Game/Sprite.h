//-------------------------------------------------------------------------------
// Name:        Sprite.h
// Purpose:     Jeremy's sprite drawing made easy.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------
#ifndef SPRITE_H
#define SPRITE_H

#include "Vec2.h"
#include "Vec3.h"
#include "Matrix.h"
#include "Shader.h"

namespace JeremySprite
{
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

  namespace JBlend
  {
    enum Mode
    {
      Alpha,    ///< Pixel = Src * a + Dest * (1 - a)
      Add,      ///< Pixel = Src + Dest
      Multiply, ///< Pixel = Src * Dest
      None      ///< No blending
    };
  }

  class JSprite
  {
    public:
      JSprite( const unsigned image = 0 );
      virtual ~JSprite();
      ///////////////////////////
      void SetPosition( const Vec2 &npos );
      void SetPosition( const float x, const float y );
      const Vec2 &GetPosition() const;
      const float GetPositionX() const;
      const float GetPositionY() const;

      void SetCenter( const Vec2 &nctr );
      void SetCenter( const float x, const float y );
      const Vec2 &GetCenter() const;
      const float GetCenterX() const;
      const float GetCenterY() const;

      void SetScale( const Vec2 &nscl );
      void SetScale( const float x, const float y );
      const Vec2 &GetScale() const;
      const float GetScaleX() const;
      const float GetScaleY() const;

      void SetRotation( const float angle );
      const float GetRotation() const;

      void SetZLevel( const float zlvl );
      const float GetZLevel() const;

      void SetAlpha( const float nalpha );
      const float GetAlpha() const;

      void SetUseCamera( const bool nusecam );
      const bool GetUseCamera() const;

      void SetTextureCoord( const JRect &nTexCoord );
      void SetTextureCoord( const float left, const float right, const float top, const float bottom );
      void MoveTextureCoord( const Vec2 &distance );
      void MoveTextureCoord( const float x, const float y );
      const JRect &GetTextureCoord() const;

      void SetTexture( const unsigned ntexture );
      const unsigned GetTexture() const;

      void SetColor( const Vec3 &ncolor );
      const Vec3 &GetColor() const;

      void SetBlendMode( const JBlend::Mode &nblendmode );
      const JBlend::Mode &GetBlendMode() const;

      ///////////////////////////
      const Matrix &GetTransformation() const;
    private:
      friend class JSpriteRenderer;
      void Draw() const;
      mutable Matrix transformation;

      Vec2 position;
      Vec2 center;
      Vec2 scale;
      float rotation;

      unsigned texture;
      JRect textureCoord;
      float zlevel;
      float alpha;
      bool useCamera;
      Vec3 color;

      JBlend::Mode blendMode;

      mutable bool buildTransformation;
  };

  class JSpriteRenderer
  {
    public:
      JSpriteRenderer();
      virtual ~JSpriteRenderer();
      void Init();
      void Draw( const JSprite *jsprite );
      // draws all stored objects then clears it.
      void DrawBatch( bool sort = false );
    private:
      Shader shader;
      std::vector<const JSprite *> jsprites;
  };
}

#endif /*SPRITE_H*/
