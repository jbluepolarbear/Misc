//-------------------------------------------------------------------------------
// Name:        LuaBoundClasses.h
// Purpose:     Jeremy's lua bound classes.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------
#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include "LuaJIT/Lunar.h"
#pragma comment( lib, "LuaJIT/lua51.lib" )

#include <string>

//int Print( lua_State *L )
//{
//  int argc = lua_gettop( L );
//
//  std::string str;
//
//  for ( int n = 1 ; n <= argc; ++n )
//  {
//    int type = lua_type( L, n );
//    str += lua_tostring( L, n );
//    str += "\t";
//  }
//
//  puts( str.c_str() );
//
//  return 0;
//}

int LuaErrorFunc( lua_State *L );

void StackPrint( lua_State *L );

//#include "Utilities.h"

class LuaManager
{
  public:
    LuaManager();

    virtual ~LuaManager();

    void Init();

    void RunFile( const std::string &filename );

    void Update( double time, double totalTime );

    lua_State *GetState();

  private:
    lua_State *luaState;
};

#endif /*LUABOUNDCLASSES_H*/
