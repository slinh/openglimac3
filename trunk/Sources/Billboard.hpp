/*
 *  Billboard.h
 *  
 *
 *  Created by Nicolas Martino on 10/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __BILLBOARD_HPP__
#define __BILLBOARD_HPP__

#include "vector3d.h"
#ifdef __APPLE__
#include <openGL/gl.h>
#else
#include <GL/gl.h>
#endif

class BillBoard {
protected:
    vector3df* position; // Position du billboard
    float hsize;	// Hauteur du billboard
    float vsize;	// Largeur du billboard
public:	
        BillBoard(vector3df* p = new vector3df(0.0f,0.0f,0.0f),
			  float h = 1.0f,
			  float v = 1.0f): position(p), hsize(h), vsize(v) {}
	
	virtual ~BillBoard(){}
	
	void updatePosition(const float& dx, const float& dy, const float& dz){ 
               position->X += dx;
               position->Y += dy;
               position->Z += dz;
	}
	
	void updateSize(const float& percent){ 
               vsize *= percent / 100.0f;
               hsize *= percent / 100.0f;
	}
	
        virtual void draw(vector3df camera);
	
};

#endif
