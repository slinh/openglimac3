#include "Bbox.hpp"
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
#include "vector3d.h"

#include "Obj.hpp"
#include "Draw.hpp"
#include "Texture.hpp"

std::vector<Texture*> Bbox::textures;


Bbox::Bbox()
{
	init = false;
}

Bbox::~Bbox(){}

/*

float Bbox::getWidth()const
{
	if(init)
		return width;

	else
		return right-left;
}

float Bbox::getHeight()const
{
	if(init)
		return height;

	else
		return top - bottom;
}*/
		
		
void Bbox::initialize(Obj* obj)
{
	// apply scale and translate :
	
	height = top-bottom;
	width = right-left;
	
		
	float newWidth = width * (float) obj->getScale().X;
	float newHeight = height * (float) obj->getScale().Y;
	
	
	x = obj->getTranslate().X;
	y = obj->getTranslate().Z;
	
	
	right = x + (newWidth/2.);
	left = x - (newWidth/2.);
	
	top = y + (newHeight/2.);
	bottom = y - (newHeight/2.);
	
  
  // size-up
  /*
	right += 1.f;
  left -= 1.f;
  top += 1.f;
  bottom -= 1.f; 
	*/
	height = top-bottom;
	width = right-left;

	
}

bool Bbox::intersect(const Bbox & box)const
{
	return true;
}

bool Bbox::intersect(const vector3df point)const
{	

	return	(point.X <= right)	&&
			(point.X >= left)	&&
			(point.Z <= top)	&&
			(point.Z >= bottom);
}

void Bbox::updateCenter()
{
	x = (right + left) / (float)2;
	y = (top + bottom) / (float)2;
}

void Bbox::addPoint(const float & posX, const float & posY, const float & posZ)
{	
	//if the bounding box hasn't been initialized
	if(!init)
	{
		left = right = posX;
		bottom = top = posZ;
		init = true;
	}
	else
	{
		if(posX < left)
			left = posX;
		else if(posX > right)
			right = posX;

		if(posZ < bottom)
			bottom = posZ;
		else if(posZ > top)
			top = posZ;
	}

	updateCenter();
	
}



void Bbox::displayWall()const
{
	//vector3df color(0.19,0.14,0.15);
	
	glPushMatrix();
		// on deplace le rectangle à la bonne position	
		glTranslatef(x, 2, y);
		
		glScalef(getWidth()*1.5, 4., getHeight()*1.5);
		createWall();
	glPopMatrix();
	
}

void Bbox::displayUpDown()const
{
	//vector3df color(0.19,0.14,0.15);
	
	glPushMatrix();
	// on deplace le rectangle à la bonne position	
	glTranslatef(x, 2, y);
	
	glScalef(getWidth()*1.5, 4., getHeight()*1.5);
	
	textures[3]->bind();
		createRoof();
	textures[3]->unbind();
	
	textures[4]->bind();
	createGround();
	textures[4]->unbind();
	
	glPopMatrix();
	
}


void Bbox::initTextures()
{
   	Texture* textureOne = new Texture(0, "textures/wall.ppm"); 
    textureOne->create(1);
    
    textures.push_back(textureOne);
    
    Texture* textureTwo = new Texture(1, "textures/heightmap.ppm");
    textureTwo->create(1);
    
    textures.push_back(textureTwo);
    
    Texture* textureThree = new Texture(2, "textures/normalmap.ppm");
    textureThree->create(1);
	
		textures.push_back(textureThree);
	
		Texture* roof = new Texture(0, "models/wall/gate_wood.ppm");
    roof->create(1);
	
		textures.push_back(roof);
	
		Texture* ground = new Texture(0, "models/wall/gate_wood.ppm");
    ground->create(1);
	
    textures.push_back(ground);
}


void Bbox::bindTextures()
{
  for(unsigned int i=0; i<3; ++i)
  {
    textures[i]->bind();
  }  
}

void Bbox::unbindTextures()
{
  for(unsigned int i=0; i<3; ++i)
  {
    textures[i]->unbind();
  }  
}
