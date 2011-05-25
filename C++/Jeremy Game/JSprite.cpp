//-------------------------------------------------------------------------------
// Name:        JSprite.cpp
// Purpose:     Jeremy's sprite drawing made easy.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------

#include "GLee.h"
#include "SDL/SDL_opengl.h"
#include "SOIL/SOIL.h"
#include "JSprite.h"
#include "Vec4.h"

JRect::JRect( float top /* = 0.0f */, float bottom /* = 0.0f */, float left /* = 0.0f */, float right /* = 0.0f */ )
{
  this->top = top;
  this->bottom = bottom;
  this->left = left;
  this->right = right;
}

float &JRect::operator []( const int Index )
{
  switch(Index)
  {
  case 0:
    return top;
  case 1:
    return bottom;
  case 2:
    return left;
  default:
    return right;
  }
}

const float &JRect::operator []( const int Index ) const
{
  switch(Index)
  {
  case 0:
    return top;
  case 1:
    return bottom;
  case 2:
    return left;
  default:
    return right;
  }
}

JSprite::JSprite()
{

}

JSprite::~JSprite()
{

}

void JSprite::Init()
{
  shader.AddFragmentShader( "resources/shaders/frame.fs" );
  shader.AddVertexShader( "resources/shaders/frame.vs" );

  shader.LinkProgram();
}

void JSprite::Begin()
{
}

void JSprite::End()
{
}

Matrix JSprite::BuildTransformation( const Vec2 &position, const Vec2 &center, const Vec2 &scale /* = Vec2( 1.0f, 1.0f ) */, float direction /* = 0.0 */ )
{
  Matrix rot = RotateZ( direction );
  Matrix scl = Scale( scale.x, scale.y );
  Matrix pos = Translate( position.x, position.y );
  Matrix ctr = Translate( -center.x, -center.y );

  Matrix transform = pos * rot * scl * ctr;
  return transform;
}

void PutInPlace( std::vector<JSprite::JDrawObject> &objs, JSprite::JDrawObject &obj )
{
  for ( std::vector<JSprite::JDrawObject>::iterator it = objs.begin(), itEnd = objs.end(); it != itEnd; ++it )
  {
    if ( obj.zlevel <= it->zlevel )
      objs.insert( it, obj );
    return;
  }
  objs.push_back(obj);
}

void JSprite::Draw( unsigned texture, const Matrix &transformation, const JRect &textureCoord, float zlevel /* = 0.0 */, float alpha /* = 1.0 */, bool useCamera, const Vec3 &color /* = Vec3 */ )
{
  JDrawObject temp;
  temp.texture = texture;
  temp.transformation = transformation;
  temp.textureCoord = textureCoord;
  temp.zlevel = zlevel;
  temp.alpha = alpha;
  temp.useCamera = useCamera;
  temp.color = color;

  objects.push_back(temp);
  //PutInPlace( objects, temp );

  //glBindTexture( GL_TEXTURE_2D, texture );
  //int mem = 0;
  //shader.uniform<1>( "Texture", &mem );

  //shader.uniform<1>( "alpha", &(alpha) );
  //float tc[3] = { transparentColor.x, transparentColor.y, transparentColor.z };
  //shader.uniform<3>( "TransparentColor", tc );

  //glPushMatrix();
  //glMultMatrixf( transformation.Transpose() );
  //glBegin( GL_QUADS );
  ////(texcoord.right, texcoord.top), (texcoord.left, texcoord.top), (texcoord.left, texcoord.bottom), (texcoord.right, texcoord.bottom)
  //glTexCoord2f		( textureCoord.left, textureCoord.bottom );	glVertex2f( 0.0f, 0.0f );
  //glTexCoord2f		( textureCoord.right, textureCoord.bottom );	glVertex2f( 1.0f, 0.0f );
  //glTexCoord2f		( textureCoord.right, textureCoord.top );	glVertex2f( 1.0f, 1.0f );
  //glTexCoord2f		( textureCoord.left, textureCoord.top);	glVertex2f( 0.0f, 1.0f );
  //glEnd();
  //glPopMatrix();
}

#include <algorithm>

void InsertionSort( std::vector<JSprite::JDrawObject> &A )
{
  int size = A.size();
  for ( int i = 1; i < size; ++i )
  {
    JSprite::JDrawObject value = A[i];
    int j;

    for ( j = i - 1; j >= 0 && A[j] > value; --j )
      A[ j + 1 ] = A[j];

    A[ j + 1 ] = value;
  }
}

void JSprite::DrawBatch()
{
  shader.BindProgram();
  glActiveTexture( GL_TEXTURE0 );

  //std::sort( objects.begin(), objects.end() );
  InsertionSort(objects);

  int size = objects.size();
  for ( int i = 0; i < size; ++i )
  {
    glBindTexture( GL_TEXTURE_2D, objects[i].texture );
    shader.uniform( "Texture", 0 );

    shader.uniform( "alpha", objects[i].alpha );
    float tc[3] = { objects[i].color.x, objects[i].color.y, objects[i].color.z };
    shader.uniform<3>( "Color", tc );

    glPushMatrix();
    if ( !objects[i].useCamera )
      glLoadIdentity();

    glMultMatrixf( objects[i].transformation.Transpose() );
    glBegin( GL_QUADS );
    //(texcoord.right, texcoord.top), (texcoord.left, texcoord.top), (texcoord.left, texcoord.bottom), (texcoord.right, texcoord.bottom)
    glTexCoord2f( objects[i].textureCoord.left, objects[i].textureCoord.top );	glVertex2f( 0.0f, 0.0f );
    glTexCoord2f( objects[i].textureCoord.right, objects[i].textureCoord.top );	glVertex2f( 1.0f, 0.0f );
    glTexCoord2f( objects[i].textureCoord.right, objects[i].textureCoord.bottom );	glVertex2f( 1.0f, 1.0f );
    glTexCoord2f( objects[i].textureCoord.left, objects[i].textureCoord.bottom);	glVertex2f( 0.0f, 1.0f );
    glEnd();
    glPopMatrix();
  }

  objects.clear();


  glBindTexture( GL_TEXTURE_2D, 0 );
  shader.UnBindProgram();

}
