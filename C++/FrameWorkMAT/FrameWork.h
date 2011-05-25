/* Start Header -------------------------------------------------------
File Name: FrameWork.h
Purpose: Manage Windowing and Point drawing line Drawing and other stuff.
Language: C++, Visual Studio 2008
Platform: Visual Studio 9, Windows
Author: Jeremy Anderson
- End Header --------------------------------------------------------*/
#include <vector>
#include "SDL/SDL.h"
#include "AntTweakBar/AntTweakBar.h"
#include "SDL/SDL_opengl.h"

class FrameWork
{
  public:
    struct Point
    {
      Point( float x = 0.0f, float y = 0.0f )
      {
        this->x = x;
        this->y = y;
      }

      Point( const Point &rhs )
      {
        x = rhs.x;
        y = rhs.y;
      }

      Point operator *( float num ) const
      {
        return Point( num * x, num * y );
      }

      Point operator /( float num )
      {
        return Point( num / x, num / y );
      }

      Point &operator *=( float num )
      {
        x *= num;
        y *= num;
        return *this;
      }

      Point &operator /=( float num )
      {
        x /= num;
        y /= num;
        return *this;
      }

      Point operator +( const Point &rhs )
      {
        return Point( x + rhs.x, y + rhs.y );
      }

      Point operator +=( const Point &rhs )
      {
        return *this = *this + rhs;
      }

      Point operator -( const Point &rhs )
      {
        return Point( x - rhs.x, y - rhs.y );
      }

      Point operator -=( const Point &rhs )
      {
        return *this = *this - rhs;
      }

      float x, y;
    };

    FrameWork();

    void Init();

    void Exit();

    void Update();

    bool Running();

    void Draw();

    const std::vector< Point > &GetPoints();

    void BeginLineDrawing();

    void DrawLine( const Point &point1, const Point &point2, float *color );

    void EndLineDrawing();

    void DrawLines( const std::vector<FrameWork::Point> &dpoints, float *color );

    void DrawLineStripe( const std::vector<FrameWork::Point> &dpoints, float *color );

    void DrawPoints( const std::vector< FrameWork::Point > &dpoints, float *color );

    void SetDrawFunc( void ( * const &func )() );

    void SetModes( TwEnumVal *modes, int count );

    int GetMode();

    float GetStep();

    float GetTvalue();

    bool GetDrawShell();

  private:
    bool Collision( Point &mouse, Point &point );

    void InitGL();

    void InitTWB();

    void ClearPoints();

    int currentmode;

    int width;
    int height;
    int bpp;
    bool quit;

    float pointsize;
    float pointcolor[3];
    float linewidth;
    float linecolor[3];

    bool clearpoints;

    TwBar *bar;

    int flags;

    bool pointgrabbed;
    Point *pointheld;

    bool drawlines;

    float tvalue;

    float step;

    bool drawshell;

    void ( * drawfunc )();

    std::vector< Point > points;
};