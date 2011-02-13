#include "Game.hpp"
#include "Scene.hpp"
#include "Draw.hpp"
#include "Obj.hpp"
#include "TripleBillboard.hpp"
#include "common/include/XMLParser.hpp"
#include "ppm.hpp"
#include "utils.hpp"

//#define __NO_SHADER__


void Game::displayMatrix(GLfloat * m)
{
  std::cout << m[0] << ", " << m[1] << ", " << m[2] <<  ", " << m[3] << std::endl;
  std::cout << m[4] << ", " << m[5] << ", " << m[6] <<  ", " << m[7] << std::endl;
  std::cout << m[8] << ", " << m[9] << ", " << m[10] <<  ", " << m[11] << std::endl;
  std::cout << m[12] << ", " << m[13] << ", " << m[14] <<  ", " << m[15] << std::endl << std::endl;

}

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

	loader = new Loader();
  loadXML(pFilename, sceneList, *loader);

  // set relation between other scene and content house
  setContentHouse();

  currentScene = 0;

  biasmatrix[0] = 0.5;
  biasmatrix[1] = 0.0;
  biasmatrix[2] = 0.0;
  biasmatrix[3] = 0.0;
  biasmatrix[4] = 0.0;
  biasmatrix[5] = 0.5;
  biasmatrix[6] = 0.0;
  biasmatrix[7] = 0.0;
  biasmatrix[8] = 0.0;
  biasmatrix[9] = 0.0;
  biasmatrix[10] = 0.5;
  biasmatrix[11] = 0.0;
  biasmatrix[12] = 0.5;
  biasmatrix[13] = 0.5;
  biasmatrix[14] = 0.5;
  biasmatrix[15] = 1.0;
}

Game::~Game()
{
  sceneList.clear();
  delete loader;
}

void Game::initGL()
{
	
	// init texture in loader
	loader->initTextures();
	
	// init obj in loader
  loader->initObj();	
	
	for(unsigned int i=0; i<sceneList.size(); ++i){
		sceneList[i]->initGL();
	}

        //Appel des Billboards
        initTripleBillboard();
        checkGLError(169);


}

void Game::display()
{
	
	checkGLError(173);

	static GLfloat white[]= { 1.f, 1.f, 1.f, 1.0f };
	static GLfloat yellow[]= { 0.4f, 0.4f, 0.1f, 1.0f };
	static GLfloat softred[]= { 0.8f, 0.0f, 0.0f, 1.0f };
	static GLfloat grey[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	static GLfloat black[]= { 0.0f, 0.0f, 0.0f, 1.0f };	
	static GLfloat blue[]= { 0.0f, 0.0f, 1.0f, 1.0f };
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	//  glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,white);
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 10.0f);
	
	//first light :
	glPushMatrix();

		glLightfv(GL_LIGHT0, GL_SPECULAR, grey);
		glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);


		glPushMatrix();
		
	    if( sceneList[currentScene]->getTypeScene() != MAIN )
		{
				vector3df trans = sceneList[currentScene]->getContentHouse().getTranslate();
				glTranslatef(trans.X, trans.Y, trans.Z);

				glRotatef(sceneList[currentScene]->getLightRadian(),0.0,1.0,0.0);
		}

		glLightfv(GL_LIGHT0, GL_POSITION, sceneList[currentScene]->getLightPosition());
	    glPopMatrix();
	
	  // See where the light is
		glPushMatrix();
	if( sceneList[currentScene]->getTypeScene() != MAIN )
	{
		vector3df trans = sceneList[currentScene]->getContentHouse().getTranslate();
		glTranslatef(trans.X, trans.Y, trans.Z);
	}
		glRotatef(sceneList[currentScene]->getLightRadian(),0.0,1.0,0.0);
			 glTranslatef(sceneList[currentScene]->getLightPosition()[0],sceneList[currentScene]->getLightPosition()[1],sceneList[currentScene]->getLightPosition()[2]);
			 drawSphere(0.1, 30, 30);
		glPopMatrix();

  glPopMatrix();
	
	if(sceneList[currentScene]->getTinyLightActive())
	{
		/*
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,grey);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,grey);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,grey);
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 10.0f);
		 */
		
		// tiny light
		glPushMatrix();
		
		// define the repere to the center of the object :
		vector3df trans = sceneList[currentScene]->getContentHouse().getTranslate();
		
		glTranslatef(trans.X, trans.Y, trans.Z);
		
		drawRepere();
		
		/*
		glLightfv(GL_LIGHT0, GL_SPECULAR, white);
		glLightfv(GL_LIGHT0, GL_AMBIENT, black);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
		*/
		 
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, softred);
		glLightfv(GL_LIGHT1, GL_AMBIENT, yellow);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, white);


		
	glRotatef(sceneList[currentScene]->getTinyLightRadian(),0.0,1.0,0.0);
		glLightfv(GL_LIGHT1, GL_POSITION, sceneList[currentScene]->getTinyLightPosition());

			 glTranslatef(sceneList[currentScene]->getTinyLightPosition()[0],
										sceneList[currentScene]->getTinyLightPosition()[1],
										sceneList[currentScene]->getTinyLightPosition()[2]);
			drawSphere(sceneList[currentScene]->getTinyLightSize(), 30, 30);
		

	glPopMatrix();
		
	}
	else
	{
	//	std::cout << "no second light" << std::endl;
		glDisable(GL_LIGHT1);
	}
	
	
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
          //todo ; enlever, de la triche
          glColor3f(1.0f,1.0f,1.0f);
          glEnable(GL_TEXTURE_CUBE_MAP);
