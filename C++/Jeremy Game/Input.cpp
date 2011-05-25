/*---------------------------------------------------------------------------
Project Title : Game Engine
File Name : InputContoller.cpp
Author : Jeremy Anderson
Creation Date : 03/15/08
Purpose : InputContoller definitions
History
----------------------------------------------------------------------------*/

#include "Input.h"
#include <iostream>

Input::Input()
{
  NumberKeys = SDLK_LAST;
  memset( CurrentKeyBuffer, 0, SDLK_LAST * sizeof(bool) );
  memset( PreviousKeyBuffer, 0, SDLK_LAST * sizeof(bool) );
  memset( TriggeredKeyBuffer, 0, SDLK_LAST * sizeof(bool) );
  memset( ReleasedKeyBuffer, 0, SDLK_LAST * sizeof(bool) );

  memset( CurrentMouseBuffer, 0, MOUSE_COUNT * sizeof(bool) );
  memset( PreviousMouseBuffer, 0, MOUSE_COUNT * sizeof(bool) );
  memset( TriggeredMouseBuffer, 0, MOUSE_COUNT * sizeof(bool) );
  memset( ReleasedMouseBuffer, 0, MOUSE_COUNT * sizeof(bool) );

  x = 0;
  y = 0;

  numberOfJoysticks = 0;
  joystick = NULL;
  axes = NULL;
  CurrentJoyButtonBuffer = NULL;
  PreviousJoyButtonBuffer = NULL;
  TriggeredJoyButtonBuffer = NULL;
  ReleasedJoyButtonBuffer = NULL;
}

bool Input::Init()
{  
  numberOfJoysticks = SDL_NumJoysticks();

  if ( numberOfJoysticks > 0 )
  {
    joystick = SDL_JoystickOpen(0);
    if (joystick)
    {
      numberOfAxis = SDL_JoystickNumAxes(joystick);
      numberOfButtons = SDL_JoystickNumButtons(joystick);
      SDL_JoystickEventState(SDL_ENABLE);

      axes = new float[numberOfAxis];
      CurrentJoyButtonBuffer = new bool[numberOfButtons];
      PreviousJoyButtonBuffer = new bool[numberOfButtons];
      TriggeredJoyButtonBuffer = new bool[numberOfButtons];
      ReleasedJoyButtonBuffer = new bool[numberOfButtons];

      memset( axes, 0, numberOfAxis * sizeof(float) );
      memset( CurrentJoyButtonBuffer, 0, numberOfButtons * sizeof(bool) );
      memset( PreviousJoyButtonBuffer, 0, numberOfButtons * sizeof(bool) );
      memset( TriggeredJoyButtonBuffer, 0, numberOfButtons * sizeof(bool) );
      memset( ReleasedJoyButtonBuffer, 0, numberOfButtons * sizeof(bool) );
    }
  }

  return true;
}

Input::~Input()
{
  Exit();
}

void Input::Exit()
{
  if ( joystick )
  {
    SDL_JoystickClose(joystick);
    joystick = NULL;
  }

  if ( axes )
  {
    delete [] axes;
    axes = NULL;
  }

  if ( CurrentJoyButtonBuffer )
  {
    delete [] CurrentJoyButtonBuffer;
    CurrentJoyButtonBuffer = NULL;
  }

  if ( PreviousJoyButtonBuffer )
  {
    delete [] PreviousJoyButtonBuffer;
    PreviousJoyButtonBuffer = NULL;
  }

  if ( TriggeredJoyButtonBuffer )
  {
    delete [] TriggeredJoyButtonBuffer;
    TriggeredJoyButtonBuffer = NULL;
  }

  if ( ReleasedJoyButtonBuffer )
  {
    delete [] ReleasedJoyButtonBuffer;
    ReleasedJoyButtonBuffer = NULL;
  }
}

