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

#include "Texture.hpp"
#include "ppm.hpp"

Texture::~Texture()
{

}

void Texture::create(int repeat)
{
	glGenTextures(1,&idTex);

	// Texture init ok
	if(idTex == 0){
		std::cerr << "Identifiant de texture incorrect" << std::endl;
		return;
	}

	// bind for params
    glBindTexture(GL_TEXTURE_2D,idTex);

	unsigned int tmpwidth, tmpheight;
	unsigned char * image = loadPPM(file.c_str(), tmpwidth,tmpheight);
	if(image==0){
		std::cerr << "Erreur au chargement le l'image" << std::endl;
		return;
	}
  
	if( repeat == 1 )
	{
		/* Params texture smashtein
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		**/
		
//    std::cout << "texture avec repeat" << std::endl;
    
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE,image );
	}

	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE,image );
	}

	delete[] image;
	glBindTexture(GL_TEXTURE_2D,0);
}

void Texture::bind()const
{
	
  switch(id)
  {
	  case 0:
	  	glActiveTexture(GL_TEXTURE0);
	    break;
	  
	  case 1:
	  	glActiveTexture(GL_TEXTURE1);
	    break;
	  
	  case 2:
	  	glActiveTexture(GL_TEXTURE2);
	    break;
		  
	  case 3:
		  glActiveTexture(GL_TEXTURE3);
	    break;
      
    case 4:
		  glActiveTexture(GL_TEXTURE4);
	    break;
      
    case 5:
		  glActiveTexture(GL_TEXTURE5);
	    break;
  }	
	
	glBindTexture(GL_TEXTURE_2D,idTex);
}


void Texture::unbind()const
{
	
  switch(id)
  {
	  case 0:
	  	glActiveTexture(GL_TEXTURE0);
	    break;
	  
	  case 1:
	  	glActiveTexture(GL_TEXTURE1);
	    break;
	  
	  case 2:
	  	glActiveTexture(GL_TEXTURE2);
	    break;
		  
	  case 3:
		  glActiveTexture(GL_TEXTURE3);
	    break;
      
    case 4:
		  glActiveTexture(GL_TEXTURE4);
	    break;
      
    case 5:
		  glActiveTexture(GL_TEXTURE5);
	    break;
  }	
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
}


void Texture::tryCube(){

	
	//application de la texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idTex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
    glTranslatef(-10,-10,0.0f);
	glColor3ub(0,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.f,0.0f);	
		glVertex3f(0.,0.,0.);
		
		glTexCoord2f(1.f,0.0f);
		glVertex3f(20,0.,0.);

		glTexCoord2f(1.f,1.0f);
		glVertex3f(20,20,0.);

		glTexCoord2f(0.f,1.0f);
		glVertex3f(0.,20,0.);
	glEnd();	
  glDisable(GL_TEXTURE_2D);

}
