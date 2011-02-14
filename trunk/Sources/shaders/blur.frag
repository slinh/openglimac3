// Texture to blur
uniform sampler2D texture;

// Texture main
uniform sampler2D main;

// Texture size
uniform vec2 size;

// blur horizontal ou vertical
uniform bool horizontal;

// Taille du blurage
uniform int kernel_size;

uniform int pass;

void main()
{
	if( pass == 1)
	{
		vec2 pas = 1.0/size;
		vec2 uv = gl_TexCoord[0].st;
		vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	
		
		// HORIZONTAL BLUR
		if(horizontal) {
			int j = 0;
			int sum = 0;
			for(int i=-kernel_size; i<=kernel_size; i++) {
				vec4 value = texture2D(texture, uv + vec2(pas.x*float(i), 0.0));
				int factor = kernel_size+1 - int(abs(float(i)));
				color += value * float(factor);
				sum += factor;
			}
			color /= float(sum);
		}
		
		// VERTICAL BLUR
		else {
			int j = 0;
			int sum = 0;
			for(int i=-kernel_size; i<=kernel_size; i++) {
				vec4 value = texture2D(texture, uv + vec2(0.0, pas.y*float(i)));
				int factor = kernel_size+1 - int(abs(float(i)));
				color += value * float(factor);
				sum += factor;
			}
			color /= float(sum);
		}
	
		gl_FragColor = color;
		gl_FragColor.a = 1.0;
	}
	else // pass 3
	{
		
		// Main
		vec4 myMainColor = texture2D(main, vec2(gl_TexCoord[0].st));
	   // Glow
	   vec4 myGlowColor = texture2D(texture, vec2(gl_TexCoord[0].st));
	
	    gl_FragColor = myMainColor+myGlowColor;
	}
	

}