void Input::Reset()
{
  memset( CurrentKeyBuffer, 0, SDLK_LAST * sizeof(bool) );
  memset( PreviousKeyBuffer, 0, SDLK_LAST * sizeof(bool) );
  memset( TriggeredKeyBuffer, 0, SDLK_LAST * sizeof(bool) );
  memset( ReleasedKeyBuffer, 0, SDLK_LAST * sizeof(bool) );

  memset( CurrentMouseBuffer, 0, MOUSE_COUNT * sizeof(bool) );
  memset( PreviousMouseBuffer, 0, MOUSE_COUNT * sizeof(bool) );
  memset( TriggeredMouseBuffer, 0, MOUSE_COUNT * sizeof(bool) );
  memset( ReleasedMouseBuffer, 0, MOUSE_COUNT * sizeof(bool) );

  if ( joystick )
  {
    memset( axes, 0, numberOfAxis * sizeof(float) );
    memset( CurrentJoyButtonBuffer, 0, numberOfButtons * sizeof(bool) );
    memset( PreviousJoyButtonBuffer, 0, numberOfButtons * sizeof(bool) );
    memset( TriggeredJoyButtonBuffer, 0, numberOfButtons * sizeof(bool) );
    memset( ReleasedJoyButtonBuffer, 0, numberOfButtons * sizeof(bool) );
  }

  x = 0;
  y = 0;
}

bool Input::KeyBoardChanged()
{
  return keyboardChanged;
}

float normlizeJoystick( int val )
{
  float temp = (val - JoystickAxisMin)/float(JoystickAxisMax-JoystickAxisMin);
  temp = temp * 2.0f - 1.0f;
  if ( temp < 0.0001f && temp > -0.0001f )
    temp = 0.0f;
  return temp;
}

void Input::Update()
{
  unsigned char *buffer = SDL_GetKeyState( &NumberKeys );

  keyboardChanged = false;
   
  for ( int i = 0; i < NumberKeys; ++i )
  {
    //if ( CurrentKeyBuffer[i] )
    //  __asm int 3;
    CurrentKeyBuffer[i] = buffer[i];

    if ( PreviousKeyBuffer[i] != CurrentKeyBuffer[i] )
      keyboardChanged = true;

    TriggeredKeyBuffer[i] = !PreviousKeyBuffer[i] && CurrentKeyBuffer[i];

    ReleasedKeyBuffer[i] = PreviousKeyBuffer[i] && !CurrentKeyBuffer[i];
  }

  memcpy( PreviousKeyBuffer, CurrentKeyBuffer, sizeof( PreviousKeyBuffer ) );

  unsigned MouseState = SDL_GetMouseState( &x, &y );

  for ( int i = 0; i < MOUSE_COUNT; ++i )
  {
    CurrentMouseBuffer[i] = MouseState & SDL_BUTTON(i+1);

    TriggeredMouseBuffer[i] = !PreviousMouseBuffer[i] && CurrentMouseBuffer[i];

    ReleasedMouseBuffer[i] = PreviousMouseBuffer[i] && !CurrentMouseBuffer[i];
  }

  memcpy( PreviousMouseBuffer, CurrentMouseBuffer, sizeof( PreviousMouseBuffer ) );

  // Joysticks

  if ( joystick )
  {
    for ( int i = 0; i < numberOfAxis; ++i )
    {
      axes[i] = normlizeJoystick( SDL_JoystickGetAxis( joystick, i ) );
    }
  }
}

bool Input::KeyPressed( int key )
{
  if ( CurrentKeyBuffer[key] )
    return true;
  return false;
}

bool Input::KeyPrevious( int key )
{ 
  if ( PreviousKeyBuffer[key] )
    return true;
  return false;
}

bool Input::KeyTriggered( int key )
{
  if ( TriggeredKeyBuffer[key] )
    return true;
  return false;
}

bool Input::KeyReleased( int key )
{
  if ( ReleasedKeyBuffer[key] )
    return true;
  return false;
}

bool Input::MouseButtonPressed( int key )
{
  if ( CurrentMouseBuffer[key] )
    return true;
  return false;
}

bool Input::MouseButtonPrevious( int key )
{ 
  if ( PreviousMouseBuffer[key] )
    return true;
  return false;
}

bool Input::MouseButtonTriggered( int key )
{
  if ( TriggeredMouseBuffer[key] )
    return true;
  return false;
}

bool Input::MouseButtonReleased( int key )
{
  if ( ReleasedMouseBuffer[key] )
    return true;
  return false;
}

float Input::GetAxis( int axis )
{
  if ( numberOfJoysticks > 0 )
    return axes[axis];
  return 0.0f;
}

unsigned short Input::GetCursorX()
{
  return x;
}

unsigned short Input::GetCursorY()
{
  return y;
}

bool Input::JoystickActive()
{
  return numberOfJoysticks > 0;
}
