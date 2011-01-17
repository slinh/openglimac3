#include "Game.hpp"
#include "Scene.hpp"
#include "Obj.hpp"
#include "common/include/XMLParser.hpp"


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
	}
	
	
}

void Game::display()
{
	
	if( currentScene < (int)sceneList.size() )
	{
		// if not main : display wall
		if( sceneList[currentScene]->getTypeScene() != MAIN)
		{
      if(sceneList[currentScene]->getTypeShader() == ENV)
      {
        /*glPushMatrix();
          sceneList[currentScene]->getContentHouse().getBbox().display();
        glPopMatrix();*/

        glPushMatrix();
        glBindTexture (GL_TEXTURE_CUBE_MAP, church->getIdTex());

        church->drawCubeMap(10.0);

        #ifndef __NO_SHADER__
          glUseProgramObjectARB(0);
        #endif

        float mat[16];
        float invmat[16];
        glGetFloatv (GL_MODELVIEW_MATRIX, mat);
        mat_inverse (mat, invmat);
        setInvMat(invmat);

      }
      else
      {
        glUseProgramObjectARB(programObject[PARALLAX]);

       
        glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "wallTex"), 0);
        glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "heightmapTex"), 1);
        glUniform1i(glGetUniformLocationARB(programObject[PARALLAX], "normalmapTex"), 2);

        Bbox::bindTextures();

        glPushMatrix();

        sceneList[currentScene]->getContentHouse().getBbox().display();
        glPopMatrix();
        Bbox::unbindTextures();

        glUseProgramObjectARB(0);
      }
		}

		
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

        // CPU => GPU
        glUniformMatrix4fv(glGetUniformLocationARB(programObject[ENV], "matInv"), 1, false, invmat);
        // myTexture => GPU
        glUniform1i(glGetUniformLocationARB(programObject[ENV], "myTexCube"), 0);
        // position => GPU
        glUniform3fv(glGetUniformLocationARB(programObject[ENV], "posCamera"), 1, position);

			break;

			case CUBEMAP:
			break;
			case TOON:
        glUniform1i(glGetUniformLocationARB(programObject[TOON], "diffuseTexture"), 0);
        
				// glUseProgramObjectARB(programObject[TOON]);
				// std::cout << "TOON" << std::endl;
			break;
/*			case BUMP:
			break;
*/
      case ALPHA:
        glUniform1i(glGetUniformLocationARB(programObject[ALPHA], "colorMap"), 0);
        glUniform1i(glGetUniformLocationARB(programObject[ALPHA], "alphaMap"), 1);
			break;

      case SHADOW:
        glUseProgramObjectARB(0);
        std::cout << "shadow" << std::endl;
      break;

      default:
      break;
		}

		// display the scene
		sceneList[currentScene]->display();

	}

      if(sceneList[currentScene]->getTypeShader() == ENV)
      {
          glPopMatrix();
          glPopMatrix();

          glMatrixMode(GL_TEXTURE);
          glPopMatrix();
          glMatrixMode(GL_MODELVIEW);
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
