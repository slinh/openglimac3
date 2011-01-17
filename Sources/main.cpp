#include "main.hpp"
#include "Draw.hpp"
#include <stdlib.h>
#include "Obj.hpp"
#include "common/include/XMLParser.hpp"

#include "Game.hpp"
#include "ppm.hpp"





//#define __NO_SHADER__
#define __CUBE_MAP__
#define __MAIN_SCENE__
//#define __ENV_TEST__
//#define __BUMP_TEST__
//#define __TOON_TEST__
//#define __ALPHA_TEST__
//#define __SHADOW_TEST__
#define CHECK_ERRORS



void checkGLError(int line) {
#ifdef CHECK_ERRORS
 err = glGetError();
 if(err!=GL_NO_ERROR){
   std::cerr << "Erreur GL "<<line<<" :" << std::endl;
   std::cerr << gluErrorString(err) << std::endl;
 }
#endif
}

void mat_inverse (float *in, float *out)
{
  float det, oneOverDet;

  det = in[0] * ((in[5] * in[10]) - (in[6] * in[9]))
    + in[1] * ((in[6] * in[8]) - (in[4] * in[10]))
    + in[2] * ((in[4] * in[9]) - (in[5] * in[8]));

  oneOverDet = 1.0f / det;

  out[0] = ((in[5] * in[10])- (in[6] * in[9])) * oneOverDet;
  out[1] = ((in[2] * in[9]) - (in[1] * in[10]))* oneOverDet;
  out[2] = ((in[1] * in[6]) - (in[2] * in[5])) * oneOverDet;
  out[3] = 0.0f;

  out[4] = ((in[6] * in[8]) - (in[4] * in[10]))* oneOverDet;
  out[5] = ((in[0] * in[10])- (in[2] * in[8])) * oneOverDet;
  out[6] = ((in[2] * in[4]) - (in[0] * in[6])) * oneOverDet;
  out[7] = 0.0f;

  out[8] = ((in[4] * in[9]) - (in[5] * in[8])) * oneOverDet;
  out[9] = ((in[1] * in[8]) - (in[0] * in[9])) * oneOverDet;
  out[10]= ((in[0] * in[5]) - (in[1] * in[4])) * oneOverDet;
  out[11]= 0.0f;

  out[12] = 0.0f;
  out[13] = 0.0f;
  out[14] = 0.0f;
  out[15] = 1.0f;
}

