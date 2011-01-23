#include "Game.hpp"
#include "Scene.hpp"
#include "Draw.hpp"
#include "Obj.hpp"
#include "common/include/XMLParser.hpp"
#include "utils.hpp"

bool Game::checkFramebufferStatus(void)
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

void Game::multMatrix4x4(float* m1, float* m2, float* res)
{
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(m1);
    glMultMatrixf(m2);
    glGetFloatv(GL_MODELVIEW_MATRIX, res);
    glPopMatrix();
}

void Game::mat_inverse (float *in, float *out)
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

Game::Game()
{
	
  /// scene principale
  const char * pFilename = "common/xml/demo.xml";

  loadXML(pFilename, sceneList);

  // set relation between other scene and content house
  setContentHouse();

  currentScene = 0;
}

Game::~Game()
{
  sceneList.clear();
}

void Game::initGL()
{
  
  Bbox::initTextures();
	
	for(unsigned int i=0; i<sceneList.size(); ++i)
  {
		sceneList[i]->initGL();

/*    if (sceneList[i]->getTypeShader() == SHADOW)
    {
      initShadowGL();
    }
*/
	}
	

}

void Game::display()
{
	//IF 1
	if( currentScene < (int)sceneList.size() )
	{
		// if not main : display wall
		//IF 2
    if( sceneList[currentScene]->getTypeScene() != MAIN)
		{
			//IF 3
			if(sceneList[currentScene]->getTypeShader() == ENV)
			{
				// draw cubemap
				//PUSH 1
				glPushMatrix();
          glEnable(GL_TEXTURE_CUBE_MAP);
//          std::cout << "church->getIdTex():" << church->getIdTex() << std::endl;
					glBindTexture (GL_TEXTURE_CUBE_MAP, church->getIdTex());
          glEnable(GL_TEXTURE_GEN_S);
          glEnable(GL_TEXTURE_GEN_T);
          glEnable(GL_TEXTURE_GEN_R);

          glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
          glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
          glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
					church->drawCubeMap(10.0);

					#ifndef __NO_SHADER__
						glUseProgramObjectARB(0);
					#endif

					// invert matrix
					float mat[16];
					float invmat[16];
					glGetFloatv (GL_MODELVIEW_MATRIX, mat);
					mat_inverse (mat, invmat);
					setInvMat(invmat);

			}
      else if (sceneList[currentScene]->getTypeShader() == SHADOW)
      {
          //shadowTest();
          checkGLError(182);
          return;
      }
			//END IF 3
			//ELSE IF 1

			//END ELSE IF 1
			//ELSE 1
			else{
				
				glUseProgramObjectARB(programObject[PARALLAX]);
				glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "wallTex"), 0);
				glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "heightmapTex"), 1);
				glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "normalmapTex"), 2);
				Bbox::bindTextures();
				
				//PUSH 6
				glPushMatrix();
					sceneList[currentScene]->getContentHouse().getBbox().display();
				//POP 6
				glPopMatrix();
				Bbox::unbindTextures();
				glUseProgramObjectARB(0);
			}
			//END ELSE
		}
    else
    {
      displaySky();
    }
		//END IF 2
		
		// active the scene shader
		glUseProgramObjectARB(programObject[sceneList[currentScene]->getTypeShader()]);
		
		float position[3];
		
		position[0] = camera.X;
		position[1] = camera.Y;
		position[2] = camera.Z;
				
		switch(sceneList[currentScene]->getTypeShader())
		{
			case NORMALSPEC:	
      
				glUniform1i(glGetUniformLocationARB(programObject[NORMALSPEC], "diffuseTexture"), 0);
				glUniform1i(glGetUniformLocationARB(programObject[NORMALSPEC], "normalMap"), 1);
				glUniform1i(glGetUniformLocationARB(programObject[NORMALSPEC], "specularMap"), 2);
				glUniform1f(glGetUniformLocationARB(programObject[NORMALSPEC], "shininess"), .2);
				break;
			
			case PARALLAX:
				break;
				
			case ENV:

				glMatrixMode(GL_TEXTURE);
				glPushMatrix();
				glLoadMatrixf(invmat);
				glMatrixMode(GL_MODELVIEW);

				glUniformMatrix4fv(glGetUniformLocationARB(programObject[ENV], "matInv"), 1, false, invmat);
				glUniform1i(glGetUniformLocationARB(programObject[ENV], "myTexCube"), 0);
				glUniform3fv(glGetUniformLocationARB(programObject[ENV], "posCamera"), 1, position);
				break;
				
			case CUBEMAP:
				break;
				
			case TOON:
				glUniform1i(glGetUniformLocationARB(programObject[TOON], "diffuseTexture"), 0);
				break;

			case ALPHA:
        glUniform1i(glGetUniformLocationARB(programObject[ALPHA], "colorMap"), 0);
				glUniform1i(glGetUniformLocationARB(programObject[ALPHA], "alphaMap"), 1);
				break;

			case SHADOW:

				break;

			default:
				break;
		}

		// display the scene
      sceneList[currentScene]->display();

	}
	//END IF 1
	checkGLError(254);

	if(sceneList[currentScene]->getTypeShader() == ENV){
		glPopMatrix();
		//glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		//POP 1
		glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDisable(GL_TEXTURE_CUBE_MAP);

	}
	checkGLError(265);
}

