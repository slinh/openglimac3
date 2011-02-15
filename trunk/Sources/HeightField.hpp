#ifndef __HEIGHTFIELD_H__
#define __HEIGHTFIELD_H__

#include "vector3d.h"
#include "Texture.hpp"
#include <string>
#include <vector>

#ifdef __APPLE__
	#include <openGL/gl.h>
#else
	#include <GL/gl.h>
#endif

class Vert{
public:
        float x;
        float y;
        float z;
};

class TexCoord{
public:
        float u;
        float v;
};

class HeightField {

protected:
	float height;
	float width;
	
	std::string filename;
	GLint displayListId;
	std::vector<Texture *> textures;
	
	vector3df scale;

        int vhVertexCount;
        Vert *vhVertices;
        TexCoord *vhTexCoords;

        GLuint vhVBOVertices;
        GLuint vhVBOTexCoords;
	
public:
	HeightField(std::string hFileName, const int hWidth, const int hHeight);
	~HeightField();
	
	void init(void);
	void display(void);
        void draw(void);

        //VBO
        int hLOD;
        //VBO
	
        char hHeightField[512][512];
	

};

#endif
