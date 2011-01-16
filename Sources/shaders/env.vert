varying vec3 N;
varying vec3 vectorPC;
uniform vec3 posCamera;
uniform mat4 matInv;

void main(void) {
	

	gl_TexCoord[0]  = gl_MultiTexCoord0;

	// calcul de la matrice qui passe du repere objet au repere global
	// mat3 T = gl_TextureMatrix[0] * gl_ModelViewMatrix;
	mat4 T = matInv * gl_ModelViewMatrix;

	// vector between current position (in global) and camera
	vec4 newVector = T*gl_Vertex;

	vec3 vectorTCoord = newVector.xyz/newVector.w;
	vectorPC = posCamera - vectorTCoord;
	//vectorPC = vectorTCoord - posCamera;
	vectorPC = normalize(vectorPC);

	// normal
	vec4 normal4Param = vec4(gl_Normal.xyz, 1.);
	vec4 myN4Param = T*normal4Param;
	//N = normalize(myN4Param.xyz/myN4Param.w);
	N = gl_Normal;
	gl_Position = ftransform();

}





