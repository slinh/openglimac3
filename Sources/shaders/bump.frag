
varying vec3 vecLight;

void main()
{
  vec4 veccouleur;  
  gl_TexCoord[0] *= 16.;
  vec3 normal=vec3(0.,0.,1.);
  vec2 texcoord = vec2(mod(gl_TexCoord[0].x,2.)-1., mod(gl_TexCoord[0].y,2.)-1.);
  float radius = 0.9;

  if(sqrt(pow(texcoord.x, 2.) + pow(texcoord.y, 2.)) < radius )
  {
    float h=sqrt(pow(radius,2.)-pow(length(texcoord),2.));  
    normal = vec3(texcoord,h);
    //veccouleur = vec4(1.0, 1.0, 1.0, 1.0);
  } 
  else
  {
    //veccouleur = vec4(1.0, 0.0, 0.0, 1.0);
  }
  gl_FragColor = /*veccouleur +*/ gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * max(0., dot(normalize(normal),normalize(vecLight)));
}
