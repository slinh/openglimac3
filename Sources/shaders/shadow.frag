uniform sampler2DShadow shadowmap;
uniform sampler2D diffuseTexture;
uniform int boolShadow;

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
  vec4 color = texture2D(diffuseTexture, gl_TexCoord[0].xy);
  vec4 colorfrag;
  vec4 colorambient;
  
  if(boolShadow == 1)
  {
     colorfrag = 
      gl_LightSource[0].ambient * 
      color;

    colorfrag += 
      max(dot(L,N),0.0) * 
      gl_LightSource[0].diffuse * color;
  }
  else
  {
     colorfrag = 
      gl_LightSource[0].ambient * 
      gl_FrontMaterial.ambient;

    colorfrag += 
      max(dot(L,N),0.0) * 
      gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
  }
  
  vec3 R = reflect(-L,N);
  
  colorfrag += 
    pow(max(dot(E,R),0.0),gl_FrontMaterial.shininess) * 
    gl_LightSource[0].specular * 
    gl_FrontMaterial.specular;
  
  colorfrag *= 1.0/(gl_LightSource[0].constantAttenuation +
		  gl_LightSource[0].linearAttenuation * Lnorm +
		  gl_LightSource[0].quadraticAttenuation * Lnorm * Lnorm );

  vec4 proj = vertprojection / vertprojection.w;

  float shadow = shadow2D(shadowmap,(proj.stp)).r;
  
  if(boolShadow == 1)
  {
    colorambient = gl_LightSource[0].ambient * color;
  }
  else
  {
    colorambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
    
  }
    

  if(proj.s > 1.0 || proj.t > 1.0)
         gl_FragColor = colorfrag;
  else{
    if(shadow != 1.0)
      gl_FragColor = colorambient;
    else
      gl_FragColor = colorfrag;
  }

  //  gl_FragColor = color;
}