//          std::cout << "church->getIdTex():" << church->getIdTex() << std::endl;
					glBindTexture (GL_TEXTURE_CUBE_MAP, church->getIdTex());
          glEnable(GL_TEXTURE_GEN_S);
          glEnable(GL_TEXTURE_GEN_T);
          glEnable(GL_TEXTURE_GEN_R);

					church->drawCubeMap(10.0);

					#ifndef __NO_SHADER__
						glUseProgramObjectARB(0);
          #endif
					glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
					glDisable(GL_TEXTURE_CUBE_MAP);
					
					// invert matrix
					float mat[16];
					float invmat[16];
					glGetFloatv (GL_MODELVIEW_MATRIX, mat);
					mat_inverse (mat, invmat);
					setInvMat(invmat);

			}
      else if (sceneList[currentScene]->getTypeShader() == SHADOW)
      {

          #ifndef __NO_SHADER__
		  
          displayShadow();
          
          glUseProgramObjectARB(programObject[PARALLAX]);
          glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "wallTex"), 0);
          glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "heightmapTex"), 1);
          glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "normalmapTex"), 2);
          #endif

          //PUSH 6
          glPushMatrix();
             sceneList[currentScene]->getContentHouse().getBbox().displayWall(CLASSIC);
          //POP 6
          glPopMatrix();

          #ifndef __NO_SHADER__
          glUseProgramObjectARB(0);
          #endif
 