void draw(bool shaders = false)
{
  glMaterialfv(GL_FRONT, GL_SPECULAR,white);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,red);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softred);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);


  if(shaders){
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(2.0,1.0,2.0);
      glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
      glUniformMatrix4fvARB(glGetUniformLocationARB(programobject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
      glPopMatrix();
  }

  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glTranslatef(2.0,1.0,2.0);
  drawSphere(0.5,30,30);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_SPECULAR,white);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,red);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softred);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);


  if(shaders){
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(-1.0,-0.33,-1.0);
      glRotatef(angle,1.0,0.0,0.0);
      glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
      glUniformMatrix4fvARB(glGetUniformLocationARB(programobject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
      glPopMatrix();
  }
  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glTranslatef(-1.0,-0.33,-1.0);
  glRotatef(angle,1.0,0.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();


  glMaterialfv(GL_FRONT, GL_SPECULAR,gray);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,blue);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softblue);
  glMaterialf( GL_FRONT, GL_SHININESS, 60.0f);

  if(shaders){
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(0.0,-1.0,0.0);
      glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
      glUniformMatrix4fvARB(glGetUniformLocationARB(programobject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
      glPopMatrix();
      }

  glTranslatef(0.0,-1.0,0.0);
  glPushMatrix();
  glBegin(GL_QUADS);
  glColor3f(1.0f,1.0f,1.0f);
  glNormal3f(0.0f,1.0f,0.0f);
  glVertex3f(-20.0f, 0.0f, 20.0f);
  glVertex3f( 20.0f, 0.0f, 20.0f);
  glVertex3f( 20.0f, 0.0f,-20.0f);
  glVertex3f(-20.0f, 0.0f,-20.0f);
  glEnd();
  glPopMatrix();
}

bool checkFramebufferStatus(void)
{
  GLenum status;
  status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  switch(status) {

  case GL_FRAMEBUFFER_COMPLETE_EXT:
    return true;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
    std::cerr << "FrameBuffer Status Error : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT" << std::endl;
    return false;
    break;

  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
    std::cerr << "FrameBuffer Status Error : GL_FRAMEBUFFER_UNSUPPORTED_EXT" << std::endl;
    return false;
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
    std::cerr << "FrameBuffer Status Error : GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT" << std::endl;
    return false;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
    std::cerr << "FrameBuffer Status Error : GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" << std::endl;
    return false;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
    std::cerr << "FrameBuffer Status Error : GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" << std::endl;
    return false;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
    std::cerr << "FrameBuffer Status Error : GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT" << std::endl;
    return false;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
    std::cerr << "FrameBuffer Status Error : GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT" << std::endl;
    return false;
    break;

  default:
    std::cerr << "FrameBuffer Status Error : unknown error" << std::endl;
    return false;

  }
}

void multMatrix4x4(float* m1, float* m2, float* res)
{
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(m1);
    glMultMatrixf(m2);
    glGetFloatv(GL_MODELVIEW_MATRIX, res);
    glPopMatrix();
}


static void displayGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  glRotatef(xrotation,1.0f,0.0f,0.0f);
  glRotatef(yrotation,0.0f,1.0f,0.0f);
  glTranslatef(-position[0],-position[1],-position[2]);
    
  glPushMatrix();
//glRotatef(angle,0.0,0.0,1.0);
//  glLightfv(GL_LIGHT0, GL_POSITION,lightPosition);

//   glLightfv(GL_LIGHT0, GL_POSITION, game.getLightPosition());
//   std::cout << "lightPos" <<  game.getLightPosition()[0] << ", " << game.getLightPosition()[1] << ", " << game.getLightPosition()[2] << std::endl;
    
  glPopMatrix();
  
  
  GLfloat global_ambient[] = { 1.0f, 0.0f, 0.0f, 1.0f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  
  // See where the light is
  glPushMatrix();
     glTranslatef(game.getLightPosition()[0],game.getLightPosition()[1],game.getLightPosition()[2]);
     drawSphere(0.01, 30, 30);
     std::cout << "lightPos :" << game.getLightPosition()[0] << " / " << game.getLightPosition()[1] << " / " << game.getLightPosition()[2] << " /"  << game.getLightPosition()[3]<< std::endl;     
  glPopMatrix();

/*  
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,black);
  glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 10.0f);
*/

#ifdef __CUBE_MAP__ // --- CUBEMAP

//glActiveTexture(GL_TEXTURE3);
//glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getIdTex());

#ifndef __NO_SHADER__  
  glUseProgramObjectARB(programobject[CUBEMAP]);
  
  glUniform1i(glGetUniformLocationARB(programobject[CUBEMAP] ,"id_tex"), cubeMap->getIdTex());
  
#endif
  
  //glBindTexture(GL_TEXTURE_CUBE_MAP,texturesid[3]);
  //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  cubeMap->display();
  

  //HeightField
//  glScalef(0.002, 0.002, 0.002);
//  hField.Render();	
	
  //cubeMap->drawSphere(4.0,50,50);
  
 	
#ifndef __NO_SHADER__  
  glUseProgramObjectARB(0);
#endif
  
#endif // --- END CUBEMAP 

#ifdef __MAIN_SCENE__  // MAIN SCENE - FROM XML - + NORMAL SPEC ILLUMINATION
#ifndef __NO_SHADER__  

/*
  glUseProgramObjectARB(programobject[NORMALSPEC]);

  glUniform1i(glGetUniformLocationARB(programobject[NORMALSPEC], "diffuseTexture"), 0);
  glUniform1i(glGetUniformLocationARB(programobject[NORMALSPEC], "normalMap"), 1);
  glUniform1i(glGetUniformLocationARB(programobject[NORMALSPEC], "specularMap"), 2);
  glUniform1f(glGetUniformLocationARB(programobject[NORMALSPEC], "shininess"), .2);
  */
#endif

// Game & game = Game::Instance();
vector3df cam = vector3df(position[0], position[1], position[2]);
game.setCamera() = cam;
game.checkCurrentScene();
game.display();


#ifndef __NO_SHADER__  
  glUseProgramObjectARB(0);
#endif

#endif  // --- END MAIN SCENE


#ifdef __ENV_TEST__  // --- TEST ENV MAPPING
  float mat[16];
  float invmat[16];
  glGetFloatv (GL_MODELVIEW_MATRIX, mat);
  mat_inverse (mat, invmat);

  game.setInvMat(invmat);

#ifndef __NO_SHADER__ 
  glUseProgramObjectARB(programobject[ENV]);

  // CPU => GPU
  glUniformMatrix4fv(glGetUniformLocationARB(programobject[ENV], "matInv"), 1, false, invmat);

  // myTexture => GPU
  glUniform1i(glGetUniformLocationARB(programobject[ENV], "myTexCube"), 0);

  // position => GPU
  glUniform3fv(glGetUniformLocationARB(programobject[ENV], "posCamera"), 1, position);
    
#endif      

  glColor3f(1.0f,1.0f,1.0f);

  glFrontFace(GL_CW);

  glutSolidTeapot(0.2);

  glFrontFace(GL_CCW);

  glTranslatef(0.0,0.5,0.0);

//  drawSphere(0.2,30,30);
  
#ifndef __NO_SHADER__ 
  glUseProgramObjectARB(0);
#endif     

#endif  // --- END ENV MAPPING


#ifdef __TOON_TEST__  // --- TEST TOON SHADING
#ifndef __NO_SHADER__ 
  glUseProgramObjectARB(programobject[TOON]);
#endif  
  glColor3f(1.0f,1.0f,1.0f);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);
  glDepthFunc(GL_LESS);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
  glutSolidTeapot(1.);
  glFrontFace(GL_CCW);
#ifndef __NO_SHADER__  
  glUseProgramObjectARB(0);
#endif
  glLineWidth(3.0);
  glDisable(GL_LIGHTING);
  glPolygonMode(GL_BACK, GL_LINE);
  glDepthFunc(GL_LEQUAL);
  glCullFace(GL_FRONT);
  glColor3f(0.0,0.0,0.0);
  glFrontFace(GL_CW);
  glutSolidTeapot(1.0);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);
#ifndef __NO_SHADER__  
  glUseProgramObjectARB(0);
#endif
#endif  // --- END TEST TOON SHADING

#ifdef __ALPHA_TEST__  // --- TEST ALPHA SHADING
#ifndef __NO_SHADER__ 
  glUseProgramObjectARB(programobject[ALPHA]);
  //glUniform1i(glGetUniformLocationARB(programobject[ALPHA], "colorMap"), 0);
  glUniform1i(glGetUniformLocationARB(programobject[ALPHA], "alphaMap"), idTexAlpha);

#endif

  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, idTexAlpha);
