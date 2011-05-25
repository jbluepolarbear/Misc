//-------------------------------------------------------------------------------
// Name:        JImage.h
// Purpose:     Jeremy's image loading and handling made easy.
//
// Author:      Jeremy Anderson

//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------
#include <string>
#include <map>

#ifndef JIMAGE_H
#define JIMAGE_H

class JImageManager
{
  public:
    JImageManager();
    ~JImageManager();
    unsigned Create( const std::string &image );
    //reloads specified texture. If 0 reloads all
    void Reset( unsigned texture = 0 );
    void Delete( unsigned texture );
    void DeleteAll();
  private:
    struct JImage
    {
      JImage();
      unsigned texture;
      unsigned numberOfObjectsRefernced;
    };
    typedef JImage * JImagePtr;

    void ResetTexture( unsigned texture );
    void ResetAllTexture();
    unsigned CreateTexture( const std::string &image );
    unsigned LoadTexture( const std::string &image, unsigned reuse = 0 );
    int totalNumberOfTextures;
    std::map< std::string, JImagePtr > textureMap;
};

#endif //JIMAGE_h