//          displayFBO();

          checkGLError(182);
          return;
      }
			//END IF 3
			//ELSE IF 1
      else if  (sceneList[currentScene]->getTypeShader() == TOON)
      {
              glEnable(GL_CULL_FACE);
              glFrontFace(GL_CW);
              glPolygonMode(GL_FRONT, GL_FILL);
              glCullFace(GL_FRONT);
      }
			//END ELSE IF 1
			//ELSE 1
			else{
				
				#ifndef __NO_SHADER__
				glUseProgramObjectARB(programObject[PARALLAX]);
				glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "wallTex"), 0);
				glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "heightmapTex"), 1);
				glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "normalmapTex"), 2);
				#endif

				//PUSH 6
				glPushMatrix();

					sceneList[currentScene]->getContentHouse().getBbox().displayWall(CLASSIC);

				//POP 6
				glPopMatrix();
				
				#ifndef __NO_SHADER__
				glUseProgramObjectARB(0);
				#endif

				sceneList[currentScene]->setContentHouse().setBbox().displayUpDown();

			}
			//END ELSE
		}
    else
    {
      displaySky();
    }
		//END IF 2
		
		// active the scene shader
		#ifndef __NO_SHADER__
		glUseProgramObjectARB(programObject[sceneList[currentScene]->getTypeShader()]);
		#endif
		
		float position[3];
		
		position[0] = camera.X;
		position[1] = camera.Y;
		position[2] = camera.Z;
				
		switch(sceneList[currentScene]->getTypeShader())
		{
			case NORMALSPEC:	
				#ifndef __NO_SHADER__
				glUniform1i(glGetUniformLocationARB(programObject[NORMALSPEC], "diffuseTexture"), 0);
				glUniform1i(glGetUniformLocationARB(programObject[NORMALSPEC], "normalMap"), 1);
				glUniform1i(glGetUniformLocationARB(programObject[NORMALSPEC], "specularMap"), 2);
				glUniform1f(glGetUniformLocationARB(programObject[NORMALSPEC], "shininess"), .2);
				#endif
				break;
			
			case PARALLAX:
				break;
				
			case ENV:

				glMatrixMode(GL_TEXTURE);
				glPushMatrix();
				glLoadMatrixf(invmat);
				glMatrixMode(GL_MODELVIEW);
				
				glEnable(GL_TEXTURE_CUBE_MAP);
  			glActiveTexture(GL_TEXTURE0);
 	 			glBindTexture (GL_TEXTURE_CUBE_MAP, church->getIdTex());

				#ifndef __NO_SHADER__
				glUniformMatrix4fv(glGetUniformLocationARB(programObject[ENV], "matInv"), 1, false, invmat);
				glUniform1i(glGetUniformLocationARB(programObject[ENV], "myTexCube"), 0);
				glUniform3fv(glGetUniformLocationARB(programObject[ENV], "posCamera"), 1, position);
				#endif
				
				break;
				
			case CUBEMAP:
				break;
				
      case TOON:
				#ifndef __NO_SHADER__
        glUniform1i(glGetUniformLocationARB(programObject[TOON], "diffuseTexture"), 0);
        #endif
				break;

			case ALPHA:
        #ifndef __NO_SHADER__
        glUniform1i(glGetUniformLocationARB(programObject[ALPHA], "colorMap"), 0);
				glUniform1i(glGetUniformLocationARB(programObject[ALPHA], "alphaMap"), 1);
				#endif
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

  if(sceneList[currentScene]->getTypeShader() == ENV)
  {
		glPopMatrix();
		//glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		//POP 1
		glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDisable(GL_TEXTURE_CUBE_MAP);

  }

  else if(sceneList[currentScene]->getTypeShader() == TOON)
  {

      // display strokes for toon shading
      #ifndef __NO_SHADER__
        glUseProgramObjectARB(0);
      #endif

      glLineWidth(5.0);
      glDisable(GL_LIGHTING);
      glCullFace (GL_BACK);
      glPolygonMode(GL_FRONT, GL_LINE);
      glColor3f(0.0,0.0,0.0);

      sceneList[currentScene]->display();
      sceneList[currentScene]->getContentHouse().getBbox().displayWall(TOONWALL);
      sceneList[currentScene]->setContentHouse().setBbox().displayUpDown();

      glLineWidth (1.0f);
      glDisable(GL_CULL_FACE);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glFrontFace(GL_CCW);
  }
  checkGLError(265);

#ifndef __NO_SHADER__
  glUseProgramObjectARB(0);
#endif

  // si scene principale :
  if(sceneList[currentScene]->getTypeShader() == MAIN)
  {
      /********************************  Billboard  *******************************************/
      glDisable(GL_CULL_FACE);
      glEnable(GL_TEXTURE_2D);
      //Gestion de la transparence
      glEnable(GL_BLEND);
      // faire des mélanges
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_EQUAL,1);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBindTexture(GL_TEXTURE_2D, idTexBillboard);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      // Affichage du billboard
      // vector3df cam = vector3df(position[0], position[1], position[2]);


      //   glutSolidSphere(1., 30, 30);
      //monBillboard->draw(cam);
      //Appel du glCallList

      checkGLError(396);
      glCallList(displayListId);
checkGLError(398);

      glBindTexture(GL_TEXTURE_2D, 0);
      // Activation/Desactivation de certains états
      glDisable(GL_TEXTURE_2D);
	  
	  glDisable(GL_BLEND);
	  glDisable(GL_ALPHA_TEST);

      /********************************  Billboard  *******************************************/

  }

}

