#define _CRT_SECURE_NO_WARNINGS

#include "TimeClass.h"
///////////////////////////////////
//#include <vld.h>
#include "GLee.h"
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "Shader.h"
#include "Input.h"
#include "SOIL/SOIL.h"

#include "JImage.h"
#include "Sprite.h"
#include "Camera.h"
///////////////////////////////////

#pragma comment( lib, "SDL/lib/SDL.lib" )
#pragma comment( lib, "SDL/lib/SDLmain.lib" )

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "SOIL/SOIL.lib" )

//The attributes of the screen
const int SCREEN_BPP = 32;
////////////////////////////////

#include <cstdlib>

#include "LuaManager.h"
//
//#include "LuaBoundClasses.h"

#include <iostream>

void EnableMemoryLeakChecking(int breakAlloc=-1)
{
  //Set the leak checking flag
  int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
  _CrtSetDbgFlag(tmpDbgFlag);

  //If a valid break alloc provided set the breakAlloc
  if(breakAlloc!=-1) _CrtSetBreakAlloc( breakAlloc );
}

void SetConfig( lua_State *L, int &width, int &height, std::string &windowName, bool &fullscreen )
{
  lua_getglobal( L, "screenWidth" );
  if ( lua_isnil( L, 1 ) != 1 )
    width = lua_tonumber( L, 1 );
  lua_pop( L, 1 );

  lua_getglobal( L, "screenHeight" );
  if ( lua_isnil( L, 1 ) != 1 )
    height = lua_tonumber( L, 1 );
  lua_pop( L, 1 );

  lua_getglobal( L, "windowName" );
  if ( lua_isnil( L, 1 ) != 1 )
    windowName = lua_tostring( L, 1 );
  lua_pop( L, 1 );

  lua_getglobal( L, "fullscreen" );
  if ( lua_isnil( L, 1 ) != 1 )
    fullscreen = lua_toboolean( L, 1 );
  lua_pop( L, 1 );
}

int main( int argc, char *argv[] )
{
  EnableMemoryLeakChecking();
  srand(Time::Clock());

  int width = 800;
  int height = 600;
  std::string windowName = "Game Title";
  bool fullscreen = false;
  unsigned windowFlags = SDL_OPENGL;


  ////////////////////////////////////////////////////////////////////
  /////// Lua Stuff
  ////////////////////////////////////////////////////////////////////
  LuaManager lua;
  lua.Init();
  lua.RunFile( "resources/scripts/Config.lua" );

  SetConfig( lua.GetState(), width, height, windowName, fullscreen );
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  Input input;
  JImageManager image;

  input.Init();

  if ( fullscreen )
    windowFlags |= SDL_FULLSCREEN;

  SDL_Init( SDL_INIT_EVERYTHING );
  SDL_SetVideoMode( width, height, SCREEN_BPP, windowFlags );
  SDL_WM_SetCaption( windowName.c_str(), windowName.c_str() );
  //SDL_ShowCursor(0);
  ////////////////////////////////
  /////////////
  //glEnable( GL_ALPHA );

  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glViewport( 0, 0, width, height );
  //gluPerspective( 41.95f, float(SCREEN_WIDTH)/SCREEN_HEIGHT, 0.1f, 1000.0f );
  gluOrtho2D( 0, width, height, 0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  /////////////

  glDisable( GL_CULL_FACE );

  bool running = true;
  Time::Timer timer;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  //FILE *file = fopen("stderr.txt", "w");

  unsigned shipimage = image.Create( "resources/textures/blue_circle.png" );


  JeremySprite::JSpriteRenderer jrenderer;
  jrenderer.Init();

  JeremySprite::JSprite ship(shipimage);
  ship.SetPosition( Vec2( width/2.0f, height/2.0f ) );
  ship.SetCenter( 0.5f, 0.5f );
  ship.SetScale( 64.0f, 64.0f );
  //ship.SetUseCamera( false );

  bool pause = false;
  Camera cam;
  cam.SetScreenSize( width, height );
  cam.SetPosition( Vec2( width/2.0f, height/2.0f ) );
  while ( running )
  {
    timer.Start();
    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {
      if ( event.type == SDL_QUIT )
      {
        running = false;
      }
    }

    input.Update();

    if ( input.KeyPressed( SDLK_ESCAPE ) )
      running = false;

    //if ( input.KeyPressed( SDLK_LEFT ) )
    //  cam.SetRotate( cam.GetRotate() - 1.0f );

    //if ( input.KeyPressed( SDLK_RIGHT ) )
    //  cam.SetRotate( cam.GetRotate() + 1.0f );

    //if ( input.KeyPressed( SDLK_UP ) )
    //  cam.SetDistance( cam.GetDistance() + 0.1f );

    //if ( input.KeyPressed( SDLK_DOWN ) )
    //  cam.SetDistance( cam.GetDistance() - 0.1f );

    glClear( GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    //glTranslatef( SCREEN_WIDTH/2.0 - camx, SCREEN_HEIGHT/2.0 - camy, 0.0 );
    cam.SetView();

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    jrenderer.Draw(&ship);

    jrenderer.DrawBatch();

    SDL_GL_SwapBuffers();
    timer.Stop();
  }

  input.Exit();
  SDL_Quit();

  return 0;
}