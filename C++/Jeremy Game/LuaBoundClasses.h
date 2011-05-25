//-------------------------------------------------------------------------------
// Name:        LuaBoundClasses.h
// Purpose:     Jeremy's lua bound classes.
//
// Author:      Jeremy Anderson
//
// Created:     Monday, March 4, 2011
// Copyright:   (c) Jeremy Anderson 2011
//-------------------------------------------------------------------------------
#ifndef LUABOUNDCLASSES_H
#define LUABOUNDCLASSES_H

#include "JSprite.h"
#include "Input.h"
#include "JImage.h"
#include "LuaJIT/Lunar.h"

class JSpriteLua: public JSprite
{
  public:
    JSpriteLua();
    JSpriteLua( lua_State *L );
    virtual ~JSpriteLua();
    using JSprite::Init;
    int Init( lua_State *L );
    //Matrix BuildTransformation( const Vec2 &position, const Vec2 &center, const Vec2 &scale = Vec2( 1.0f, 1.0f ), float direction = 0.0f, float zlevel = 0.0f );
    using JSprite::BuildTransformation;
    int BuildTransformation( lua_State *L );
    //void Draw( unsigned texture, const Matrix &transformation, const JRect &textureCoord, float alpha = 1.0f, const Vec3 &transparentColor = Vec3( 1.0f, 0.0f, 1.0f ) );
    using JSprite::Draw;
    int Draw( lua_State *L );

    static const char className[];
    static Lunar<JSpriteLua>::RegType methods[];
  private:
};

/////////////////////////////////////////////////////////////////////////////////
// Input
/////////////////////////////////////////////////////////////////////////////////

class JInputLua: public Input
{
  public:
    JInputLua();
    JInputLua( lua_State *L );
    virtual ~JInputLua();

    Input::KeyBoardChanged;
    int KeyBoardChanged( lua_State *L );

    using Input::KeyPressed;
    int KeyPressed( lua_State *L );
    using Input::KeyPrevious;
    int KeyPrevious( lua_State *L );
    using Input::KeyTriggered;
    int KeyTriggered( lua_State *L );
    using Input::KeyReleased;
    int KeyReleased( lua_State *L );

    using Input::MouseButtonPressed;
    int MouseButtonPressed( lua_State *L );
    using Input::MouseButtonPrevious;
    int MouseButtonPrevious( lua_State *L );
    using Input::MouseButtonTriggered;
    int MouseButtonTriggered( lua_State *L );
    using Input::MouseButtonReleased;
    int MouseButtonReleased( lua_State *L );

    using Input::GetAxis;
    int GetAxis( lua_State *L );

    using Input::GetCursorX;
    int GetCursorX( lua_State *L );
    using Input::GetCursorY;
    int GetCursorY( lua_State *L );

    using Input::JoystickActive;
    int JoystickActive( lua_State *L );

    static const char className[];
    static Lunar<JInputLua>::RegType methods[];
};

/////////////////////////////////////////////////////////////////////////////////
// Jimage
/////////////////////////////////////////////////////////////////////////////////
class JImageManagerLua : public JImageManager
{
  public:
    JImageManagerLua();
    JImageManagerLua( lua_State *L );
    virtual ~JImageManagerLua();

    using JImageManager::Create;
    int Create( lua_State *L );
    //reloads specified texture. If 0 reloads all
    using JImageManager::Reset;
    int Reset( lua_State *L );
    using JImageManager::Delete;
    int Delete( lua_State *L );
    using JImageManager::DeleteAll;
    int DeleteAll( lua_State *L );

    static const char className[];
    static Lunar<JImageManagerLua>::RegType methods[];
};

#endif /*LUABOUNDCLASSES_H*/