//  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glutSolidTeapot(1.);
 /* glBegin( GL_QUADS );
    glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
    glTexCoord2d(1.0,0.0); glVertex2d(1.0,0.0);
    glTexCoord2d(1.0,1.0); glVertex2d(1.0,1.0);
    glTexCoord2d(0.0,1.0); glVertex2d(0.0,1.0);
  glEnd();*/
  glDisable(GL_TEXTURE_2D);
  
#ifndef __NO_SHADER__  
  glUseProgramObjectARB(0);
#endif
#endif  // --- END TEST ALPHA SHADING

#ifdef __SHADOW_TEST__
  // Premiere passe en FBO
  if(shadowbufferid!=0)
  {
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, shadowbufferid);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    checkFramebufferStatus();
    checkGLError(417);

    glPushMatrix();
    glLoadIdentity();

    gluLookAt(lightPosition[0], lightPosition[1], lightPosition[2],
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    glRotatef(-angle,0.0,1.0,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightmodelviewmatrix);

    multMatrix4x4(lightprojectionmatrix,lightmodelviewmatrix,shadowmatrix);

    checkGLError(434);
    multMatrix4x4(biasmatrix,shadowmatrix,shadowmatrix);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(4.0,4.0);
    draw(false);

    glDisable(GL_POLYGON_OFFSET_FILL);

    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);

    glPopMatrix();
  }

  glPushMatrix();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glRotatef(xrotation,1.0f,0.0f,0.0f);
  glRotatef(yrotation,0.0f,1.0f,0.0f);
  glTranslatef(-position[0],-position[1],-position[2]);

  glPushMatrix();
  glRotatef(angle,0.0,1.0,0.0);
  glLightfv(GL_LIGHT0, GL_POSITION,lightPosition);
  glPopMatrix();

  glPushMatrix();
  glRotatef(angle,0.0,1.0,0.0);
  glTranslatef(lightPosition[0],lightPosition[1],lightPosition[2]);

  glutSolidSphere(0.5,30,30);
  glPopMatrix();

  glActiveTexture(GL_TEXTURE0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D,shadowtexid);

  if(programobject!=0){
      glUseProgramObjectARB(programobject[SHADOW]);
      glUniformMatrix4fvARB(glGetUniformLocationARB(programobject[SHADOW],"eyematrix"),1,GL_FALSE,shadowmatrix);
      glUniform1iARB(glGetUniformLocationARB(programobject[SHADOW],"shadowmap"),0);
  }

  draw(true);
  glUseProgramObjectARB(0);

