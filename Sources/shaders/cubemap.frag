uniform samplerCube id_tex;
varying vec3 TexCoord0;

void main()
{
  vec4 tex = textureCube(id_tex, TexCoord0); 
  
  gl_FragColor = tex;

}