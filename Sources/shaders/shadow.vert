varying vec3 eye;
varying vec3 lightvec;
varying vec3 normal;
varying vec4 vertprojection;

uniform mat4 eyematrix;
uniform mat4 transmatrix;

void main(void)
{
  vec4 vertexpos = gl_ModelViewMatrix * gl_Vertex;
  vec3 verpos = (vertexpos.xyz)/vertexpos.w;

  lightvec = gl_LightSource[0].position.xyz - verpos;

  eye = -verpos;

  normal = gl_NormalMatrix * gl_Normal;

  vertprojection = eyematrix*transmatrix*gl_Vertex;

  gl_Position = ftransform();
}
