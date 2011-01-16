#include "HeightField.hpp"
#include <stdio.h>
#include <string>
#include <vector>

HeightField::HeightField(std::string hFileName, const int hWidth, const int hHeight) : height(hHeight), width(hWidth), filename(hFileName)
{
	Texture * tmp = new Texture(6, "textures/hmpRGB.ppm");
	
	textures.push_back(tmp);
	
	scale = vector3df(0.05, 0.03, 0.05);
	
}

HeightField::~HeightField()
{
	for( unsigned int i=0; i<textures.size(); ++i )
	{
		delete textures[i];
	}
}

void HeightField::init(void){	

	
	FILE *fp;
	
	fp = fopen(filename.c_str(), "rb");
	fread(hHeightField, 1, width * height, fp);
	fclose(fp);
	
	// load each texture
	for( unsigned int i=0; i<textures.size(); ++i )
	{
		textures[i]->create();
	}
	
	// create displaylist
	displayListId=glGenLists(1);
	if(displayListId==0)
		std::cerr << "erreur d'initialisation de la liste " << std::endl;
	glNewList(displayListId, GL_COMPILE);
		draw();
	glEndList();
	
}

void HeightField::draw(void){
	
	#ifndef __NO_SHADER__  
		//glUseProgramObjectARB(0);
	#endif
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, tID[0]);
	for( unsigned int i=0; i<textures.size(); ++i )
	{
	//	glActiveTexture(GL_TEXTURE0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		textures[i]->bind();
		std::cout<< "on bind cette texture : "<< textures[i]->getFile()<<std::endl;
	}
	
	
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	
	glPushMatrix();
	//X->x, Y->z, Z->y
	glTranslatef(-13., 0, -13.);
	glScalef(scale.X, scale.Y, scale.Z);
	
	
	for (int hMapX = 0; hMapX < width; hMapX++){
        for (int hMapZ = 0; hMapZ < height; hMapZ++){
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f((float)hMapX / width, (float)hMapZ / height);
			glVertex3f(hMapX, hHeightField[hMapX][hMapZ], hMapZ);
			glTexCoord2f((float)hMapX / width, (float)(hMapZ + 1) / height) ;
			glVertex3f(hMapX, hHeightField[hMapX][hMapZ + 1], hMapZ + 1);
			glTexCoord2f((float)(hMapX + 1) / width, (float)hMapZ / height);
			glVertex3f(hMapX + 1, hHeightField[hMapX + 1][hMapZ], hMapZ);
			glTexCoord2f((float)(hMapX + 1) / width, (float)(hMapZ + 1) / height);
			glVertex3f(hMapX + 1, hHeightField[hMapX + 1][hMapZ + 1], hMapZ + 1);
			glEnd();
		}
	}
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void HeightField::display(void){
	glCallList(displayListId);
}