#endif

  checkGLError(458);

  glPopMatrix();

  glutSwapBuffers();

}

static void reshapeGL(int newwidth,
          int newheight)
{
  ratio = (float)newwidth/(float)newheight;
  glViewport(0, 0, (GLint)newwidth, (GLint)newheight);

  windowwidth = newwidth;
  windowheight = newheight;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);

  glGetFloatv(GL_PROJECTION_MATRIX, lightprojectionmatrix);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

/**
 * casteljau : implantation de l'algo de Casteljau
 * Copyright (C) 2002 Stephane Marchesin
**/
vector3df casteljau(float t,std::vector<vector3df> l)
{
	std::vector<vector3df> liste_points=l;
	std::vector<vector3df> nouvelle_liste_points;

	do
	{
		for(unsigned int i=0;i<liste_points.size()-1;i++)
		{
			nouvelle_liste_points.push_back(liste_points[i]*(1.0f-t)+liste_points[i+1]*t);
		}
		liste_points=nouvelle_liste_points;
		nouvelle_liste_points.clear();
	}
	while(liste_points.size()>1);
	
	if (liste_points.size()!=1)
	{
		printf("Erreur !\n");
	}

	return liste_points[0];
}


static void idleGL(void)
{
    angle += 0.25;

   	if(f<=1.){
			vector3df p=casteljau(f,controlPoints);
			position[0]=p.X;
			position[1]=p.Y;
			position[2]=p.Z;
			f+=1./(float)nbPoints;
			sleep(0.1);
		}

    
    glutPostRedisplay();
}

static void quit(void)
{
  exit(0);
}

static void keyboardGL(unsigned char c, 
		       int x, 
		       int y){ 
  float step = 0.05f;
  float x1,z1;
  switch(c){ 
  case 27:
    quit();
    break;
    
   case 'y': // light up
    game.lightUp();
   break; 
   case 'h': // light down
    game.lightDown();
   break; 
   case 'g': // light left
    game.lightLeft();
   break;
   case 'j': // light right
    game.lightRight();
   break; 
   case 'i': // light left
    game.lightFront();
   break;
   case 'k': // light right
    game.lightBack();
   break;  
    
    
  case 'z' :
    position[0] += step*direction[0];
    position[2] += step*direction[2];
    break;
  case 's' : 
    position[0] -= step*direction[0];
    position[2] -= step*direction[2];
    break;
  case 'q' :
    x1 = -direction[2];
    z1 = direction[0];
    position[0] -= step*x1;
    position[2] -= step*z1;
    break;
  case 'd' :
    x1 = -direction[2];
    z1 = direction[0];
    position[0] += step*x1;
    position[2] += step*z1;
	break;
  case 'r':
	scale += 0.02;
    break;
  case 'f':
	scale -= 0.02;
    break;
  case 'p' :
	position[1] += step;
	direction[1] += step;
	break;
  case 'l' :
	position[1] -= step;
	direction[1] -= step;
	break;
  case 'w':
    GLint wtype[2];
    glGetIntegerv(GL_POLYGON_MODE,wtype);
    if(wtype[0]==GL_FILL)
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  default:
    break;
  }
  glutPostRedisplay();
}

