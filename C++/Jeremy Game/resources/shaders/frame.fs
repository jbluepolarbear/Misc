//varying vec3 normal;
//varying vec3 pos;

uniform sampler2D Texture;
uniform vec3 Color;
uniform float alpha;

void main()
{
  gl_FragColor = texture2D( Texture, gl_TexCoord[0].st );
  if ( Color != vec3( 1.0, 0.0, 1.0 ) && gl_FragColor.xyz != vec3(0.0,0.0,0.0) )
	gl_FragColor.rgb = Color;
  gl_FragColor.a *= alpha;
  //if ( gl_FragColor.a < 0.2 )
//	discard;
}
