#include "main.hpp"
#include "Draw.hpp"
#include <stdlib.h>
#include "Obj.hpp"
#include "common/include/XMLParser.hpp"
#include "Timer.hpp"
#include "Game.hpp"
#include "ppm.hpp"
#include "utils.hpp"



//#define __NO_SHADER__
#define __CASTELJAU__
#define __CUBE_MAP__
#define __MAIN_SCENE__
//#define __TEST_TEXTURE__ 
//#define __ENV_TEST__
//#define __BUMP_TEST__
//#define __TOON_TEST__
//#define __ALPHA_TEST__
#define CHECK_ERRORS


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


static void displayGL(void)
{
#ifndef __NO_SHADER__
  if(game.getSceneList()[game.getCurrentScene()]->getTypeShader() == SHADOW)
  {
   game.FBO();
  }
#endif

#ifdef __MAIN_SCENE__  // MAIN SCENE - FROM XML - + NORMAL SPEC ILLUMINATION
 // glClearColor (0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);

  glViewport(0, 0, windowwidth, windowheight);
  glLoadIdentity();

  glPushMatrix();
  gluLookAt(position[0],position[1],position[2],position[0]+direction[0],position[1]+direction[1],position[2]+direction[2],up[0],up[1],up[2]);

//  gluLookAt(game.getLightPosition()[0],game.getLightPosition()[1]+1.0,game.getLightPosition()[2],
//            0.0,0.0,0.0,
//            up[0],up[1],up[2]);


  //glRotatef(xrotation,1.0f,0.0f,0.0f);
  //glRotatef(yrotation,0.0f,1.0f,0.0f);
  //glTranslatef(-position[0],-position[1],-position[2]);



/*
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(1.0f,0.0f,0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glEnd();


  glBegin(GL_LINES);
  glColor3f(0.0f,1.0f,0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 1.0f);
  glEnd();

  glBegin(GL_LINES);
  glColor3f(0.0f,0.0f,1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 1.0f, 0.0f);
  glEnd();
 glEnable(GL_LIGHTING);
 */



  vector3df cam = vector3df(position[0], position[1], position[2]);
  vector3df dir = vector3df(direction[0], direction[1], direction[2]);

  game.setCamera() = cam;
  game.setDirection() = dir;

  game.checkCurrentScene();

  game.display();

#ifdef __TEST_TEXTURE__  
  glPushMatrix();
    glTranslatef(-1.0,1.0,2.0);

 //   glUseProgramObjectARB(0);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture (GL_TEXTURE_2D, idTexAlpha);
      glTexEnvf(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE0);
  glTexEnvf(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
  
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
    
	  
    drawSphere(2.2,30,30);
    
 //  glutSolidSphere(2, 30 ,30);
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_TEXTURE_2D);

  glPopMatrix();
#endif

checkGLError(332);

glPopMatrix();

//glutSwapBuffers();

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

  // Affichage du texte
  glColor3f(1.0f, 1.0f, 1.0f);
  std::stringstream strStream1;
  strStream1 << Timer::getInstance().getFPSCounter() << " FPS";
  drawString(1.0f, (float)windowheight - 20.0f, GLUT_BITMAP_HELVETICA_18, strStream1.str());



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

  glGetFloatv(GL_PROJECTION_MATRIX, game.setLightprojectionmatrix());

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

/**
 * casteljau : implantation de l'algo de Casteljau
 * Copyright (C) 2002 Stephane Marchesin
**/
vector3df casteljau(float t,std::vector<vector3df> l)
{
	std::vector<vector3df> list_points=l;
	std::vector<vector3df> new_list_points;

	do
	{
		for(unsigned int i=0;i<list_points.size()-1;i++)
		{
			new_list_points.push_back(list_points[i]*(1.0f-t)+list_points[i+1]*t);
		}
		list_points=new_list_points;
		new_list_points.clear();
	}
	while(list_points.size()>1);
	
	if (list_points.size()!=1)
	{
		printf("Erreur !\n");
	}

	return list_points[0];
}


static void idleGL(void)
{
	game.idleGL();

    angle += 0.25;
    
    //Timer
    if(game.getTimer()>1.) facteur=-1.; 
    if(game.getTimer()<0.) facteur=1.;

    game.setTimer() +=facteur * 0.006;
    //Récupération de l'instance du timer pour le FPS
    Timer& timer = Timer::getInstance();
    timer.Idle();
    //float fElapsedTime = 1.0f * timer.getElapsedTime();

	#ifdef __CASTELJAU__

   	if(f<=1.){
			vector3df p=casteljau(f,controlPoints);
			vector3df d=casteljau(f, aimPoints);
			position[0]=p.X;
			position[1]=p.Y;
			position[2]=p.Z;
			//std::cout << "pos : "<<p.X<<" , "<<p.Y<<" , "<<p.Z<<std::endl;


			direction[0]=d.X-p.X;
    	direction[1]=d.Y-p.Y;
    	direction[2]=d.Z-p.Z;
			//std::cout << "aim : "<<d.X<<" , "<<d.Y<<" , "<<d.Z<<std::endl;

			nextP = p;
			f+=1./(float)nbPoints;
			sleep(0.97);
		}
		else if(f>1. && points.size() != 0){
			controlPoints = points.back();
			points.pop_back();
			aimPoints = aimPointsList.back();
			aimPointsList.pop_back();
			f=0;
		}
	#endif
		//std::cout << "pos : "<<position[0]<<" , "<<position[1]<<" , "<<position[2]<<std::endl;

		//std::cout << "direction : "<<direction[0]<<" , "<<direction[1]<<" , "<<direction[2]<<std::endl;
    
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
	//direction[1] += step;
	break;
  case 'l' :
	position[1] -= step;
	//direction[1] -= step;
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
		if(x>xmouseposition){ // droite
			
			GLfloat directionTmp[] ={direction[0], direction[1],direction[2]};
	
			direction[0]=directionTmp[0]*cos(M_PI/24)-directionTmp[2]*sin(M_PI/24);
			direction[2]=directionTmp[0]*sin(M_PI/24)+directionTmp[2]*cos(M_PI/24);
		
			//produitVectoriel(right, direction, up);
	
		}
		else if(x<xmouseposition){ // gauche
			GLfloat directionTmp[] ={direction[0], direction[1],direction[2]};
		
			direction[0]=directionTmp[0]*cos(M_PI/24)+directionTmp[2]*sin(M_PI/24);
			direction[2]=-directionTmp[0]*sin(M_PI/24)+directionTmp[2]*cos(M_PI/24);
		
			//produitVectoriel(right, direction, up);
	
		}
		if(y>ymouseposition){ // haut
			GLfloat directionTmp[] ={direction[0], direction[1],direction[2]};
			GLfloat upTmp[] = { up[0], up[1], up[2]};
	
			direction[2]=directionTmp[2]*cos(M_PI/48)-upTmp[2]*sin(M_PI/48);
			direction[1]=directionTmp[1]*cos(M_PI/48)-upTmp[1]*sin(M_PI/48);
			direction[0]=directionTmp[0]*cos(M_PI/48)-upTmp[0]*sin(M_PI/48);
			
			//produitVectoriel(right, direction, up);
		}
		else if(y<ymouseposition){ // bas
			GLfloat directionTmp[] ={direction[0], direction[1],direction[2]};
			GLfloat upTmp[] = { up[0], up[1], up[2]};
	
			direction[2]=directionTmp[2]*cos(M_PI/60)+upTmp[2]*sin(M_PI/60);
			direction[1]=directionTmp[1]*cos(M_PI/60)+upTmp[1]*sin(M_PI/60);
			direction[0]=directionTmp[0]*cos(M_PI/60)+upTmp[0]*sin(M_PI/60);
			
			//produitVectoriel(right, direction, up);
		}
	
	
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
	game.setTimer()=1.;

#ifndef __NO_SHADER__
  // load all shader programs  
  for(unsigned int i=0; i < NB_SHADERS; ++i)
  { 
    loadShader(tabShader[i]+".vert", tabShader[i]+".frag", i);
  }
  
  game.initShadowGL();

 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
#endif
 
  

#ifdef __CUBE_MAP__
  //Creation de la cube map
		
  // sky cubemap
  cubeMap=new CubeMap();

  //glBindTexture(GL_TEXTURE_CUBE_MAP, texturesid[3]);
  cubeMap->initDoubleCubeMap(
   "textures/greenhill_negative_x.ppm",
   "textures/greenhill_negative_z.ppm",
   "textures/greenhill_negative_y.ppm",
   "textures/greenhill_positive_x.ppm",
   "textures/greenhill_positive_y.ppm",
   "textures/greenhill_positive_z.ppm",
   "textures/night_negx.ppm",
   "textures/night_negz.ppm",
   "textures/night_negy.ppm",
   "textures/night_posx.ppm",
   "textures/night_posy.ppm",
   "textures/night_posz.ppm");

  // church cubemap
  church=new CubeMap();
  church->initSimpleCubeMap(
   "textures/church/negative_x.ppm",
   "textures/church/negative_z.ppm",
   "textures/church/negative_y.ppm",
   "textures/church/positive_x.ppm",
   "textures/church/positive_y.ppm",
   "textures/church/positive_z.ppm");

  game.setSky(cubeMap);
  game.setChurch(church);


#endif

#ifdef __ALPHA_TEST__
  glGenTextures(1,&idTexAlpha);
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

	//start position {0.0f,6.5f,-2.0f};
  // camera
	nbPoints = 256;
	f=0;
	vector3df point1= vector3df(-22.,6.5,-2.);
	vector3df point2= vector3df(0.,6.5, 10.0);
  vector3df point3= vector3df(0.,0.5,2.);
	vector3df cam = vector3df(position[0], position[1], position[2]);
	std::vector<vector3df>  list1;
	list1.push_back(cam);
	list1.push_back(point1);
	list1.push_back(point2);
	list1.push_back(point3);
	

	std::vector<vector3df>  list2;
	list2.push_back(point3);
	vector3df point= vector3df(-1.,0.5,1.);	
	list2.push_back(point);
	point.X=-0.75; point.Y=1.5; point.Z=-2.5;
	list2.push_back(point);
	point.X=2.5; point.Y=1.5; point.Z=-2.5;
	list2.push_back(point);
	point.X=3.2; point.Y=1.5; point.Z=0.;
	list2.push_back(point);
	point.X=2.4; point.Y=1.5; point.Z=1.2;
	list2.push_back(point);
	
	std::vector<vector3df>  list3;
	list3.push_back(point);	
	point.X=-0.9; point.Y=0.5; point.Z=-1.5;
	list3.push_back(point);	
	point.X=-3.; point.Y=1.5; point.Z=-2.;
	list3.push_back(point);	
	point.X=-4.5; point.Y=1.5; point.Z=-2.6;
	list3.push_back(point);	
	point.X=-2.; point.Y=0.5; point.Z=-3.6;
	list3.push_back(point);		

	std::vector<vector3df>  list4;
	list4.push_back(point);		
	list4.push_back(point);		
	point.X=0.2; point.Y=0.5; point.Z=-5;
	list4.push_back(point);	

	std::vector<vector3df>  list5;
	list5.push_back(point);		
	point.X=3.7; point.Y=2.5; point.Z=-3.;
	list5.push_back(point);	
	point.X=2.5; point.Y=2.5; point.Z=-6.6;
	list5.push_back(point);
	point.X=0.7; point.Y=0.65; point.Z=-7;
	list5.push_back(point);

	
	std::vector<vector3df>  list6;
	list6.push_back(point);
	point.X=0.4; point.Y=0.65; point.Z=-2.3;
	list6.push_back(point);
	point.X=-2.9; point.Y=0.65; point.Z=1.78;
	list6.push_back(point);
	point.X=-3.; point.Y=1.; point.Z=3.4;
	list6.push_back(point);
	point.X=-5.1; point.Y=1.8; point.Z=3.4;
	list6.push_back(point);	
	point.X=-5.125; point.Y=1.8; point.Z=1.65;
	list6.push_back(point);		
	point.X=-2.8; point.Y=1.8; point.Z=1.79;
	list6.push_back(point);
	list6.push_back(point);
	point.X=-5.125; point.Y=1.8; point.Z=1.65;
	list6.push_back(point);	
	point.X=-5.05; point.Y=1.8; point.Z=3.3;
	list6.push_back(point);	
	list6.push_back(point);	

	std::vector<vector3df>  list7;
	list7.push_back(point);	
	point.X=-3.9; point.Y=1.8; point.Z=1.4;
	list7.push_back(point);
	point.X=0.; point.Y=6.5; point.Z=10.0;
	list7.push_back(point);
	point.X=-24.; point.Y=6.5; point.Z=-2.;
	list7.push_back(point);
	point.X=0.0f; point.Y=6.5f; point.Z=-10.0f;
	list7.push_back(point);
			
	controlPoints =list1;
	points.push_back(list7);
	points.push_back(list6);
	points.push_back(list5);
	points.push_back(list4);
	points.push_back(list3);
	points.push_back(list2);
		
	//Direction
	vector3df dir = vector3df(direction[0], direction[1], direction[2]);
	std::vector<vector3df>  aimList1;
	std::vector<vector3df>  aimList2;
	std::vector<vector3df>  aimList3;
	std::vector<vector3df>  aimList4;
	std::vector<vector3df>  aimList5;
	std::vector<vector3df>  aimList6;
	std::vector<vector3df>  aimList7;

	aimList1.push_back(dir);
	aimList1.push_back(dir);
	aimList1.push_back(dir);
	aimList1.push_back(dir);
	dir.X=1.; dir.Z=0.;
	aimList1.push_back(dir);
		
	aimList2.push_back(dir);
	aimList2.push_back(dir);
	aimList2.push_back(dir);
	aimList2.push_back(dir);
	aimList2.push_back(dir);
	dir.X=-2.; dir.Z=-2.;
	aimList2.push_back(dir);
	
	aimList3.push_back(dir);
	aimList3.push_back(dir);
	dir.X=-4.; dir.Z=-3.;
	aimList3.push_back(dir);
	dir.X=-2.; dir.Z=-2.;
	aimList3.push_back(dir);
	aimList3.push_back(dir);
	
	aimList4.push_back(dir);
	dir.X=0.4; dir.Z=-5.2;
	aimList4.push_back(dir);
	aimList4.push_back(dir);

	dir.X=-0.9; dir.Y=0.7; dir.Z=0.7;
	aimList5.push_back(dir);
	aimList5.push_back(dir);
	dir.X=-4.7; dir.Y=-1.5; dir.Z=10.6;
	aimList5.push_back(dir);
	dir.X=2.1; dir.Y=-6.2; dir.Z=16.9;
	aimList5.push_back(dir);

	
	aimList6.push_back(dir);	
	dir.X=-10.4; dir.Y=-5.7; dir.Z=20.8;
	aimList6.push_back(dir);
	dir.X=-6.4; dir.Y=-6.35; dir.Z=19.5;
	aimList6.push_back(dir);
	dir.X=-4.1; dir.Y=0.7; dir.Z=2.1;
	aimList6.push_back(dir);
	aimList6.push_back(dir);
	aimList6.push_back(dir);
	aimList6.push_back(dir);
	dir.X=-4.1; dir.Y=3.; dir.Z=2.1;
	aimList6.push_back(dir);
	aimList6.push_back(dir);
	aimList6.push_back(dir);
	aimList6.push_back(dir);
			
	aimList7.push_back(dir);
	dir.X=0.0f; dir.Y=0.5; dir.Z=-2.0f;
	aimList7.push_back(dir);
	aimList7.push_back(dir);
	aimList7.push_back(dir);
					
	aimPoints =aimList1;
	aimPointsList.push_back(aimList7);
	aimPointsList.push_back(aimList6);
	aimPointsList.push_back(aimList5);
	aimPointsList.push_back(aimList4);
	aimPointsList.push_back(aimList3);
	aimPointsList.push_back(aimList2);

	nextP = vector3df(position[0], position[1], position[2]);

#ifdef __MAIN_SCENE__
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  checkGLError(711);

  glLightfv(GL_LIGHT0, GL_SPECULAR, grey);
  glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);

  game.setProgramObject(programobject);
  game.initGL();

#endif


  
#ifdef __TEST_TEXTURE__  
   
    glGenTextures(1,&idTexAlpha);
    glBindTexture (GL_TEXTURE_2D, idTexAlpha);
    unsigned int tmpwidth, tmpheight;
    unsigned char * image = loadPPM("models/wall/gate_wood.ppm",tmpwidth,tmpheight);
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
  
#endif
  

}

int main(int argc, char **argv)
{

  Game & game = Game::Instance();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowPosition(windowx, windowy);
  glutInitWindowSize(windowwidth, windowheight);
  game.setWindowSize(windowwidth, windowheight);

  
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