static void keyboardSpecialGL(int c, int x, int y)
{ 

  float step = 0.05f;
  float x1,z1;
  switch(c){
  case GLUT_KEY_UP :
    position[0] += step*direction[0];
    position[2] += step*direction[2];
    break;
  case GLUT_KEY_DOWN : 
    position[0] -= step*direction[0];
    position[2] -= step*direction[2];
    break;
  case GLUT_KEY_LEFT :
    x1 = -direction[2];
    z1 = direction[0];
    position[0] -= step*x1;
    position[2] -= step*z1;
    break;
  case GLUT_KEY_RIGHT :
    x1 = -direction[2];
    z1 = direction[0];
    position[0] += step*x1;
    position[2] += step*z1;
    break;
  default:
    break;
  }

  glutPostRedisplay();

}

static void mouseGL(int button, int state, int x, int y)
{

  if(button == int(GLUT_LEFT_BUTTON) && state == int(GLUT_DOWN)){
    xmouseposition = x;
    ymouseposition = y;
    buttonpressed = 1;
  }

  if(button == int(GLUT_LEFT_BUTTON) && state == int(GLUT_UP)){
    buttonpressed = 0;
  }

  glutPostRedisplay();

}


static void motionGL(int x, int y)
{

  if(buttonpressed){
    yrotation += (x - xmouseposition)/10.0f;
    xrotation += (y - ymouseposition)/10.0f;
    aim[0]=sin(yrotation*M_PI/180.0f);
    aim[1]=-sin(xrotation*M_PI/180.0f);
    aim[2]=-cos(yrotation*M_PI/180.0f);
    direction[0]=sin(yrotation*M_PI/180.0f);
    direction[2]=-cos(yrotation*M_PI/180.0f);
    xmouseposition = x;
    ymouseposition = y;
  }

  glutPostRedisplay();

}


void loadShader(std::string vert, std::string frag, int idShader)
{
	if(!isExtensionSupported("GL_ARB_shading_language_100"))
		quit();

	GLhandleARB so[2];
	bzero(so,sizeof(GLhandleARB)*2);

	// std::string s1 = "shaders/foo.vert";

	so[0] = loadShader(vert.c_str());
	if(so[0]==0){
		std::cerr << "loading shader "+vert+" failed (exiting...)" << std::endl;
		quit();
	}
	if(!compileShader(so[0])){
		std::cerr << "compiling shader "+vert+" failed (exiting...)" << std::endl;
		quit();
	}

	// std::string s2 = "shaders/foo.frag";
	so[1] = loadShader(frag.c_str());
	if(so[0]==0){
		std::cerr << "loading shader "+frag+" failed (exiting...)" << std::endl;
		quit();
	}
	if(!compileShader(so[1])){
		std::cerr << "compiling shader "+frag+" failed (exiting...)" << std::endl;
		quit();
	}
	programobject[idShader] = linkShaders(so,2);
}

void loadTexture(std::string file, GLuint idTex)
{
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

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE,image );

  delete[] image;

}

