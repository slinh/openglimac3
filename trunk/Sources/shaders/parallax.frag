uniform sampler2D wallTex;
uniform sampler2D heightmapTex;
uniform sampler2D normalmapTex;
varying vec3 position;
varying vec3 N;
varying vec3 lightVector;
varying vec3 posCamVector;



void main(void)
{
//  vec4 alpha_color = texture2D(wallTex, gl_TexCoord[0].xy);
//  gl_FragColor = alpha_color;

  float attenuationCst;
	float attenuationLineaire;
 	float attenuationQuadra;
	float factor;
	float lightDistance;

	vec4 diffuse;
	vec4 ambient;
	vec4 specular = vec4(0.);
	float scalar;
	float scalar2;
	vec3 vectorR;

  float myS = 0.04;

	vec4 color;

	vec4 myWall = texture2D(wallTex, vec2(gl_TexCoord[0].st));

	float myH = texture2D(heightmapTex, vec2(gl_TexCoord[0].st)).r;
	float offsetDist = (myH-0.5)*(myS);
  
	vec3 myPosCamVector = normalize(posCamVector);

	vec2 offset = offsetDist* (myPosCamVector.xy/myPosCamVector.z);
	vec2 texel = offset + gl_TexCoord[0].st;

	vec3 myNormal = texture2D(normalmapTex, texel).xyz;
	
	myNormal = myNormal*2.0 - vec3(1.0);
	myNormal = normalize(myNormal);


	//factors
	lightDistance = length(lightVector);
  	vec3 myLightVector = normalize(lightVector);
	attenuationCst = gl_LightSource[0].constantAttenuation;
	attenuationLineaire = gl_LightSource[0].linearAttenuation;
	attenuationQuadra = gl_LightSource[0].quadraticAttenuation;
	
	// softening factor	
	factor = 1.0 / (attenuationCst + attenuationLineaire*lightDistance + attenuationQuadra*lightDistance*lightDistance);

	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;

	scalar = max(dot(myLightVector,myNormal), 0.0);

	diffuse = scalar * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	// calculate half vector

	vectorR = 2. * dot(myLightVector,N) * N - myLightVector;
	scalar2 = max(dot(vectorR,myPosCamVector), 0.0);

	scalar2 = pow(scalar2,gl_FrontMaterial.shininess);
	specular = scalar2 * gl_FrontMaterial.specular * gl_LightSource[0].specular;


	 gl_FragColor = diffuse*myWall;
//	 gl_FragColor = myWall;
 // gl_FragColor = vec4(10*N.x, 10*N.y, 10*N.z, 1.0f);
}

