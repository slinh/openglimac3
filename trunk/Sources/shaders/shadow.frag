uniform sampler2DShadow shadowmap;

varying vec3 eye;
varying vec3 lightvec;
varying vec3 normal;
varying vec4 vertprojection;

void main(void)
{

  vec3 N = normalize(normal);
  float Lnorm = length(lightvec);
  vec3 L = normalize(lightvec);
  vec3 E = normalize(eye);

  vec4  colorfrag =
    gl_LightSource[0].ambient *
    gl_FrontMaterial.ambient;

  colorfrag +=
    max(dot(L,N),0.0) *
    gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;

 vec3 R = reflect(-L,N);

  colorfrag +=
    pow(max(dot(E,R),0.0),gl_FrontMaterial.shininess) *
    gl_LightSource[0].specular *
    gl_FrontMaterial.specular;

  colorfrag *= 1.0/(gl_LightSource[0].constantAttenuation +
      gl_LightSource[0].linearAttenuation * Lnorm +
      gl_LightSource[0].quadraticAttenuation * Lnorm * Lnorm );

  vec4 myvertprojection = vertprojection /vertprojection.w;
 // myvertprojection.z += 0.0005;


  float shadow = shadow2D(shadowmap,(myvertprojection.stp)).r;

  vec4 colorambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

  /*if(myvertprojection.s > 1.0 || myvertprojection.t > 1.0)
         gl_FragColor = colorfrag;
  else{*/
    if(shadow != 1.0)
      gl_FragColor = colorambient;
    else
      gl_FragColor = colorfrag;
  //}

}
