#ifndef __BBOX_HPP__
#define __BBOX_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "Texture.hpp"
#include "vector3d.h"

class Obj;

class Bbox
{
	protected:
		float left;	 	// left border
		float right;	// right border
		float bottom;	// bottom border
		float top;		// top border
		float x;		// x position
		float y;		// y position
		
		float width;
		float height;
		
		bool init;	
		
		static std::vector<Texture*>	textures;	
				
	public:
		Bbox();
		~Bbox();
		
	//	float getWidth()const;
	//	float getHeight()const;		
		void initialize(Obj* obj);
		
		bool intersect(const Bbox & box)const;
		bool intersect(const vector3df point)const;
		void updateCenter();
		void addPoint(const float & posX, const float & posY, const float & posZ);
		void displayWall()const;
		void displayUpDown()const;

		inline const float & getX()const { return x; };
		inline const float & getY()const { return y; };
	
		inline const float & getWidth()const { return width; };
		inline const float & getHeight()const { return height; };
    
		static void initTextures();
		static void bindTextures();
		static void unbindTextures();
		
};

#endif
