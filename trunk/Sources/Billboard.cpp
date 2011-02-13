/*
 *  Billboard.cpp
 *  
 *
 *  Created by Nicolas Martino on 10/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Billboard.hpp"

void BillBoard::draw(vector3df camera){
	float x = hsize / 2.0f; 
	float y = vsize / 2.0f;
	
	glPushMatrix();
                glTranslatef(position->X, position->Y, position->Z);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, y, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,-y, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( x,-y, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( x, y, 0.0f);
		glEnd();
	glPopMatrix();
}
