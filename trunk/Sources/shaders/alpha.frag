uniform sampler2D colorMap;
uniform sampler2D alphaMap;

void main (void)
{
  vec4 color = texture2D(alphaMap, gl_TexCoord[0].xy);

  if(color.r < 0.1)
  {
		discard;
  }

  gl_FragColor = texture2D(colorMap, gl_TexCoord[0].xy);
  //gl_FragColor = color;
  
}
