////////////////////////////////////////////////////////////////////////////////
/*!
  @par    Project: CS260 Game
  @par    Course: CS-260F08
  @file   message.h
  @author Tommy A. Brosman IV
  @par    email:  tbrosman\@digipen.edu
  @date   November 19, 2008

  @brief

  @brief  Copyright (C) 2008 DigiPen Institute of Technology.
*/
////////////////////////////////////////////////////////////////////////////////

#pragma once

class Param
{
  public:
    enum Type
    {
      NONE,
      U_INT,
      S_INT,
      FLOAT_32,
      FLOAT_64,
      UI_VECTOR,
      F_VECTOR,
      STRING
    };

    Param();

    unsigned char type;

    union
    {
      unsigned ui;
      int      si;
      float    f32;
      double   f64;
      unsigned uiVec[4];
      float    fVec[4];
      char     str[32];
    };
};

const unsigned NULL_TYPE = 0;
const unsigned BROADCAST_TYPE = (unsigned)-1;
const unsigned START_TYPE = 1;

const unsigned NULL_ID   = 0;
const unsigned BROADCAST_ID   = (unsigned)-1;
const unsigned START_ID = 1;

typedef Param Message;

