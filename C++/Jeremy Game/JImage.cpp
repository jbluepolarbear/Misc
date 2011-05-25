//-------------------------------------------------------------------------------
// Name:        JImage.cpp
// Purpose:     Jeremy's image loading and handling made easy.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------
#include "JImage.h"
#include "SDL/SDL_opengl.h"
#include "SOIL/SOIL.h"
#include <iostream>

JImageManager::JImage::JImage()
{
  texture = 0;
  numberOfObjectsRefernced = 0;
}

JImageManager::JImageManager()
{
  totalNumberOfTextures = 0;
  textureMap.clear();
}

JImageManager::~JImageManager()
{
  DeleteAll();
}

unsigned JImageManager::Create( const std::string &image )
{
  std::map< std::string, JImagePtr >::iterator it;
  it = textureMap.find( image );

  if ( it != textureMap.end() )
  {
    (*it).second->numberOfObjectsRefernced++;
    return (*it).second->texture;
  }

  return CreateTexture( image );
}

void JImageManager::Reset( unsigned texture /* = 0 */ )
{
  if ( texture )
    ResetTexture( texture );
  else
    ResetAllTexture();
}

void JImageManager::Delete( unsigned texture )
{
  std::map< std::string, JImagePtr >::iterator it;

  if ( !totalNumberOfTextures )
    return;

  for ( it = textureMap.begin(); it != textureMap.end(); ++it )
  {
    if ( (*it).second->texture == texture )
    {
      if ( --(*it).second->numberOfObjectsRefernced == 0 )
      {
        delete (*it).second;
        textureMap.erase( it );
        glDeleteTextures( 1, &texture );
        --totalNumberOfTextures;
      }
      return;
    }
  }
}

void JImageManager::DeleteAll()
{
  std::map< std::string, JImagePtr >::iterator it, it2;

  for ( it = textureMap.begin(), it2 = textureMap.end(); it != it2; ++it )
  {
    glDeleteTextures( 1, &(*it).second->texture );
    (*it).second->numberOfObjectsRefernced = 0;
    delete (*it).second;
    --totalNumberOfTextures;
  }

  textureMap.clear();
}

unsigned JImageManager::CreateTexture( const std::string &image )
{
  JImagePtr newTexture = new JImage;
  newTexture->texture = LoadTexture( image );
  if ( newTexture->texture == 0 )
  {
    //MessageBox( NULL, "Failed to load Texture", "Failed", MB_OK );
    std::cout << "Failed to load Texture: " << image << std::endl;
    delete newTexture;
    return 0;
  }
  ++totalNumberOfTextures;
  newTexture->numberOfObjectsRefernced++;
  textureMap[image] = newTexture;
  return newTexture->texture;
}

void JImageManager::ResetTexture( unsigned texture )
{
  std::map< std::string, JImagePtr >::iterator it;

  if ( !totalNumberOfTextures )
    return;

  for ( it = textureMap.begin(); it != textureMap.end(); ++it )
  {
    if ( (*it).second->texture == texture )
    {
      glDeleteTextures( 1, &texture );
      texture = LoadTexture( it->first, texture );
      return;
    }
  }
}

void JImageManager::ResetAllTexture()
{
  std::map< std::string, JImagePtr >::iterator it, it2;

  for ( it = textureMap.begin(), it2 = textureMap.end(); it != it2; ++it )
  {
    glDeleteTextures( 1, &(*it).second->texture );
    (*it).second->texture = LoadTexture( it->first, (*it).second->texture );
  }
}

unsigned JImageManager::LoadTexture( const std::string &image, unsigned reuse /*= 0*/ )
{
  return SOIL_load_OGL_texture(
    image.c_str(),
    SOIL_LOAD_AUTO,
    reuse,
    SOIL_FLAG_TEXTURE_REPEATS );
}
