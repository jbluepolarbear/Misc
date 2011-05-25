/*---------------------------------------------------------------------------
Project Title : Game Engine
File Name : Input.h
Author : Jeremy Anderson
Creation Date : 05/05/08
Purpose : Input Declarations
History
----------------------------------------------------------------------------*/
#ifndef INPUT_H
#define INPUT_H
// ---------------------------------------------------------------------------
#include "SDL/SDL.h"

enum MouseButtons
{
  MOUSE_LEFTBUTTON = 0,
  MOUSE_MIDDLEBUTTON,
  MOUSE_RIGHTBUTTON,
  MOUSE_4BUTTON,
  MOUSE_5BUTTON,
  MOUSE_6BUTTON,
  MOUSE_7BUTTON,
  MOUSE_8BUTTON,
  MOUSE_COUNT
};

const int JoystickAxisMin = -32768;
const int JoystickAxisMax = 32767;
// Function prototypes

class Input
{
  public:
    Input();
    virtual ~Input();

    bool Init();
    void Reset();
    void Update();
    void Exit();
    bool KeyBoardChanged();

    bool KeyPressed( int key );
    bool KeyPrevious( int key );
    bool KeyTriggered( int key );
    bool KeyReleased( int key );

    bool MouseButtonPressed( int button );
    bool MouseButtonPrevious( int button );
    bool MouseButtonTriggered( int button );
    bool MouseButtonReleased( int button );

    unsigned short GetCursorX();
    unsigned short GetCursorY();

    float GetAxis( int axis );

    bool JoystickActive();

  private:
    int NumberKeys;
    bool keyboardChanged;
    bool CurrentKeyBuffer[SDLK_LAST];
    bool PreviousKeyBuffer[SDLK_LAST];
    bool TriggeredKeyBuffer[SDLK_LAST];
    bool ReleasedKeyBuffer[SDLK_LAST];

    bool CurrentMouseBuffer[MOUSE_COUNT];
    bool PreviousMouseBuffer[MOUSE_COUNT];
    bool TriggeredMouseBuffer[MOUSE_COUNT];
    bool ReleasedMouseBuffer[MOUSE_COUNT];

    int x;
    int y;

    int numberOfJoysticks;
    int numberOfAxis;
    int numberOfButtons;
    SDL_Joystick *joystick;

    float *axes;
    bool *CurrentJoyButtonBuffer;
    bool *PreviousJoyButtonBuffer;
    bool *TriggeredJoyButtonBuffer;
    bool *ReleasedJoyButtonBuffer;
};

// ---------------------------------------------------------------------------
#endif/*INPUT_CONTROLLER_H*/