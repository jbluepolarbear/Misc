#include "GLee.h"
#include "SDL/SDL_opengl.h"
#include "SOIL/SOIL.h"
#include "Sprite.h"

namespace JeremySprite
{
  JRect::JRect( float top /* = 0.0f */, float bottom /* = 0.0f */, float left /* = 0.0f */, float right /* = 0.0f */ )
  {
    this->top = top;
    this->bottom = bottom;
    this->left = left;
    this->right = right;
  }

  float &JRect::operator []( const int Index )
  {
    switch(Index)
    {
    case 0:
      return top;
    case 1:
      return bottom;
    case 2:
      return left;
    default:
      return right;
    }
  }

  ////////////////////////////////////////////
  /// JSprite
  ////////////////////////////////////////////
  JSprite::JSprite( const unsigned image )
  {
    texture = image;
    position = Vec2();
    center = Vec2();
    scale = Vec2( 1.0f, 1.0f );
    rotation = 0.0f;
    alpha = 1.0f;
    textureCoord = JRect( 1.0f, 0.0f, 0.0f, 1.0f );
    useCamera = true;
    color = Vec3( 1.0f, 0.0f, 1.0f );
    blendMode = JBlend::Mode::Alpha;
    buildTransformation = true;
  }

  JSprite::~JSprite()
  {

  }

  ///////////////////////////
  void JSprite::SetPosition( const Vec2 &npos )
  {
    position = npos;
    buildTransformation = true;
  }

  void JSprite::SetPosition( const float x, const float y )
  {
    position = Vec2( x, y );
    buildTransformation = true;
  }

  const Vec2 &JSprite::GetPosition() const
  {
    return position;
  }

  const float JSprite::GetPositionX() const
  {
    return position.x;
  }

  const float JSprite::GetPositionY() const
  {
    return position.y;
  }

  void JSprite::SetCenter( const Vec2 &nctr )
  {
    center = nctr;
    buildTransformation = true;
  }

  void JSprite::SetCenter( const float x, const float y )
  {
    center = Vec2( x, y );
    buildTransformation = true;
  }

  const Vec2 &JSprite::GetCenter() const
  {
    return center;
  }

  const float JSprite::GetCenterX() const
  {
    return center.x;
  }

  const float JSprite::GetCenterY() const
  {
    return center.y;
  }

  void JSprite::SetScale( const Vec2 &nscl )
  {
    scale = nscl;
    buildTransformation = true;
  }

  void JSprite::SetScale( const float x, const float y )
  {
    scale = Vec2( x, y );
    buildTransformation = true;
  }

  const Vec2 &JSprite::GetScale() const
  {
    return scale;
  }

  const float JSprite::GetScaleX() const
  {
    return scale.x;
  }

  const float JSprite::GetScaleY() const
  {
    return scale.y;
  }

  void JSprite::SetRotation( const float angle )
  {
    rotation = angle;
    buildTransformation = true;
  }

  const float JSprite::GetRotation() const
  {
    return rotation;
  }

  void JSprite::SetZLevel( const float zlvl )
  {
    zlevel = zlvl;
  }

  const float JSprite::GetZLevel() const
  {
    return zlevel;
  }

  void JSprite::SetAlpha( const float nalpha )
  {
    alpha = nalpha;
  }

  const float JSprite::GetAlpha() const
  {
    return alpha;
  }

  void JSprite::SetUseCamera( const bool nusecam )
  {
    useCamera = nusecam;
  }

  const bool JSprite::GetUseCamera() const
  {
    return useCamera;
  }

  void JSprite::SetTextureCoord( const JRect &nTexCoord )
  {
    textureCoord = nTexCoord;
  }

  void JSprite::SetTextureCoord( const float left, const float right, const float top, const float bottom )
  {
    textureCoord = JRect( left, right, top, bottom );
  }

  void JSprite::MoveTextureCoord( const Vec2 &distance )
  {
    textureCoord.left += distance.x;
    textureCoord.right += distance.x;
    textureCoord.top += distance.y;
    textureCoord.bottom += distance.y;
  }

