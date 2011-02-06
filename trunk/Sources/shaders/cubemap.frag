uniform samplerCube id_tex_day;
uniform samplerCube id_tex_night;
varying vec3 TexCoord0;
uniform float timer;

void main()
{
	vec4 tex_day = textureCube(id_tex_day, TexCoord0); 
  vec4 tex_night = textureCube(id_tex_night, TexCoord0); 
  
  gl_FragColor = timer * tex_day.rgba + tex_night;

}