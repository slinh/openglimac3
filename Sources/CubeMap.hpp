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
		CubeMap(int size):size(size){};
		~CubeMap();

	
	private:
		int size;
    GLint displayListId;
 		GLuint idTex;
		GLuint idTex2;

		
	public:
void loadTextures(const char* negative_x,
                           const char* negative_z,
                           const char* negative_y,
                           const char* positive_x,
                           const char* positive_y,
                           const char* positive_z,
                           int idT);
void initSimpleCubeMap(const char* negative_x,
                           const char* negative_z,
                           const char* negative_y,
                           const char* positive_x,
                           const char* positive_y,
                           const char* positive_z
                           );
void initDoubleCubeMap(const char* negative_1_x,
                         const char* negative_1_z,
                         const char* negative_1_y,
                         const char* positive_1_x,
                         const char* positive_1_y,
                         const char* positive_1_z,
                         const char* negative_2_x,
                         const char* negative_2_z,
                         const char* negative_2_y,
                         const char* positive_2_x,
                         const char* positive_2_y,
                         const char* positive_2_z
                         );
void drawCubeMap(float size);
void display();

const GLuint getIdTex() const;
const GLuint getIdTex2() const;


//inline GLuint getIdTex() {return idTex;};
		
};
#endif
