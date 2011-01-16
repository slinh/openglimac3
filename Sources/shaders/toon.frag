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
  float val = intensity.r + intensity.g + intensity.b/3.;
  vec4 color = texture2D(diffuseTexture, gl_TexCoord[0].xy);

  if(val > 0.95)
    color *= 0.99;
  else if(val > 0.5)
    color *= 0.6;
  else if(val > 0.25)
    color *= 0.3;
  else
    color *= 0.2;

/*
  if (intensity.rvb > 0.03)
    color = vec4(1.0,0.5,0.5,1.0);  // rose pale
  else if (intensity.r > 0.025)
    color = vec4(0.6,0.3,0.3,1.0);  // rose sale
  else if (intensity.r > 0.01)
    color = vec4(0.4,0.2,0.2,1.0);  // bordeau moche
  else
    color = vec4(0.2 ,0.1, 0.1, 1.0);  // gris foncé
*/
 gl_FragColor = color;

}
