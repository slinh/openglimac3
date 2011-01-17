#ifndef __CUBEMAP__HPP
#define __CUBEMAP__HPP

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>


#ifdef __APPLE__
	#include <GLUT/glut.h>
   
#else
	#include <GL/glut.h>
#endif

class CubeMap{
	
	public:
		CubeMap(){};
		CubeMap(int size, GLuint idtex):size(size), idTex(idtex){};
		~CubeMap();
	
	private:
		int size;
		GLuint idTex;
    GLint displayListId;
		
	public:
void initCubeMap(const char* negative_x,
                           const char* negative_z,
                           const char* negative_y,
                           const char* positive_x,
                           const char* positive_y,
                           const char* positive_z
                           );
void drawCubeMap(float size);
void display();
inline GLuint getIdTex() {return idTex;};
		
};
#endif
