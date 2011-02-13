#include "Game.hpp"
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
#include "ObjLoader.hpp"
#include "Obj.hpp"
#include "Draw.hpp"
#include "Texture.hpp"

class Game;

Bbox::Bbox()
{
	init = false;
}


Bbox::Bbox(const Bbox & copy)
{

	init = copy.init;
	left = copy.left;
	right = copy.right;
	bottom = copy.bottom;
	top = copy.top;
	x = copy.x;
	y = copy.y;
	
	width = copy.width;
	height = copy.height;
	
	init = copy.init;
	
	for(unsigned int i=0; i<copy.textures.size(); ++i)
	{
		Texture* tmp = copy.textures[i];
		textures.push_back(tmp);
	}
}


Bbox::~Bbox(){}


void Bbox::initialize(ObjLoader* obj)
{
	// apply scale and translate :


	height = top-bottom;
	width = right-left;
}

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


const Texture & Bbox::getTexture(int id)const
{
	return *textures[id];
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



void Bbox::displayWall(TypeWall type)const
{

	switch(type)
	{
		case TOON:
			textures[5]->bind();
		break;
		
		case CLASSIC:
			textures[0]->bind();
			textures[1]->bind();
			textures[2]->bind();
		break;
	}
	
	glPushMatrix();
		// on deplace le rectangle ˆ la bonne position	
		glTranslatef(x, 2, y);
		
		glScalef(getWidth()*1.5, 4., getHeight()*1.5);
		createWall();
	glPopMatrix();
	
	switch(type)
	{
		case TOON:
			textures[5]->unbind();
		break;
		
		case CLASSIC:
			textures[0]->unbind();
			textures[1]->unbind();
			textures[2]->unbind();
		break;
	}
	
	
}

void Bbox::displayUpDown()const
{
	//vector3df color(0.19,0.14,0.15);
	
	glPushMatrix();
	// on deplace le rectangle Ã  la bonne position	
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
		Game & game = Game::Instance();

		TextureLoader* tmp = game.getLoader().getTextureLoader(20);

   	Texture* textureOne = new Texture(0, tmp); 
    
    textures.push_back(textureOne);
    
    tmp = game.getLoader().getTextureLoader(21);
    Texture* textureTwo = new Texture(1, tmp);
    
    textures.push_back(textureTwo);
    
    tmp = game.getLoader().getTextureLoader(22);
    Texture* textureThree = new Texture(2, tmp);
	
		textures.push_back(textureThree);
	
		tmp = game.getLoader().getTextureLoader(23);
		Texture* roof = new Texture(0, tmp);
	
		textures.push_back(roof);
	
		Texture* ground = new Texture(0, tmp);
	
    textures.push_back(ground);
    
    Texture* toonWall = new Texture(29, tmp);
	
    textures.push_back(toonWall);
    
}


void Bbox::bindTextures()
{
	
  for(unsigned int i=0; i<(textures.size()-2); ++i)
  {
    textures[i]->bind();
  }  
}

void Bbox::unbindTextures()
{
  for(unsigned int i=0; i<(textures.size()-2); ++i)
  {
    textures[i]->unbind();
  }  
}
