#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#ifdef __APPLE__
	#include "glew.h"
	#include <openGL/gl.h>
	#include <GLUT/glut.h>
    #include <OpenGL/glu.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
	#include <GL/glut.h>
	#include <GL/glu.h>
#endif
#include <iostream>
#include <sstream>

#include <cmath>

#include "Shaders.hpp"
#include "Obj.hpp"
#include "CubeMap.hpp"
#include "Scene.hpp"
#include "Game.hpp"


// Origine de la fenetre
static const unsigned int windowx = 0;
static const unsigned int windowy = 0;

// Dimensions de la fenetre
static unsigned int windowwidth  = 800;
static unsigned int windowheight = 600;

GLenum err;

// Modele de la camera
static float position[] = {0.0f,0.5f,3.0f};
static float aim[] = {0.0f,0.0f,-1.0f};
static float direction[] = {0.0f,0.0f,-1.0f};

// Rotation de la camera
static float yrotation = 0.0f;
static float xrotation = 0.0f;

// Données sur le curseur et la souris
static int xmouseposition = 0;
static int ymouseposition = 0;
static bool buttonpressed = false;

// lights
GLfloat lightPosition[] = {0.0f,5.0f,5.0f,1.0f};
GLfloat lightprojectionmatrix[16];
GLfloat lightmodelviewmatrix[16];
GLfloat transformationmatrix[16];
GLfloat shadowmatrix[16];
static GLfloat biasmatrix[16] =
{
  0.5f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.5f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f
};

static GLfloat white[]= { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat gray[]= { 0.5f, 0.5f, 0.5f, 1.0f };
//static GLfloat black[]= { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat red[]= { 1.0f, 0.0f, 0.0f, 1.0f };
static GLfloat softred[]= { 0.2f, 0.0f, 0.0f, 1.0f };
static GLfloat blue[]= { 0.0f, 0.0f, 1.0f, 1.0f };
static GLfloat softblue[]= { 0.0f, 0.0f, 0.2f, 1.0f };
static GLfloat black[]= { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat grey[]= { .5f, .5f, .5f, 1.0f };
//static GLfloat red[]= { 1.0f, 0.0f, 0.0f, 1.0f };

// FBO
unsigned int shadowbufferid = 0;

float scale = 0.04;
Obj* objfile;

float angle = 0.0f;
float ratio;

// SphereMap
CubeMap * cubeMap;
CubeMap * church;

// textures
static const unsigned int nbtextures = 5;
GLuint texturesid[nbtextures];
GLuint shadowtexid;

// Shaders
GLhandleARB programobject[NB_SHADERS];
std::string tabShader[NB_SHADERS] = { "shaders/normalspec",
                                      "shaders/parallax",
                                      "shaders/env",
                                      "shaders/cubemap",
                                      "shaders/toon",
                                      "shaders/alpha",
                                      "shaders/shadow"
                                    };
									
									
Scene* scene = NULL;
Game & game = Game::Instance();									
									
#endif