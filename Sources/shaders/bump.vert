varying vec3 N;
varying vec3 vecLight;

void main()
{
  // normal
  vec3 N = gl_NormalMatrix * gl_Normal;
  vec3 T = gl_ModelViewMatrix * gl_MultiTexCoord1;
  vec3 B = cross(T, N);

  mat3 matTBN = mat3(T,B,N);
 // matTBN = transpose(matTBN);   
  
  // light direction
  vec4 pTemp = gl_ModelViewMatrix * gl_Vertex;
  vec3 p = pTemp.xyz / pTemp.w; 
  vecLight = gl_LightSource[0].position.xyz - p;

  vecLight = vecLight * matTBN;

  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_TexCoord[0]  = gl_MultiTexCoord0;
}
