/* Start Header -------------------------------------------------------
File Name: FrameWork.cpp
Purpose: Manage Windowing and Point drawing line Drawing and other stuff.
Language: C++, Visual Studio 2008
Platform: Visual Studio 9, Windows
Author: Jeremy Anderson
- End Header --------------------------------------------------------*/
#include "FrameWork.h"

#pragma comment( lib, "SDL/lib/SDL.lib" )
#pragma comment( lib, "SDL/lib/SDLmain.lib" )

#pragma comment( lib, "AntTweakBar.lib" )

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

extern const int MAX_DIMENSION;

FrameWork::FrameWork()
{
  width = 800;
  height = 600;
  bpp = 32;

  quit = false;

  pointsize = 6.0;
  linewidth = 2.0;

  pointcolor[0] = 1.0f;
  pointcolor[1] = 0.0f;
  pointcolor[2] = 0.0f;

  linecolor[0] = 0.0f;
  linecolor[1] = 0.0f;
  linecolor[2] = 0.0f;

  clearpoints = false;

  flags = SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE;

  pointgrabbed = false;
  pointheld = NULL;

  drawlines = true;

  currentmode = 0;

  tvalue = 0.5f;

  step = 0.005f;

  drawshell = false;

  drawfunc = NULL;
}

void FrameWork::Init()
{
  SDL_Init( SDL_INIT_EVERYTHING );

  SDL_SetVideoMode( width, height, 32, flags );

  SDL_WM_SetCaption( "FrameWorkMAT", "FrameWorkMAT" );

  SDL_EnableUNICODE( 1 );
  SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL );

  InitGL();
  InitTWB();

  // Set OpenGL camera
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0, width, 0, height, -1.0, 1.0 );
  glMatrixMode( GL_MODELVIEW );
}

void FrameWork::Exit()
{
  ClearPoints();

  // Terminate AntTweakBar
  TwTerminate();

  // Terminate SDL
  SDL_Quit();
}

void FrameWork::Update()
{
  SDL_Event event;
  int handled;

  if ( clearpoints )
  {
    ClearPoints();
  }

  // Process incoming events
  while( SDL_PollEvent( &event ) ) 
  {
    // Send event to AntTweakBar
    handled = TwEventSDL( &event );

    // If event has not been handled by AntTweakBar, process it
    if( !handled )
    {
      switch( event.type )
      {
        case SDL_QUIT:  // Window is closed
          quit = 1;
          break;

        case SDL_MOUSEBUTTONDOWN:
          if ( SDL_BUTTON( event.button.button ) == SDL_BUTTON_LEFT )
          {
            bool found = false;
            Point mouse;
            mouse.x = event.motion.x;
            mouse.y = height - event.motion.y;

            for ( unsigned i = 0; i < points.size(); ++i )
            {
              if ( Collision( mouse, points[i] ) )
              {
                pointgrabbed = true;
                pointheld = &points[i];
                break;
              }
            }

            if ( !pointgrabbed )
            {
              if ( points.size() == MAX_DIMENSION )
                points = std::vector< FrameWork::Point >( points.begin() + 1, points.end() );
              points.push_back( mouse );
              pointgrabbed = true;
              pointheld = &points.back();
            }
          }
          break;

        case SDL_MOUSEBUTTONUP:
          if ( SDL_BUTTON( event.button.button ) == SDL_BUTTON_LEFT )
          {
            pointgrabbed = false;
            pointheld = NULL;
          }
          break;

        case SDL_MOUSEMOTION:
          if ( pointgrabbed )
          {
            if ( pointheld )
            {
              pointheld->x = event.motion.x;
              pointheld->y = height - event.motion.y;
            }
          }
          break;

        case SDL_VIDEORESIZE:   // Window size has changed
          // Resize SDL video mode
          width = event.resize.w;
          height = event.resize.h;
          if( !SDL_SetVideoMode( width, height, bpp, flags ) )
            fprintf( stderr, "WARNING: Video mode set failed: %s", SDL_GetError() );

          // Resize OpenGL viewport
          glViewport( 0, 0, width, height );

          // Restore OpenGL states (SDL seems to lost them)
          glEnable( GL_DEPTH_TEST );
          glDisable( GL_CULL_FACE );
          //glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

          glMatrixMode( GL_PROJECTION );
          glLoadIdentity();
          glOrtho( 0, width, 0, height, -1.0, 1.0 );
          glMatrixMode( GL_MODELVIEW );

          break;

        case SDL_KEYDOWN:
          if( event.key.keysym.sym == ' ' ) // toggle fullscreen if Space key is pressed
          {
            flags ^= SDL_FULLSCREEN;
            SDL_SetVideoMode( 800, 600, bpp, flags );

            // Push a resize event because SDL do not do it for us
            event.type = SDL_VIDEORESIZE;
            event.resize.w = 800;
            event.resize.h = 600;
            SDL_PushEvent( &event );
          }
          break;
      }
    }
  }
}

bool FrameWork::Running()
{
  return !quit;
}

bool FrameWork::Collision( Point &mouse, Point &point )
{
  int halfsize = int( 2.0f * pointsize );//int( pointsize / 2.0f + 0.5f );

  if ( mouse.x > point.x - halfsize && mouse.x < point.x + halfsize )
    if ( mouse.y > point.y - halfsize && mouse.y < point.y + halfsize )
      return true;
  return false;
}

void FrameWork::InitGL()
{
  // Set OpenGL viewport and states
  glViewport( 0, 0, width, height );
  glEnable( GL_DEPTH_TEST );
  glDisable( GL_CULL_FACE );
}

