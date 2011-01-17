varying vec3 N;
varying vec3 vectorPC;
uniform samplerCube myTexCube;

void main(void) {

  float myDot = dot(N,vectorPC);
  vec3 myN = N;
  if( myDot < 0.)
  {
    // discard;
  }

   vec3 tmpR = vectorPC - myDot * myN;
	
	
	
	vec3 reflect = vectorPC - 2.*tmpR;
	
	gl_FragColor = textureCube(myTexCube, reflect);


}
