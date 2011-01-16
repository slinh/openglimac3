varying vec3 position;
varying vec3 N;
varying vec3 T;
varying vec3 B;
varying mat3 TBN;
varying vec3 lightVector;
varying vec3 posCamVector;

void main(void)
{
//  gl_TexCoord[0]  = gl_MultiTexCoord0;
//  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

  
	// pass the position and normal in camera repere (gl_Vertex is in model repere at first)
  	vec4 P = gl_ModelViewMatrix * gl_Vertex;
	position = P.xyz/P.w;
	
	gl_TexCoord[0] =  gl_MultiTexCoord0;

	N = normalize(gl_NormalMatrix * gl_Normal);
	T = gl_NormalMatrix * gl_MultiTexCoord1.xyz; 


	T = normalize(T);

	// B = T ^ N
	B = cross(T, N);

	TBN = mat3(T.x, B.x, N.x, T.y, B.y, N.y, T.z, B.z, N.z);

	vec4 lightPosition = gl_LightSource[0].position;
	
	lightVector = lightPosition.xyz - position;
	lightVector = TBN * lightVector;
	
	posCamVector = -position;
	posCamVector = TBN * posCamVector;

	gl_Position=ftransform();
}
  



