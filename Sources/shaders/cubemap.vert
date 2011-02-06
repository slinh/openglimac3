varying vec3 TexCoord0;
void main()
{
     
  gl_Position = ftransform();
 
  TexCoord0  = gl_Normal;
  //TexCoord0 = gl_MultiTexCoord0;

}