void FrameWork::InitTWB()
{
  // Initialize AntTweakBar
  TwInit( TW_OPENGL, NULL );

  // Tell the window size to AntTweakBar
  TwWindowSize( width, height );

  // Create a tweak bar
  bar = TwNewBar( "TweakBar" );
  TwDefine( " GLOBAL help='This is MAT 300 FrameWork.\nPress [Space] to toggle fullscreen.' " ); // Message added to the help bar.

  TwAddVarRO( bar, "MaxPoints", TW_TYPE_INT32, &MAX_DIMENSION, 
    " label='MaxPoints' help='The maximum amount of points.' ");

  TwAddVarRW( bar, "step", TW_TYPE_FLOAT, &step, 
    " label='step' min=0.005 max=0.25 step=0.005 help='steps for NLI and BB.' " );

  TwAddVarRW( bar, "t", TW_TYPE_FLOAT, &tvalue, 
    " label='t' min=0.0 max=1.0 step=0.005 help='MidPoint t value.' " );

  TwAddVarRW( bar, "Draw Shell", TW_TYPE_BOOL32, &drawshell, 
    " label='Draw Shell?' true='T' false='F' help='Draw Shell for the MidPoint.' ");

  TwAddVarRW( bar, "Draw Lines", TW_TYPE_BOOL32, &drawlines, 
    " label='Draw Lines?' true='T' false='F' help='Draw Lines.' ");

  TwAddVarRW( bar, "Clear Points", TW_TYPE_BOOL32, &clearpoints, 
    " label='Clear Points?' true='T' false='F' key='c' help='Clears Points.' ");

  TwAddVarRW( bar, "Quit", TW_TYPE_BOOL32, &quit, 
    " label='Quit?' true='T' false='F' key='ESC' help='Quit program.' ");

}

void FrameWork::ClearPoints()
{
  clearpoints = false;
  points.clear();
}

void FrameWork::Draw()
{
  // Clear screen
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Set OpenGL camera
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();



  glPointSize( pointsize );
  glBegin( GL_POINTS );
  glColor3f( pointcolor[0], pointcolor[1], pointcolor[2] );
  for ( unsigned i = 0; i < points.size(); ++i )
  {
    glVertex2f( points[i].x, points[i].y );
  }
  glEnd();

  if ( drawlines )
  {
    if ( points.size() > 1 )
    {
      glLineWidth( linewidth );
      glBegin( GL_LINES );
      glColor3f( linecolor[0], linecolor[1], linecolor[2] );
      for ( unsigned i = 1; i < points.size(); ++i )
      {
        glVertex2f( points[ i - 1 ].x, points[ i - 1 ].y );
        glVertex2f( points[i].x, points[i].y );
      }
      glEnd();
    }
  }

  if ( drawfunc )
    drawfunc();

  TwDraw();

  // Present frame buffer
  SDL_GL_SwapBuffers();
}

const std::vector< FrameWork::Point > &FrameWork::GetPoints()
{
  return points;
}

void FrameWork::BeginLineDrawing()
{
  // Set OpenGL camera
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glLineWidth( linewidth );
  glBegin( GL_LINES );
}

void FrameWork::DrawLine( const Point &point1, const Point &point2, float *color )
{
  glColor3f( color[0], color[1], color[2] );
  glVertex2f( point1.x, point1.y );
  glVertex2f( point2.x, point2.y );
}

void FrameWork::EndLineDrawing()
{
  glEnd();
}

void FrameWork::DrawLines( const std::vector<FrameWork::Point> &dpoints, float *color )
{
  float defaultcolor[] = { 0.0f, 1.0f, 0.0f };
  if ( color == NULL )
    color = defaultcolor;

  BeginLineDrawing();

  for ( unsigned i = 1; i < dpoints.size(); ++i )
  {
    DrawLine( dpoints[ i - 1 ], dpoints[ i ], color );
  }

  EndLineDrawing();
}

void FrameWork::DrawLineStripe( const std::vector<FrameWork::Point> &dpoints, float *color )
{
  float defaultcolor[] = { 0.0f, 1.0f, 0.0f };
  if ( color == NULL )
    color = defaultcolor;

  BeginLineDrawing();

  for ( unsigned i = 1; i < dpoints.size(); i += 2 )
  {
    DrawLine( dpoints[ i - 1 ], dpoints[ i ], color );
  }

  EndLineDrawing();
}

void FrameWork::DrawPoints( const std::vector< FrameWork::Point > &dpoints, float *color )
{
  float defaultcolor[] = { 0.5f, 0.5f, 0.5f };
  if ( color == NULL )
    color = defaultcolor;

  glPointSize( pointsize );
  glBegin( GL_POINTS );
  glColor3f( color[0], color[1], color[2] );
  for ( unsigned i = 0; i < dpoints.size(); ++i )
  {
    glVertex3f( dpoints[i].x, dpoints[i].y, 0.5f );
  }
  glEnd();
}

void FrameWork::SetDrawFunc( void ( * const &func )() )
{
  drawfunc = func;
}

void FrameWork::SetModes( TwEnumVal *modes, int count )
{
  TwType ModeType = TwDefineEnum( "Rotation Mode", modes, count );
  TwAddVarRW( bar, "Modes", ModeType, &currentmode, 
    " label='Mode' help='The mode that is selected.' ");

}

int FrameWork::GetMode()
{
  return currentmode;
}

float FrameWork::GetStep()
{
  return step;
}

float FrameWork::GetTvalue()
{
  return tvalue;
}

bool FrameWork::GetDrawShell()
{
  return drawshell;
}