void Game::idleGL(void)
{
	// light principal
	if(sceneList[currentScene]->getTypeScene() != MAIN)
	{
		sceneList[currentScene]->setLightRadian() += sceneList[currentScene]->getLightPas();
		//std::cout << "light : " << sceneList[currentScene]->getLightRadian() << std::endl;
	}
	
	// moving light
	if(sceneList[currentScene]->getTinyLightActive())
	{
			sceneList[currentScene]->setTinyLightRadian() += sceneList[currentScene]->getTinyLightPas();
	}
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
//  std::cout << "init shadow GL" << std::endl;
  // shadow mapping
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);


  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glGenTextures (1,&shadowtexid);
  glBindTexture (GL_TEXTURE_2D, shadowtexid);
  glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,
               windowwidth * 3.,windowheight * 3.,0,
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


  glGenFramebuffersEXT (1, &shadowbufferid);

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

  GLfloat white[]= { 1.0f, 1.0f, 1.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_SPECULAR,white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,white);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);

  glBindTexture (GL_TEXTURE_2D, 0);



  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void Game::displaySky()
{
		//std::cout<< "displaySky idtex0 : " << (GLuint)sky->getIdTex() << " idtex1 : " << sky->getIdTex2() << std::endl;

  glPushMatrix();

    glEnable(GL_TEXTURE_CUBE_MAP);
  	glActiveTexture(GL_TEXTURE0);
 	 	glBindTexture (GL_TEXTURE_CUBE_MAP, sky->getIdTex());
 	 	
 	 	glEnable(GL_TEXTURE_CUBE_MAP);
 	 	glActiveTexture(GL_TEXTURE1);
 	 	glBindTexture(GL_TEXTURE_CUBE_MAP, sky->getIdTex2());

#ifndef __NO_SHADER__
    glUseProgramObjectARB(programObject[CUBEMAP]);
    glUniform1i(glGetUniformLocationARB(programObject[CUBEMAP] ,"id_tex_day"), 0);
    glUniform1i(glGetUniformLocationARB(programObject[CUBEMAP] ,"id_tex_night"), 1);
    glUniform1f(glGetUniformLocationARB(programObject[CUBEMAP] ,"timer"), timer);



#endif
    sky->display();
    
    //std::cout<< "display Sky2 idtex0 : " << sky->getIdTex() << "idtex1 : " << sky->getIdTex2() << std::endl;
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
    glDisable(GL_TEXTURE_CUBE_MAP);

#ifndef __NO_SHADER__
   glUseProgramObjectARB(0);
#endif
		    
    

  glPopMatrix();
}


