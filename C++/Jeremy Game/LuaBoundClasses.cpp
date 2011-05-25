//-------------------------------------------------------------------------------
// Name:        LuaBoundClasses.h
// Purpose:     Jeremy's lua bound classes.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------
#include "LuaBoundClasses.h"
#include "LuaManager.h"

const char JSpriteLua::className[] = "JSpriteLua";
Lunar<JSpriteLua>::RegType JSpriteLua::methods[] = {
  method(JSpriteLua, BuildTransformation),
  method(JSpriteLua, Draw),
  {0,0}
};

JSpriteLua::JSpriteLua()
{

}

JSpriteLua::JSpriteLua( lua_State *L )
{

}

JSpriteLua::~JSpriteLua()
{

}

int JSpriteLua::Init( lua_State *L )
{
  Init();

  return 0;
}

void FillVec2( lua_State *L, int idx, Vec2 &vec )
{
  //lua_pushnil(L);  /* first key */
  //lua_next(L, idx);
  //vec.x = lua_tonumber(L,-1);
  //lua_pop(L, 1);  /* removes `value'; keeps `key' for next iteration */
  //lua_next(L, idx);
  //vec.y = lua_tonumber(L,-1);
  //lua_pop(L, 1);  /* removes `value'; keeps `key' for next iteration */
  lua_rawgeti(L,idx,1);
  vec.x = static_cast<float>(lua_tonumber(L,-1));
  //lua_pop(L,1);
  lua_rawgeti(L,idx,2);
  vec.y = static_cast<float>(lua_tonumber(L,-1));
  lua_pop(L,2);
}

