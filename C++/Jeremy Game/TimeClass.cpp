////////////////////////////////////////////////////////////////////////////////
/*!
@par    Project:GAM300 Game
@par    Course: GAM300
@file   TimeClass.cpp
@author Jeremy Robert Anderson
@par    email: janders2\@digipen.edu
@date   October 14, 2009

@brief
A simple timer class and the Clock function

@brief  Copyright (C) 2009 DigiPen Institute of Technology.
*/
////////////////////////////////////////////////////////////////////////////////
#include "TimeClass.h"
#include <windows.h>

namespace Time
{
  long long Clock()
  {
    LARGE_INTEGER Frequency;
    LARGE_INTEGER Counter;
    if ( QueryPerformanceFrequency( &Frequency ) )
    {
      QueryPerformanceCounter( &Counter );
      return Counter.QuadPart * 1000000LL / Frequency.QuadPart;
    }
    return 0LL;
  }

  double Timer::GetFrame()
  {
    return FrameTime / 1000000.0;
  }

  double Timer::GetTotal()
  {
    return total / 1000000.0;
  }

  Timer::Timer()
  {
    Reset();
  }

  void	Timer::Start()
  {
    FrameStart = Clock();
  }

  void	Timer::Stop()
  {
    FrameTime = Clock() - FrameStart;
    total += FrameTime;
  }

  void Timer::Reset()
  {
    FrameStart = 0LL;
    FrameTime = 0LL;
    total = 0LL;
  }
}