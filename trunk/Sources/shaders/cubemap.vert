varying vec3 TexCoord0;
void main()
{
     
  gl_Position = ftransform();
 
  TexCoord0  = gl_Normal;

}