#include <string>
#include <iostream>
//Matrix BuildTransformation( const Vec2 &position, const Vec2 &center, const Vec2 &scale = Vec2( 1.0f, 1.0f ), float direction = 0.0f, float zlevel = 0.0f );
int JSpriteLua::BuildTransformation( lua_State *L )
{
  //StackPrint( L );

  int sizeParams = lua_gettop(L);
  if ( sizeParams < 2 )
  {
    std::cout << "BuildTransformation takes 2-4 parameters, received " << sizeParams
    << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  /////////////////////////////////////
  Vec2 position;
  Vec2 center;
  Vec2 scale( 1.0f, 1.0f );
  float direction = 0.0f;
  Matrix transformation;
  /////////////////////////////////////

  int t = 1;
  int size = lua_objlen(L,t);

  if ( size != 2 )
  {
    std::cout << "BuildTransformation parameter 1 must contain 2 members, received " << size
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  FillVec2(L,t,position);

  t += 1;
  size = lua_objlen(L,t);
  if ( size != 2 )
  {
    std::cout << "BuildTransformation parameter 2 must contain 2 members, received " << size
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  FillVec2(L,t,center);


  t += 1;
  if ( sizeParams >= 3 )
  {
    size = lua_objlen(L,t);

    if ( size != 2 )
    {
      std::cout << "BuildTransformation parameter 3 must contain 2 members, received " << size
        << ". Returning nil." << std::endl;
      lua_pushnil(L);
      return 1;
    }

    FillVec2(L,t,scale);
  }

  if ( sizeParams >= 4 )
  {
    direction = static_cast<float>(lua_tonumber(L,-1));
  }

  lua_pop(L,sizeParams);

  transformation = BuildTransformation( position, center, scale, direction );

  lua_newtable(L);
  int top = lua_gettop(L);
  for ( int i = 0; i < 16; ++i )
  {
    lua_pushnumber(L, transformation[i]);
    lua_rawseti(L, top, i+1);
  }

  //StackPrint(L);
  return 1;
}


//void Draw( unsigned texture, const Matrix &transformation, const JRect &textureCoord, float alpha = 1.0f, const Vec3 &transparentColor = Vec3( 1.0f, 0.0f, 1.0f ) );
int JSpriteLua::Draw( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams < 3 )
  {
    std::cout << "Draw takes 3-7 parameters, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  ///////////////////////////////////////
  unsigned texture;
  Matrix transformation;
  JRect textureCoord;
  float zlevel = -1.0f;
  float alpha = 1.0f;
  bool useCamera = true;
  Vec3 color( 1.0f, 0.0f, 1.0f );
  ///////////////////////////////////////

  texture = static_cast<int>(lua_tonumber(L,1));

  int t = 2;
  if ( !lua_istable(L,t) )
  {
    std::cout << "Draw parameter 2 must be a table. Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int size = lua_objlen(L,t);

  if ( size != 16 )
  {
    std::cout << "Draw parameter 2 must contain 16 members, received " << size
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  for ( int i = 0; i < 16; ++i )
  {
    lua_rawgeti(L,t,i+1);
    transformation[i] = static_cast<float>(lua_tonumber(L,-1));
    lua_pop(L,1);
  }

  t += 1;

  size = lua_objlen(L,t);

  if ( size != 4 )
  {
    std::cout << "Draw parameter 3 must contain 4 members, received " << size
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  for ( int i = 0; i < 4; ++i )
  {
    lua_rawgeti(L,t,i+1);
    textureCoord[i] = static_cast<float>(lua_tonumber(L,-1));
    lua_pop(L,1);
  }

  if ( sizeParams >= 4 )
  {
    zlevel = lua_tonumber(L,4);
  }

  if ( sizeParams >= 5 )
  {
    alpha = lua_tonumber(L,5);
  }

  if ( sizeParams >= 6 )
  {
    useCamera = lua_toboolean(L,6);
  }

  if ( sizeParams == 7 )
  {
    t = 7;

    size = lua_objlen(L,t);

    if ( size != 3 )
    {
      std::cout << "Draw parameter 7 must contain 3 members, received " << size
        << ". Returning nil." << std::endl;
      lua_pushnil(L);
      return 1;
    }

    //r
    lua_rawgeti(L,t,1);
    color.x = static_cast<float>(lua_tonumber(L,-1));
    lua_pop(L,1);
    //g
    lua_rawgeti(L,t,2);
    color.y = static_cast<float>(lua_tonumber(L,-1));
    lua_pop(L,1);
    //b
    lua_rawgeti(L,t,3);
    color.z = static_cast<float>(lua_tonumber(L,-1));
    lua_pop(L,1);
  }

  lua_pop(L,sizeParams);

  Draw( texture, transformation, textureCoord, zlevel, alpha, useCamera, color );

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Input
/////////////////////////////////////////////////////////////////////////////////

const char JInputLua::className[] = "JInputLua";
Lunar<JInputLua>::RegType JInputLua::methods[] = {
  method(JInputLua, KeyPressed),
  method(JInputLua, KeyPrevious),
  method(JInputLua, KeyTriggered),
  method(JInputLua, KeyReleased),
  method(JInputLua, MouseButtonPressed),
  method(JInputLua, MouseButtonPrevious),
  method(JInputLua, MouseButtonTriggered),
  method(JInputLua, MouseButtonReleased),
  method(JInputLua, GetAxis),
  method(JInputLua, GetCursorX),
  method(JInputLua, GetCursorY),
  method(JInputLua, JoystickActive),
  method(JInputLua, KeyBoardChanged),
  {0,0}
};

JInputLua::JInputLua()
{

}

 JInputLua::JInputLua( lua_State *L )
 {
 
 }

JInputLua::~JInputLua()
{

}

int JInputLua::KeyBoardChanged( lua_State *L )
{
  lua_pushboolean( L, KeyBoardChanged() );

  return 1;
}

int JInputLua::KeyPressed( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "KeyPressed takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int key = static_cast<int>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  bool flag = KeyPressed( key );
  lua_pushboolean( L, flag );

  return 1;
}

int JInputLua::KeyPrevious( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "KeyPrevious takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int key = static_cast<int>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  bool flag = KeyPrevious( key );
  lua_pushboolean( L, flag );

  return 1;
}

int JInputLua::KeyTriggered( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "KeyTriggered takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int key = static_cast<int>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  bool flag = KeyTriggered( key );
  lua_pushboolean( L, flag );

  return 1;
}

int JInputLua::KeyReleased( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "KeyReleased takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int key = static_cast<int>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  bool flag = KeyReleased( key );
  lua_pushboolean( L, flag );

  return 1;
}


int JInputLua::MouseButtonPressed( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "MouseButtonPressed takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int button = static_cast<int>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  bool flag = MouseButtonPressed( button );
  lua_pushboolean( L, flag );

  return 1;
}

int JInputLua::MouseButtonPrevious( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "MouseButtonPrevious takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int button = static_cast<int>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  bool flag = MouseButtonPrevious( button );
  lua_pushboolean( L, flag );

  return 1;
}

int JInputLua::MouseButtonTriggered( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "MouseButtonTriggered takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int button = static_cast<int>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  bool flag = MouseButtonTriggered( button );
  lua_pushboolean( L, flag );

  return 1;
}

int JInputLua::MouseButtonReleased( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "MouseButtonReleased takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int button = static_cast<int>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  bool flag = MouseButtonReleased( button );
  lua_pushboolean( L, flag );

  return 1;
}

int JInputLua::GetAxis( lua_State *L )
{
  //StackPrint(L);

  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "JInputLua::GetAxis takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  int axis = static_cast<float>(lua_tonumber(L,1));
  lua_pop( L, 1 );

  float value = GetAxis( axis );
  lua_pushnumber( L, value );

  return 1;
}

int JInputLua::GetCursorX( lua_State *L )
{
  lua_pushnumber( L, GetCursorX() );

  return 1;
}

int JInputLua::GetCursorY( lua_State *L )
{
  lua_pushnumber( L, GetCursorY() );

  return 1;
}

int JInputLua::JoystickActive( lua_State *L )
{
  lua_pushboolean( L, JoystickActive() );

  return 1;
}

/////////////////////////////////////////////////////////////////////////////////
// Jimage
/////////////////////////////////////////////////////////////////////////////////

const char JImageManagerLua::className[] = "JImageManagerLua";
Lunar<JImageManagerLua>::RegType JImageManagerLua::methods[] = {
  method(JImageManagerLua, Create),
  method(JImageManagerLua, Reset),
  method(JImageManagerLua, Delete),
  method(JImageManagerLua, DeleteAll),
  {0,0}
};

JImageManagerLua::JImageManagerLua()
{

}

JImageManagerLua::JImageManagerLua( lua_State *L )
{

}

JImageManagerLua::~JImageManagerLua()
{

}

int JImageManagerLua::Create( lua_State *L )
{
  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "JImageManagerLua::Create takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  const char *file = lua_tostring(L,1);
  lua_pop(L, 1);

  unsigned image = Create(file);
  
  lua_pushnumber(L, image);

  return 1;
}

//reloads specified texture. If 0 reloads all
int JImageManagerLua::Reset( lua_State *L )
{
  int sizeParams = lua_gettop(L);
  if ( sizeParams > 1 )
  {
    std::cout << "JImageManagerLua::Reset takes 0-1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  unsigned image = 0;

  if ( sizeParams == 1 )
  {
    image = lua_tonumber(L,1);
    lua_pop(L, 1);
  }

  Reset(image);

  return 0;
}

int JImageManagerLua::Delete( lua_State *L )
{
  int sizeParams = lua_gettop(L);
  if ( sizeParams != 1 )
  {
    std::cout << "JImageManagerLua::Delete takes 1 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  unsigned image = lua_tonumber(L,1);
  lua_pop(L, 1);

  Delete(image);

  return 0;
}

int JImageManagerLua::DeleteAll( lua_State *L )
{
  int sizeParams = lua_gettop(L);
  if ( sizeParams != 0 )
  {
    std::cout << "JImageManagerLua::DeleteAll takes 0 parameter, received " << sizeParams
      << ". Returning nil." << std::endl;
    lua_pushnil(L);
    return 1;
  }

  DeleteAll();

  return 0;
}