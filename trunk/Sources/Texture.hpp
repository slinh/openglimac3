#ifndef __TEXTURE_HPP_
#define __TEXTURE_HPP_

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

class Texture 
{
	protected:
		int				id;
		GLuint 			idTex;
		std::string 	file;
	
	public:
	
		Texture(int id, std::string s) : id(id), file(s) {} ;
		~Texture();
    
    const std::string & getFile() const { return file; }
    std::string & setFile() { return file; }

	void create(int repeat=0);
	void bind() const;
	void unbind() const;
	void tryCube();
};

#endif
