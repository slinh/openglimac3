#ifndef __BBOX_HPP__
#define __BBOX_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "Texture.hpp"
#include "vector3d.h"
#include "ObjLoader.hpp"

class Obj;

typedef enum TypeWall_
{
	TOONWALL,
	CLASSIC
}TypeWall;

class Bbox
{
	
protected:
	float 	left;	 	// left border
	float 	right;	// right border
	float 	bottom;	// bottom border
	float 	top;		// top border
	float 	x;		// x position
	float 	y;		// y position
	
	float 	width;
	float 	height;
	bool 		init;	
	
	
	std::vector<Texture*>	textures;	
				
public:
	Bbox();
	Bbox(const Bbox & copy);
	~Bbox();	
		
	void initialize(ObjLoader* obj);
	void initialize(Obj* obj);
	
	bool intersect(const Bbox & box)const;
	bool intersect(const vector3df point)const;
	void updateCenter();
	void addPoint(const float & posX, const float & posY, const float & posZ);
	void displayWall(TypeWall type)const;
	void displayUpDown()const;

	inline const float & getX()const { return x; };
	inline const float & getY()const { return y; };

	const Texture & getTexture(int id)const;

	inline const float & getWidth()const { return width; };
	inline const float & getHeight()const { return height; };
	
	void initTextures();
	void bindTextures();
	void unbindTextures();
		
};

#endif