void Game::drawShadow(bool shaders)
{

  if(shaders)
  {
  //  glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
  }
   else
  {
   glEnable(GL_CULL_FACE);
   //glDisable(GL_CULL_FACE);
   glCullFace(GL_FRONT);
  }

  GLfloat white[]= { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat gray[]= { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat red[]= { 1.0f, 0.0f, 0.0f, 1.0f };
  GLfloat softred[]= { 0.2f, 0.0f, 0.0f, 1.0f };
  GLfloat blue[]= { 0.0f, 0.0f, 1.0f, 1.0f };
  GLfloat softblue[]= { 0.0f, 0.0f, 0.2f, 1.0f };
  GLfloat black[]= { 0.0f, 0.0f, 0.0f, 1.0f };
  GLfloat grey[]= { .5f, .5f, .5f, 1.0f };
/*
  glMaterialfv(GL_FRONT, GL_SPECULAR,white);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,red);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softred);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);
*/

  Obj* tmp;

  for(unsigned int i=0; i<sceneList[currentScene]->getObjList().size(); ++i)
  {
    tmp = sceneList[currentScene]->getObjList()[i];

    if(shaders)
    {

      /*
      glActiveTexture(GL_TEXTURE1);

      glBindTexture(GL_TEXTURE_2D,1);
*/
      glPushMatrix();
      glLoadIdentity();

      glTranslatef(tmp->getTranslate().X, tmp->getTranslate().Y, tmp->getTranslate().Z);
      glScalef(tmp->getScale().X, tmp->getScale().Y, tmp->getScale().Z);
    //  glRotatef(tmp->getAngleRotation(), tmp->getAxeRotate().X, tmp->getAxeRotate().Y, tmp->getAxeRotate().Z);

      glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
      glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
      glUniform1i(glGetUniformLocationARB(programObject[SHADOW],"diffuseTexture"), 1);
      glUniform1i(glGetUniformLocationARB(programObject[SHADOW],"boolShadow"), 1);

      glPopMatrix();

    }


    glPushMatrix();
        glTranslatef(tmp->getTranslate().X, tmp->getTranslate().Y, tmp->getTranslate().Z);
        glScalef(tmp->getScale().X, tmp->getScale().Y, tmp->getScale().Z);
       // glRotatef(tmp->getAngleRotation(), tmp->getAxeRotate().X, tmp->getAxeRotate().Y, tmp->getAxeRotate().Z);
       //  tmp->RenderOBJModel();
        tmp->displayList();
    glPopMatrix();

    //objList[i]->setBbox().display();
  }

/*
  if(shaders)
  {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-1.,0.33,2.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
//    std::cout << "trans 1" << std::endl;
//    displayMatrix(transformationmatrix);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
    glPopMatrix();
  }


  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glTranslatef(-1.0,0.33,2.0);
  //glutSolidSphere(0.5,30,30);
  glutSolidTeapot(1.0);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_SPECULAR,gray);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,blue);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softblue);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);

  glCullFace(GL_BACK);
  */



  glMaterialfv(GL_FRONT, GL_SPECULAR,gray);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,white);
  glMaterialfv(GL_FRONT, GL_AMBIENT,white);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);

  if(shaders)
  {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0,0.0,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
    glUniform1i(glGetUniformLocationARB(programObject[SHADOW],"boolShadow"), 0);
    glPopMatrix();
  }

  glPushMatrix();
  glTranslatef(0.0,0.0,0.0);

  glBegin(GL_QUADS);
  glColor3f(1.0f,1.0f,1.0f);
  glNormal3f(0.0f,1.0f,0.0f);
  glVertex3f(-10.0f, 0.0f, 10.0f);
  glVertex3f( 10.0f, 0.0f, 10.0f);
  glVertex3f( 10.0f, 0.0f,-10.0f);
  glVertex3f(-10.0f, 0.0f,-10.0f);
  glEnd();
  glPopMatrix();

  glCullFace(GL_BACK);

}