Game & Game::Instance()
{
  static Game g;
  return g;
}

void Game::checkCurrentScene()
{
	int tmpCurrent = -1;
	
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
	// only obj in main scene :
	std::vector<Obj *> myList = sceneList[0]->getObjList();
	
	for(unsigned int i=0; i<myList.size(); ++i)
  {
		if( myList[i]->getType() == HOUSE )
		{	
			if( myList[i]->getBbox().intersect(camera) )
			{
				// std::cout << "intersect avec obj " << i << std::endl;
				tmpCurrent = myList[i]->getAssociatedScene();
			}
		}
	}
	
	// if no intersect : main scene
	if( tmpCurrent == -1 )
		currentScene = 0;
	else
		currentScene = tmpCurrent;
}

void Game::setContentHouse()
{
	std::vector<Obj*> main = sceneList[0]->getObjList();
	
	for(unsigned int i=0; i<main.size(); ++i)
	{
		if( main[i]->getType() == HOUSE )
		{
			int id = main[i]->getAssociatedScene();
			sceneList[id]->setContentHouse(main[i]);
		}
	}
}

GLfloat* Game::getLightPosition()
{
  if( currentScene < (int)sceneList.size() )
	{
    return sceneList[currentScene]->getLightPosition();
  }
}

void Game::lightUp()
{
  if( currentScene < (int)sceneList.size() )
	{
    return sceneList[currentScene]->lightUp();
  }
}
  
void Game::lightDown()
{
  if( currentScene < (int)sceneList.size() )
	{
    return sceneList[currentScene]->lightDown();
  }
}

void Game::lightRight()
{
  if( currentScene < (int)sceneList.size() )
	{
    return sceneList[currentScene]->lightRight();
  }
}

void Game::lightLeft()
{
  if( currentScene < (int)sceneList.size() )
	{
    return sceneList[currentScene]->lightLeft();
  }
}

void Game::lightFront()
{
  if( currentScene < (int)sceneList.size() )
	{
    return sceneList[currentScene]->lightFront();
  }
}

void Game::lightBack()
{
  if( currentScene < (int)sceneList.size() )
	{
    return sceneList[currentScene]->lightBack();
  }
}


