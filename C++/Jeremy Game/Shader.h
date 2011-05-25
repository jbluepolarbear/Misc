//-------------------------------------------------------------------------------
// Name:        Shader.h
// Purpose:     Jeremy's shaders made easy.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------

#pragma once
#include <string>
#include <vector>

enum ShaderType
{
  FRAGMENT = 1,
  VERTEX
};

class Shader
{
  public:
    Shader();
    Shader( const std::string &vertFilename, const std::string &fragFilename );
    ~Shader();

    void AddVertexShader( const std::string &filename );

    void AddFragmentShader( const std::string &filename );

    void LinkProgram();

    void uniform( const std::string &name, int val );
    void uniform( const std::string &name, float val );
    template <int num>
    void uniform( const std::string &name, int *val );
    template <int num>
    void uniform( const std::string &name, float *val );

    void uniformMatrix( const std::string &name, bool transpose, float *val );

    int VertexAttribute( const std::string &name );

    void BindProgram();

    void UnBindProgram();

    static void ReleaseShaders();

    int p;
  private:
    std::vector< std::string > vertexes;
    std::vector< std::string > fragments;
};