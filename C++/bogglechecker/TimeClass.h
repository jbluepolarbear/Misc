////////////////////////////////////////////////////////////////////////////////
/*!
@par    Project:GAM300 Game
@par    Course: GAM300
@file   TimeClass.h
@author Jeremy Robert Anderson
@par    email: janders2\@digipen.edu
@date   October 14, 2009

@brief
A simple timer class and the Clock function

@brief  Copyright (C) 2009 DigiPen Institute of Technology.
*/
////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace Time
{
  long long Clock();

  class Timer
  {
    public:
      Timer();
      double Get();
      void Start();
      void Stop();
      void Reset();
    private:
      long long	FrameTime;
      long long FrameStart;
  };
}