void Game::initShadowGL()
{
  std::cout << "init shadow GL" << std::endl;
  // shadow mapping
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
   return;

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  glDrawBuffer(GL_BACK);
  glReadBuffer(GL_FRONT);

  checkGLError(755);

  GLfloat white[]= { 1.0f, 1.0f, 1.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_SPECULAR,white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,white);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);

  glBindTexture (GL_TEXTURE_2D, 0);
  checkGLError(765);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void Game::displaySky()
{

  glPushMatrix();

    glEnable(GL_TEXTURE_CUBE_MAP);
//    std::cout << "cubeMap->getIdTex():" << sky->getIdTex() << std::endl;

// TODO : commenté ---> afficher les arbres
  switch(sky->getIdTex())
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

  glBindTexture (GL_TEXTURE_CUBE_MAP, sky->getIdTex());

#ifndef __NO_SHADER__
    glUseProgramObjectARB(programObject[CUBEMAP]);
    glUniform1i(glGetUniformLocationARB(programObject[CUBEMAP] ,"id_tex"), sky->getIdTex());

#endif
    sky->display();

#ifndef __NO_SHADER__
   glUseProgramObjectARB(0);
#endif
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDisable(GL_TEXTURE_CUBE_MAP);

  glPopMatrix();
}

void Game::drawShadow(bool shaders)
{

  GLfloat white[]= { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat gray[]= { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat red[]= { 1.0f, 0.0f, 0.0f, 1.0f };
  GLfloat softred[]= { 0.2f, 0.0f, 0.0f, 1.0f };
  GLfloat blue[]= { 0.0f, 0.0f, 1.0f, 1.0f };
  GLfloat softblue[]= { 0.0f, 0.0f, 0.2f, 1.0f };
  GLfloat black[]= { 0.0f, 0.0f, 0.0f, 1.0f };
  GLfloat grey[]= { .5f, .5f, .5f, 1.0f };

  glMaterialfv(GL_FRONT, GL_SPECULAR,white);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,red);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softred);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);

  if(shaders){
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(2.0,1.0,2.0);
      glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
      glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
      glPopMatrix();
  }

  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glTranslatef(2.0,1.0,2.0);
  drawSphere(0.5,30,30);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_SPECULAR,white);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,blue);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softblue);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);


  if(shaders){
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0,-1.0,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
    glPopMatrix();
  }


  glPushMatrix();
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

  glPopMatrix();
  /*
  glPushMatrix();
    glScalef(10.0, 10.0, 10.0);
    createCube();
  glPopMatrix();
*/
}

void Game::shadowTest()
{
// Premiere passe en FBO
    if(shadowbufferid!=0){
        glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, shadowbufferid);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        checkFramebufferStatus();

        glPushMatrix();
        glLoadIdentity();


        gluLookAt(getLightPosition()[0], getLightPosition()[1], getLightPosition()[2],
                  0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f);
//        glRotatef(-angle,0.0,1.0,0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, lightmodelviewmatrix);

        multMatrix4x4(lightprojectionmatrix,lightmodelviewmatrix,shadowmatrix);
        multMatrix4x4(biasmatrix,shadowmatrix,shadowmatrix);

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(4.0,4.0);
        drawShadow(false);
        glDisable(GL_POLYGON_OFFSET_FILL);

        glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);

        glPopMatrix();

    }

//    glPushMatrix();

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glRotatef(xrotation,1.0f,0.0f,0.0f);
//    glRotatef(yrotation,0.0f,1.0f,0.0f);
//    glTranslatef(-position[0],-position[1],-position[2]);

//    glPushMatrix();
//    glRotatef(angle,0.0,1.0,0.0);
//    glLightfv(GL_LIGHT0, GL_POSITION,getLightPosition());
//    glPopMatrix();

//    glPushMatrix();
//    glRotatef(angle,0.0,1.0,0.0);
//    glTranslatef(lightPosition[0],lightPosition[1],lightPosition[2]);

//    glutSolidSphere(0.5,30,30);
//    glPopMatrix();


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,shadowtexid);

    if(programObject[SHADOW]!=0){
        glUseProgramObjectARB(programObject[SHADOW]);
        glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"eyematrix"),1,GL_FALSE,shadowmatrix);
        glUniform1iARB(glGetUniformLocationARB(programObject[SHADOW],"shadowmap"),0);
    }


    drawShadow(true);
    glUseProgramObjectARB(0);

    GLenum err = glGetError();
    if(err!=GL_NO_ERROR){
        std::cerr << "Erreur GL :" << std::endl;
        std::cerr << gluErrorString(err) << std::endl;
    }

//    glPopMatrix();

//    glutSwapBuffers();
}
