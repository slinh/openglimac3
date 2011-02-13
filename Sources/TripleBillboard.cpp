/*
 *  TripleBillboard.cpp
 *  
 *
 *  Created by Nicolas Martino on 09/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TripleBillboard.hpp"
#include <iostream>
#ifdef __APPLE__

        #include <GLUT/glut.h>
#else
        #include <GL/glut.h>
#endif

void TripleBillBoard::draw(vector3df camera){
	
	float x = hsize / 2.0f; 
	float y = vsize / 2.0f;
	//Premier billboard
	glPushMatrix();
               glTranslatef(position->X, position->Y, position->Z);

               std::cout << "x : " << position->X << " y :" << position->Y << "z : " << position->Z << std::endl;
                glPushMatrix();
                    glBegin(GL_QUADS);
                        glNormal3f(0.0f,0.0f,1.0f);
                        glTexCoord2f(0.0f, 0.0f);
                        glVertex3f(-x, y, 0.0f);
                        glTexCoord2f(0.0f, 1.0f);
                        glVertex3f(-x,-y, 0.0f);
                        glTexCoord2f(1.0f, 1.0f);
                        glVertex3f( x,-y, 0.0f);
                        glTexCoord2f(1.0f, 0.0f);
                        glVertex3f( x, y, 0.0f);
                    glEnd();
                glPopMatrix();

                glPushMatrix();
                glTranslatef(0.2, 0., 0.3);
                glRotatef(60,0,1,0);

                glBegin(GL_QUADS);
                    glNormal3f(0.0f,0.0f,1.0f);
                    glTexCoord2f(0.0f, 0.0f);
                     glVertex3f(-x, y, 0.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(-x,-y, 0.0f);
                    glTexCoord2f(1.0f, 1.0f);
                     glVertex3f( x,-y, 0.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f( x, y, 0.0f);
                glEnd();
                glPopMatrix();

                glPushMatrix();
                glTranslatef(-0.2, 0., 0.3);
                glRotatef(-60,0,1.,0);
                glBegin(GL_QUADS);
                    glNormal3f(0.0f,0.0f,1.0f);
                    glTexCoord2f(0.0f, 0.0f);
                     glVertex3f(-x, y, 0.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(-x,-y, 0.0f);
                    glTexCoord2f(1.0f, 1.0f);
                     glVertex3f( x,-y, 0.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f( x, y, 0.0f);
                glEnd();
                glPopMatrix();

        glPopMatrix();

        /*
	//Deuxième Billboard
        glPushMatrix();
               glTranslatef(position->X, position->Y, position->Z);
                glRotatef(90,0,1,0);
		glBegin(GL_QUADS);
                        glNormal3f(0.0f,0.0f,1.0f);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(-x, y, 0.0f);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(-x,-y, 0.0f);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f( x,-y, 0.0f);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f( x, y, 0.0f);
                glEnd();

                glRotatef(180,0,1,0);
                glBegin(GL_QUADS);
                    glNormal3f(0.0f,0.0f,1.0f);
                    glTexCoord2f(0.0f, 0.0f);
                     glVertex3f(-x, y, 0.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(-x,-y, 0.0f);
                    glTexCoord2f(1.0f, 1.0f);
                     glVertex3f( x,-y, 0.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f( x, y, 0.0f);
                glEnd();
        glPopMatrix();
        */
}