  void JSprite::MoveTextureCoord( const float x, const float y )
  {
    textureCoord.left += x;
    textureCoord.right += x;
    textureCoord.top += y;
    textureCoord.bottom += y;
  }

  const JRect &JSprite::GetTextureCoord() const
  {
    return textureCoord;
  }

  void JSprite::SetTexture( const unsigned ntexture )
  {
    texture = ntexture;
  }

  const unsigned JSprite::GetTexture() const
  {
    return texture;
  }

  void JSprite::SetColor( const Vec3 &ncolor )
  {
    color = ncolor;
  }

  const Vec3 &JSprite::GetColor() const
  {
    return color;
  }

  void JSprite::SetBlendMode( const JBlend::Mode &nblendmode )
  {
    blendMode = nblendmode;
  }

  const JBlend::Mode &JSprite::GetBlendMode() const
  {
    return blendMode;
  }

  ///////////////////////////
  const Matrix &JSprite::GetTransformation() const
  {
    if ( !buildTransformation )
      return transformation;

    Matrix rot = RotateZ( rotation );
    Matrix scl = Scale( scale.x, scale.y );
    Matrix pos = Translate( position.x, position.y );
    Matrix ctr = Translate( -center.x, -center.y );

    transformation = pos * rot * scl * ctr;

    buildTransformation = false;
    return transformation;
  }


  void JSprite::Draw() const
  {
    glPushMatrix();
    if ( !useCamera )
      glLoadIdentity();

    glMultMatrixf( GetTransformation().Transpose() );
    glBegin( GL_QUADS );
      glTexCoord2f( textureCoord.left, textureCoord.top );	glVertex2f( 0.0f, 0.0f );
      glTexCoord2f( textureCoord.right, textureCoord.top );	glVertex2f( 1.0f, 0.0f );
      glTexCoord2f( textureCoord.right, textureCoord.bottom );	glVertex2f( 1.0f, 1.0f );
      glTexCoord2f( textureCoord.left, textureCoord.bottom);	glVertex2f( 0.0f, 1.0f );
    glEnd();

    glPopMatrix();
  }

  /////////////////////////////////
  /// JSpriteRenderer
  /////////////////////////////////
  JSpriteRenderer::JSpriteRenderer()
  {

  }

  JSpriteRenderer::~JSpriteRenderer()
  {

  }

  void JSpriteRenderer::Init()
  {
    shader.AddFragmentShader( "resources/shaders/frame.fs" );
    shader.AddVertexShader( "resources/shaders/frame.vs" );

    shader.LinkProgram();
  }

  void JSpriteRenderer::Draw( const JSprite *jsprite )
  {
    jsprites.push_back( jsprite );
  }

  void InsertionSort( std::vector<const JSprite *> &A )
  {
    int size = A.size();
    for ( int i = 1; i < size; ++i )
    {
      const JSprite *value = A[i];
      int j;

      for ( j = i - 1; j >= 0 && A[j]->GetZLevel() > value->GetZLevel(); --j )
        A[ j + 1 ] = A[j];

      A[ j + 1 ] = value;
    }
  }
  // draws all stored jsprites then clears it.
  void JSpriteRenderer::DrawBatch( bool sort )
  {
    shader.BindProgram();
    glActiveTexture( GL_TEXTURE0 );

    //std::sort( objects.begin(), objects.end() );
    if ( sort )
      InsertionSort(jsprites);

    int size = jsprites.size();
    for ( int i = 0; i < size; ++i )
    {
      glBindTexture( GL_TEXTURE_2D, jsprites[i]->GetTexture() );
      shader.uniform( "Texture", 0 );

      shader.uniform( "alpha", jsprites[i]->GetAlpha() );
      float tc[3] = { jsprites[i]->GetColor().x, jsprites[i]->GetColor().y, jsprites[i]->GetColor().z };
      shader.uniform<3>( "Color", tc );

      //draw
      jsprites[i]->Draw();
    }

    jsprites.clear();


    glBindTexture( GL_TEXTURE_2D, 0 );
    shader.UnBindProgram();
  }
}