void Game::drawShadow2(bool shaders)
{
 // glEnable(GL_CULL_FACE);

//  if(shaders)
 //  {
 //    glCullFace(GL_BACK);
 // }
 // else
 // {
  //  glDisable(GL_CULL_FACE);
   // glCullFace(GL_FRONT);
  // }

// glCullFace(GL_FRONT);
//  glDisable(GL_CULL_FACE);
// glCullFace(GL_BACK);
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

/*
  Obj* tmp;

  for(unsigned int i=0; i<sceneList[currentScene]->getObjList().size(); ++i)
  {
    tmp = sceneList[currentScene]->getObjList()[i];

    if(shaders)
    {
      glPushMatrix();
      glLoadIdentity();
      // todo translate

      glTranslatef(tmp->getTranslate().X, tmp->getTranslate().Y, tmp->getTranslate().Z);
      glScalef(tmp->getScale().X, tmp->getScale().Y, tmp->getScale().Z);
    //  glRotatef(tmp->getAngleRotation(), tmp->getAxeRotate().X, tmp->getAxeRotate().Y, tmp->getAxeRotate().Z);

      glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
      glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
      glPopMatrix();
    }


    glPushMatrix();
        glTranslatef(tmp->getTranslate().X, tmp->getTranslate().Y, tmp->getTranslate().Z);
        glScalef(tmp->getScale().X, tmp->getScale().Y, tmp->getScale().Z);
       // glRotatef(tmp->getAngleRotation(), tmp->getAxeRotate().X, tmp->getAxeRotate().Y, tmp->getAxeRotate().Z);
       //  tmp->RenderOBJModel();
        tmp->displayList();
    glPopMatrix();

    //objList[i]->setBbox().display();
  }


*/


  if(shaders)
  {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(1.,0.33,2.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
//    std::cout << "trans 1" << std::endl;
//    displayMatrix(transformationmatrix);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
    glPopMatrix();
  }


//  sceneList[currentScene]->display();
// test sphere

  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glTranslatef(1.0,0.33,2.0);
  //glutSolidSphere(0.5,30,30);
  glutSolidTeapot(1.0);
  glPopMatrix();


  if(shaders)
  {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(3.,1.33,2.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
 //   std::cout << "trans 2" << std::endl;
//    displayMatrix(transformationmatrix);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
    glPopMatrix();
  }


//  sceneList[currentScene]->display();
// test sphere

  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glTranslatef(3.0,1.33,2.0);
  //glutSolidSphere(0.5,30,30);
  glutSolidTeapot(2.0);
  glPopMatrix();



  glMaterialfv(GL_FRONT, GL_SPECULAR,white);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,red);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softred);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);


  if(shaders)
  {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-1.0,1.33,-1.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
 //   std::cout << "trans 3" << std::endl;
 //   displayMatrix(transformationmatrix);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
    glPopMatrix();
  }

  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glTranslatef(-1.0,1.33,-1.0);
  // glutSolidCube(1.0);
  glutSolidSphere(0.5,30,30);
  glPopMatrix();






  if(shaders)
  {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(4.,1.33,2.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
//    std::cout << "trans 1" << std::endl;
//    displayMatrix(transformationmatrix);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
    glPopMatrix();
  }


//  sceneList[currentScene]->display();
// test sphere

  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glTranslatef(4.0,1.33,2.0);
  //glutSolidSphere(0.5,30,30);
  glutSolidTeapot(1.0);
  glPopMatrix();









  glMaterialfv(GL_FRONT, GL_SPECULAR,gray);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,blue);
  glMaterialfv(GL_FRONT, GL_AMBIENT,softblue);
  glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);

//  glCullFace(GL_BACK);
  if(shaders)
  {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0,-1.0,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"transmatrix"),1,GL_FALSE,transformationmatrix);
    glPopMatrix();
  }



  glPushMatrix();
  glTranslatef(0.0,-1.0,0.0);

  glBegin(GL_QUADS);
  glColor3f(1.0f,1.0f,1.0f);
  glNormal3f(0.0f,1.0f,0.0f);
  glVertex3f(-10.0f, 0.0f, 10.0f);
  glVertex3f( 10.0f, 0.0f, 10.0f);
  glVertex3f( 10.0f, 0.0f,-10.0f);
  glVertex3f(-10.0f, 0.0f,-10.0f);
  glEnd();
  glPopMatrix();

  /*
  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  // glutSolidCube(1.0);
  glutSolidTeapot(2.0);
  glPopMatrix();
*/

//  glCullFace(GL_BACK);
// glDisable(GL_CULL_FACE);
}


