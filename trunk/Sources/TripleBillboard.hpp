/*
 *  TripleBillboard.hpp
 *  
 *
 *  Created by Nicolas Martino on 09/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __TRIPLEBILLBOARD_HPP__ 
#define __TRIPLEBILLBOARD_HPP__

#include <Billboard.hpp>
#ifdef __APPLE__
#include <openGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "vector3d.h"

class TripleBillBoard : public BillBoard {
public:
        TripleBillBoard(vector3df* p = new vector3df(0.0f,0.0f,0.0f),
					float h = 1.0f,
					float v = 1.0f): BillBoard(p,h,v) {}
	
        ~TripleBillBoard(){}
	
        void draw(vector3df camera);
	
};

#endif
