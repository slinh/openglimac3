varying vec3 normale;
varying vec3 vecLight;
varying vec3 vecView;
varying vec3 halfVec;
uniform sampler2D diffuseTexture;

void main()
{
  vec4 Dif = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * max(0., dot(normalize(normale),normalize(vecLight)));
  vec4 Spec = gl_LightSource[0].specular * gl_FrontMaterial.specular * max(0., dot(normalize(vecView), normalize(halfVec)));
  float att= 1./(4.*3.14*length(vecLight));
  vec4 intensity = gl_LightSource[0].ambient * gl_FrontMaterial.ambient + att*(Dif + Spec);
  float valAmb = intensity.r + intensity.g + intensity.b/3.;
  float valDiff = Dif.r + Dif.g + Dif.b/3.;
  float valSpec = Spec.r + Spec.g + Spec.b/3.;
  vec4 color = texture2D(diffuseTexture, gl_TexCoord[0].xy);

  Dif = normalize(gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse);
	if(valDiff < 0.3)
		gl_FragColor = Dif*0.3;
	else if(valDiff < 0.6)
		gl_FragColor = Dif*0.6;
	else
		gl_FragColor = Dif;
/*
	Spec = normalize(gl_FrontMaterial.specular * gl_LightSource[0].specular);
	if(valSpec < 0.3)
		gl_FragColor += Spec*0.3;
	else if(valSpec < 0.6)
		gl_FragColor += Spec*0.6;
	else
		gl_FragColor += Spec;
*/

	color = normalize(color);
  if(valAmb < 0.3)
		gl_FragColor += color*0.3;
	else if(valAmb < 0.6)
		gl_FragColor += color*0.6;
	else
		gl_FragColor += color;

}