static void initGL(int argc, 
		   char **argv)
{

  glClearColor(0.8f,0.8f,0.8f,1.0f);

#ifdef __MAIN_SCENE__
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
#endif

#ifndef __NO_SHADER__
  // load all shader programs  
  for(unsigned int i=0; i < NB_SHADERS; ++i)
  { 
    loadShader(tabShader[i]+".vert", tabShader[i]+".frag", i);
  }
#endif
 
#ifdef __MAIN_SCENE__

checkGLError(711);


glLightfv(GL_LIGHT0, GL_SPECULAR, grey);
glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
glLightfv(GL_LIGHT0, GL_DIFFUSE, white);


glPixelStorei(GL_UNPACK_ALIGNMENT,1);
glPixelStorei(GL_PACK_ALIGNMENT,1);


//  Game & game = Game::Instance();
  game.setProgramObject(programobject);
  game.initGL();
#endif

#ifdef __CUBE_MAP__
  //Creation de la cube map
		
  // sky cubemap
  cubeMap=new CubeMap();

  //glBindTexture(GL_TEXTURE_CUBE_MAP, texturesid[3]);
  cubeMap->initCubeMap(
   "textures/greenhill_negative_x.ppm",
   "textures/greenhill_negative_z.ppm",
   "textures/greenhill_negative_y.ppm",
   "textures/greenhill_positive_x.ppm",
   "textures/greenhill_positive_y.ppm",
   "textures/greenhill_positive_z.ppm");

  // church cubemap
  /*church=new CubeMap();
  church->initCubeMap(
   "textures/church/negative_x.ppm",
   "textures/church/negative_z.ppm",
   "textures/church/negative_y.ppm",
   "textures/church/positive_x.ppm",
   "textures/church/positive_y.ppm",
   "textures/church/positive_z.ppm");*/
    game.setChurch(church);


  //HeightField Map init
//  glBindTexture(GL_TEXTURE_CUBE_MAP, texturesid[3]);
//  hField.Create((char *)"textures/heightfieldBW.ppm", 512, 512);
    
/*
 // load model file
  objfile = new Obj();
  
 objfile->ReadOBJModel("models/house_obj1/house_obj.obj");
 */
 
 //std::cout << "load obj ok" << std::endl;
#endif

#ifdef __ALPHA_TEST__
  glGenTextures(0,&idTexAlpha);
  glBindTexture (GL_TEXTURE_2D, idTexAlpha);
  unsigned int tmpwidth, tmpheight;
  unsigned char * image = loadPPM("textures/alpha_map.ppm",tmpwidth,tmpheight);
  if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);

  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE,image );

  delete[] image;

#endif // --- END TEST ALPHA

#ifdef __SHADOW_TEST__
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glGenTextures (1, (GLuint *)&shadowtexid);
  glBindTexture (GL_TEXTURE_2D, shadowtexid);
  glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,
               windowwidth,windowheight,0,
               GL_DEPTH_COMPONENT,
               GL_UNSIGNED_BYTE,0);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  // Fonctions très imortantes pour faire la comparaison de profondeur
  // avec la fonction glsl shadow2D
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

  glGenFramebuffersEXT (1, (GLuint *)&shadowbufferid);
  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, shadowbufferid);

  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D,shadowtexid, 0);

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glDepthMask( GL_TRUE );

  if(!checkFramebufferStatus())
   quit();

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  glDrawBuffer(GL_BACK);
  glReadBuffer(GL_FRONT);

  checkGLError(849);
  
  glLightfv(GL_LIGHT0, GL_SPECULAR,white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,white);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);

  glBindTexture (GL_TEXTURE_2D, 0);
  checkGLError(849);
#endif

	nbPoints = 32;
	f=0;
	vector3df pointTmp= vector3df(5.,0.5,2.0);
	vector3df point2= vector3df(8., 0.5, -2.0);
	vector3df point3= vector3df(-1., 0.5, -2.);
	vector3df cam = vector3df(position[0], position[1], position[2]);
	controlPoints.push_back(cam);
	controlPoints.push_back(pointTmp);
	controlPoints.push_back(point2);
	controlPoints.push_back(point3);

}

int main(int argc, char **argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowPosition(windowx, windowy);
  glutInitWindowSize(windowwidth, windowheight);
 
  
  if(glutCreateWindow("Bossoutrot - Jard - Linh - Martino : Epic IMAC") == 0){
    return 1;
  }

  glewInit();

  glutReshapeFunc(reshapeGL);
  glutDisplayFunc(displayGL);
  glutIdleFunc(idleGL);
  glutKeyboardFunc(keyboardGL);
  glutSpecialFunc(keyboardSpecialGL);
  glutMouseFunc(mouseGL);
  glutMotionFunc(motionGL);

  initGL(argc, argv);

  glutMainLoop();

  return 0;
}

