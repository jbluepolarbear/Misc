//-------------------------------------------------------------------------------
// Name:        Shader.cpp
// Purpose:     Jeremy's shaders made easy.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------

#include "GLee.h"
#include "SDL/SDL_opengl.h"
#include "Shader.h"
#include <iostream>
#include <map>
#include <fstream>

std::map< std::string, int > vertexshaders;
std::map< std::string, int > fragmentshaders;


Shader::Shader( const std::string &vertFilename, const std::string &fragFilename )
{
  AddVertexShader( vertFilename );
  AddFragmentShader( fragFilename );

  LinkProgram();
}

void Shader::ReleaseShaders()
{
  for ( std::map< std::string, int >::iterator it = vertexshaders.begin(), it2 = vertexshaders.end(); it != it2; ++it )
  {
    if ( it->second )
      glDeleteShader( it->second );
  }

  for ( std::map< std::string, int >::iterator it = fragmentshaders.begin(), it2 = fragmentshaders.end(); it != it2; ++it )
  {
    if ( it->second )
      glDeleteShader( it->second );
  }
}

void LoadFile( const std::string &filename, std::string &destination )
{
  std::fstream filestr;
  filestr.open( filename.c_str(), std::fstream::in );
  if ( filestr.is_open() )
  {
    filestr.seekg( 0, std::fstream::end );
    int count = filestr.tellg();
    filestr.seekg( 0, std::fstream::beg );
    char *str = new char[count];
    if ( str )
    {
      filestr.read( str, count );
      count = filestr.gcount();
      destination = std::string( str, count );
      destination += '\0';
      delete str;
    }
    filestr.close();
  }
}

void PrintIfError( GLhandleARB obj, const std::string &type )
{
  int flag;

  glGetObjectParameterivARB( obj, GL_COMPILE_STATUS, &flag );

  if ( flag )
    return;

  int blen = 0;	
  int slen = 0;

  glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &blen );       

  if ( blen > 1 )
  {
    char *compiler_log = new char[blen];

    glGetInfoLogARB( obj, blen, &slen, compiler_log );
    std::cout << "compiler_log: " << type << "\n" << compiler_log << "\n" << std::endl;
    delete [] compiler_log;
  }
}

int LoadShader( const std::string &filename, ShaderType type )
{
  std::string sfile;
  std::string stype;
  int s;

  if ( type == FRAGMENT )
  {
    stype = "Fragment";
    s = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );
  }
  else if ( type == VERTEX )
  {
    stype = "Vertex";
    s = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );
  }
  else
    return 0;

  LoadFile( filename.c_str(), sfile );

  const char *sf = sfile.c_str();

  glShaderSourceARB( s, 1, &sf, NULL );

  glCompileShaderARB( s );

  PrintIfError( s, stype );

  return s;
}

int GetVertexShader( const std::string &filename )
{
  //return LoadShader( filename, VERTEX );
  //return 0;
  std::map< std::string, int >::iterator it;

  it = vertexshaders.find( filename );
  if ( it == vertexshaders.end() )
  {
    int s = LoadShader( filename, VERTEX );
    vertexshaders[filename] = s;
    return s;
  }

  return it->second;
}

int GetFragmentShader( const std::string &filename )
{
  //return LoadShader( filename, FRAGMENT );
  //return 0;
  std::map< std::string, int >::iterator it;

  it = fragmentshaders.find( filename );
  if ( it == fragmentshaders.end() )
  {
    int s = LoadShader( filename, FRAGMENT );
    fragmentshaders[filename] = s;
    return s;
  }

  return it->second;
}

Shader::Shader()
{
  p = 0;
}

Shader::~Shader()
{
  //if ( p )
  //  glDeleteProgramsARB( 1, &p );
}

void Shader::AddVertexShader( const std::string &filename )
{
  vertexes.push_back( filename );
}

void Shader::AddFragmentShader( const std::string &filename )
{
  fragments.push_back( filename );
}

void Shader::LinkProgram()
{
  p = glCreateProgramObjectARB();

  for ( std::vector< std::string >::iterator it = vertexes.begin(), it2 = vertexes.end(); it != it2; ++it )
  {
    int s = GetVertexShader( *it );
    glAttachObjectARB( p, s );
  }

  for ( std::vector< std::string >::iterator it = fragments.begin(), it2 = fragments.end(); it != it2; ++it )
  {
    int s = GetFragmentShader( *it );
    glAttachObjectARB( p, s );
  }

  glLinkProgramARB( p ); 

  BindProgram();
}

template <>
void Shader::uniform<1>( const std::string &name, int *val )
{
  int location = glGetUniformLocation( p, name.c_str() );
  glUniform1iv( location, 1, val );
}

void Shader::uniform( const std::string &name, int val )
{
  int location = glGetUniformLocation( p, name.c_str() );
  glUniform1iv( location, 1, &val );
}

void Shader::uniform( const std::string &name, float val )
{
  int location = glGetUniformLocation( p, name.c_str() );
  glUniform1fv( location, 1, &val );
}

template <>
void Shader::uniform<1>( const std::string &name, float *val )
{
  int location = glGetUniformLocation( p, name.c_str() );
  glUniform1fv( location, 1, val );
}

template <>
void Shader::uniform<2>( const std::string &name, float *val )
{
  int location = glGetUniformLocation( p, name.c_str() );
  glUniform2fv( location, 1, val );
}

template <>
void Shader::uniform<3>( const std::string &name, float *val )
{
  int location = glGetUniformLocation( p, name.c_str() );
  glUniform3fv( location, 1, val );
}

template <>
void Shader::uniform<4>( const std::string &name, float *val )
{
  int location = glGetUniformLocation( p, name.c_str() );
  glUniform4fv( location, 1, val );
}

void Shader::uniformMatrix( const std::string &name, bool transpose, float *val )
{
  int location = glGetUniformLocation( p, name.c_str() );
  glUniformMatrix4fv( location, 1, transpose, val );
}

int Shader::VertexAttribute( const std::string &name )
{
  int location = glGetAttribLocation( p, name.c_str() );
  glEnableVertexAttribArray( location );
  return location;
}

void Shader::BindProgram()
{
  glUseProgramObjectARB( p );
}

void Shader::UnBindProgram()
{
  glUseProgramObjectARB( 0 );
}