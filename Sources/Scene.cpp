#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

#include "Scene.hpp"
#include "HeightField.hpp"
#include "Draw.hpp"


Scene::Scene(TypeScene type):typeS(type), typeShader(PARALLAX), objList(NULL) 
{
	
	contentHouse = NULL;
	
	if(typeS==MAIN)
	{
		heightfield = new HeightField("textures/hmpBW.raw", 512, 512);
	}
	else
	{
		heightfield = NULL;
	}
  
  lightPosition = new GLfloat[4];
  lightPosition[0] = 0.;
  lightPosition[1] = 5.;
  lightPosition[2] = 2.;
  lightPosition[3] = 1.;
}

Scene::~Scene(){

    contentHouse = NULL;  

	if( heightfield != NULL)
		delete heightfield;
}

void Scene::initGL()
{
	if(typeS == MAIN)
	{
		heightfield->init();
	}
	
	for(unsigned int i=0; i<objList.size(); ++i)
	{
		objList[i]->initGL();
	}
}

void Scene::display()
{

	if(typeS == MAIN)
	{
    	heightfield->display();
	}
	
	// display all obj :
	for(unsigned int i=0; i<objList.size(); ++i)
	{	
	
		if( objList[i]->getType() == SETTING && typeS == MAIN)
		{
			glUseProgramObjectARB(0);
		}
		
		glPushMatrix();
        objList[i]->display();
		glPopMatrix();
		
		//objList[i]->setBbox().display();
	}
}

void Scene::setLightPosition(float x, float y, float z, float w )
{
  lightPosition[0] = (GLfloat)x; 
  lightPosition[1] = (GLfloat)y;
  lightPosition[2] = (GLfloat)z;
  lightPosition[3] = (GLfloat)w;
}
