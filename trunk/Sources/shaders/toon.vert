varying vec3 normale;
varying vec3 vecLight;
varying vec3 vecView;
varying vec3 halfVec;

void main()
{
  gl_TexCoord[0]  = gl_MultiTexCoord0;

  // normal
  normale = gl_NormalMatrix* gl_Normal;
  
  // light direction
  vec4 pTemp = gl_ModelViewMatrix * gl_Vertex;
  vec3 p = pTemp.xyz / pTemp.w; 
  vecLight = gl_LightSource[0].position.xyz - p;
  
  // view & half view
  vecView = -p;
  halfVec = gl_LightSource[0].halfVector.xyz / gl_LightSource[0].halfVector.w;

  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

}
