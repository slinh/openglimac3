#include "main.hpp"
#include "Draw.hpp"
#include <stdlib.h>
#include "Obj.hpp"
#include "common/include/XMLParser.hpp"

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

  if(game.getSceneList()[game.getCurrentScene()]->getTypeShader() == SHADOW)
  {
   game.FBO();
  }

#ifdef __MAIN_SCENE__  // MAIN SCENE - FROM XML - + NORMAL SPEC ILLUMINATION
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

glPushMatrix();
  gluLookAt(position[0],position[1],position[2],position[0]+direction[0],position[1]+direction[1],position[2]+direction[2],up[0],up[1],up[2]);

//  gluLookAt(game.getLightPosition()[0],game.getLightPosition()[1]+1.0,game.getLightPosition()[2],
//            0.0,0.0,0.0,
//            up[0],up[1],up[2]);



  //glRotatef(xrotation,1.0f,0.0f,0.0f);
  //glRotatef(yrotation,0.0f,1.0f,0.0f);
  //glTranslatef(-position[0],-position[1],-position[2]);

  glPushMatrix();
// glRotatef(angle,0.0,0.0,1.0);
   glLightfv(GL_LIGHT0, GL_POSITION, game.getLightPosition());

//   std::cout << "lightPos" <<  game.getLightPosition()[0] << ", " << game.getLightPosition()[1] << ", " << game.getLightPosition()[2] << std::endl;

  glPopMatrix();







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

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);






  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,white);
  glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 10.0f);

  // See where the light is
  glPushMatrix();
     glTranslatef(game.getLightPosition()[0],game.getLightPosition()[1],game.getLightPosition()[2]);
     drawSphere(0.1, 30, 30);
//     std::cout << "lightPos :" << game.getLightPosition()[0] << " / " << game.getLightPosition()[1] << " / " << game.getLightPosition()[2] << " /"  << game.getLightPosition()[3]<< std::endl;
  glPopMatrix();

  vector3df cam = vector3df(position[0], position[1], position[2]);
  vector3df dir = vector3df(direction[0], direction[1], direction[2]);

  game.setCamera() = cam;
  game.setDirection() = dir;

  game.checkCurrentScene();

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);


#ifdef __TEST_TEXTURE__  
  glPushMatrix();
    glTranslatef(-1.0,1.0,2.0);

    glUseProgramObjectARB(0);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture (GL_TEXTURE_2D, idTexAlpha);
      glTexEnvf(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE0);
  glTexEnvf(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
  
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
    
	  
    drawSphere(1.2,30,30);
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_TEXTURE_2D);

  glPopMatrix();
#endif

game.display();

checkGLError(332);

glPopMatrix();

glutSwapBuffers();

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

#ifdef __CASTELJAU__
//motion
   	if(f<=1.){
			vector3df p=casteljau(f,controlPoints);
			//xrotation+=(position[0]-p.X)/10.0f;
			//yrotation+=(position[1]- p.Y )/10.0f;
			position[0]=nextP.X;
			position[1]=nextP.Y;
			position[2]=nextP.Z;
			
      //aim[0]=sin(yrotation*M_PI/180.0f);
    	//aim[1]=-sin(xrotation*M_PI/180.0f);
    	//aim[2]=-cos(yrotation*M_PI/180.0f);
			direction[0]=p.X-nextP.X;
    	direction[1]=p.Y-nextP.Y;
    	direction[2]=p.Z-nextP.Z;
			/*std::cout << "x" << xrotation <<"y" <<yrotation << std::endl;
			
			std::cout << "x2" << xrotation <<"y2" <<yrotation << std::endl;*/
			//direction[3]=p.Z+1.;
			nextP = p;
			f+=1./(float)nbPoints;
			sleep(0.9);
		}
#endif
    
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
		if(x>xmouseposition){ // droite
			
			GLfloat directionTmp[] ={direction[0], direction[1],direction[2]};
	
			direction[0]=directionTmp[0]*cos(M_PI/60)-directionTmp[2]*sin(M_PI/60);
			direction[2]=directionTmp[0]*sin(M_PI/60)+directionTmp[2]*cos(M_PI/60);
		
			//produitVectoriel(right, direction, up);
	
		}
		else if(x<xmouseposition){ // gauche
			GLfloat directionTmp[] ={direction[0], direction[1],direction[2]};
		
			direction[0]=directionTmp[0]*cos(M_PI/60)+directionTmp[2]*sin(M_PI/60);
			direction[2]=-directionTmp[0]*sin(M_PI/60)+directionTmp[2]*cos(M_PI/60);
		
			//produitVectoriel(right, direction, up);
	
		}
		if(y>ymouseposition){ // haut
			GLfloat directionTmp[] ={direction[0], direction[1],direction[2]};
			GLfloat upTmp[] = { up[0], up[1], up[2]};
	
			direction[2]=directionTmp[2]*cos(M_PI/60)-upTmp[2]*sin(M_PI/60);
			direction[1]=directionTmp[1]*cos(M_PI/60)-upTmp[1]*sin(M_PI/60);
			direction[0]=directionTmp[0]*cos(M_PI/60)-upTmp[0]*sin(M_PI/60);
			
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


#ifndef __NO_SHADER__
  // load all shader programs  
  for(unsigned int i=0; i < NB_SHADERS; ++i)
  { 
    loadShader(tabShader[i]+".vert", tabShader[i]+".frag", i);
  }
#endif
 
  game.initShadowGL();


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
  church=new CubeMap();
  church->initCubeMap(
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

/*  game.setBiasmatrix(biasmatrix);
  game.setShadowbufferid(shadowbufferid);
  game.setShadowtexid(shadowtexid);
*/
  // camera
	nbPoints = 32;
	f=0;
	vector3df pointTmp= vector3df(-2.,0.5,3.0);
	vector3df point2= vector3df(10., 0.5, -1.0);
  vector3df point3= vector3df(-1., 0., 0.);
	vector3df cam = vector3df(position[0], position[1], position[2]);
	controlPoints.push_back(cam);
	controlPoints.push_back(pointTmp);
	controlPoints.push_back(point2);
	controlPoints.push_back(point3);

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