void Game::FBO()
{
  // Premiere passe en FBO
  if(shadowbufferid!=0)
  {
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, shadowbufferid);
    glViewport(0, 0, windowwidth*3., windowheight*3.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    checkFramebufferStatus();

    glPushMatrix();
    glLoadIdentity();

    float up[] = {0.0f, 1.0f, 0.0f};
    gluLookAt(getLightPosition()[0],getLightPosition()[1]+1.,getLightPosition()[2],
              0., 0., 0.,
              up[0],up[1],up[2]);

//    gluLookAt(getLightPosition()[0], getLightPosition()[1], getLightPosition()[2],
//      0.0f, 0.0f, 0.0f,
//      0.0f, 1.0f, 0.0f);
    glRotatef(-sceneList[currentScene]->getLightRadian(),0.0,1.0,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightmodelviewmatrix);

    multMatrix4x4(lightprojectionmatrix, lightmodelviewmatrix, shadowmatrix);
    multMatrix4x4(biasmatrix,shadowmatrix,shadowmatrix);

    glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(4.0,4.0);
      drawShadow(false);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);

    glPopMatrix();

  }
}

void Game::displayShadow()
{

  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D,shadowtexid);

  if(programObject[SHADOW]!=0){
    glUseProgramObjectARB(programObject[SHADOW]);
    glUniformMatrix4fvARB(glGetUniformLocationARB(programObject[SHADOW],"eyematrix"),1,GL_FALSE,shadowmatrix);
    glUniform1iARB(glGetUniformLocationARB(programObject[SHADOW],"shadowmap"),0);
  }
//  std::cout << "shadow matrix" << std::endl;
//  displayMatrix(shadowmatrix);

  drawShadow(true);

  glBindTexture(GL_TEXTURE_2D,0);
  glDisable(GL_TEXTURE_2D);
#ifndef __NO_SHADER__
  glUseProgramObjectARB(0);
#endif
}

void Game::displayFBO()
{
  #ifndef __NO_SHADER__
    glUseProgramObjectARB(0);
  #endif

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-2.f, 2.f, -1.5f, 1.5f, 1.f, 20.f);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();


  glEnable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shadowtexid);

  glTranslated(0., 0., -1.);

  glBegin(GL_QUADS);

  glTexCoord2f(0., 0.); glVertex3f(0., 0., 0.);
  glTexCoord2f(1., 0.); glVertex3f(2., 0., 0.);
  glTexCoord2f(1., 1.); glVertex3f(2., 1.5, 0.);
  glTexCoord2f(0., 1.); glVertex3f(0, 1.5, 0.);
  glEnd();

  //glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);


}

  void Game::setSky(CubeMap * sky){ this->sky = sky; }

  void Game::initTripleBillboard(){
      // definition de la couleur d'effacage donc couleur de fond
        glClearColor(0.0,0.0,0.0,1.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_SMOOTH);

        // comment on stocke les nifos sur la carte graphique
        glPixelStorei(GL_PACK_ALIGNMENT,1);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      int widthData, heightData;
      GLubyte * data = NULL;
      vector3di myVector = vector3di(255,255,255);
      data = loadPPMRGBA((char *)"textures/grass.ppm", &widthData, &heightData, myVector);
          glGenTextures(1, &idTexBillboard);
      glBindTexture(GL_TEXTURE_2D, idTexBillboard);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthData, heightData, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      free(data);
      checkGLError(1108);
      displayListId = glGenLists(1); //Make room for the display list
      glNewList(displayListId, GL_COMPILE); //Begin the display list
        vector3df* myPosition = new vector3df(-0.5f,0.0f,0.0f);
        TripleBillBoard * b1 = new TripleBillBoard(myPosition,0.5f,2.f);
        b1->draw(getCamera()); //Add commands for drawing a cube to the display list
        vector3df* myPosition2 = new vector3df(-0.5f,0.0f,0.0f);
        TripleBillBoard * b2 = new TripleBillBoard(myPosition2,0.5f,2.f);
        b2->draw(getCamera()); //Add commands for drawing a cube to the display list
      glEndList();
      checkGLError(1118);
      glBindTexture(GL_TEXTURE_2D, 0);
      checkGLError(1120);
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
      glDisable(GL_DEPTH_TEST);
      //glDisable(GL_SMOOTH);
      checkGLError(1125);
  }
