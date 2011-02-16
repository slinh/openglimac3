#include "CubeMap.hpp"
#include "Draw.hpp"
#include <cmath>
#include <ppm.hpp>

/*********** cubemap ***************/

void CubeMap::drawCubeMap(float size)
{
  static GLfloat xPlane[] = { 1.0f, 0.0f, 0.0f, 0.0f };
  static GLfloat yPlane[] = { 0.0f, 1.0f, 0.0f, 0.0f };
  static GLfloat zPlane[] = { 0.0f, 0.0f, 1.0f, 0.0f };

  glEnable (GL_TEXTURE_GEN_S);
  glEnable (GL_TEXTURE_GEN_T);
  glEnable (GL_TEXTURE_GEN_R);

  glEnable (GL_TEXTURE_CUBE_MAP);
  glBindTexture (GL_TEXTURE_CUBE_MAP, idTex);

  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

  glTexGenfv (GL_S, GL_OBJECT_PLANE, xPlane);
  glTexGenfv (GL_T, GL_OBJECT_PLANE, yPlane);
  glTexGenfv (GL_R, GL_OBJECT_PLANE, zPlane);

  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glutSolidSphere (size, 30, 30);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);

  glDisable (GL_TEXTURE_CUBE_MAP);

  glDisable (GL_TEXTURE_GEN_S);
  glDisable (GL_TEXTURE_GEN_T);
  glDisable (GL_TEXTURE_GEN_R);
}


void CubeMap::loadTextures(const char* negative_x,
                           const char* negative_z,
                           const char* negative_y,
                           const char* positive_x,
                           const char* positive_y,
                           const char* positive_z,
                           int idT){
		//std::cout<< "idtex load" << idT << std::endl;

    glBindTexture (GL_TEXTURE_CUBE_MAP, idT);

    unsigned int tmpwidth, tmpheight;
    unsigned char * image = loadPPM(negative_x, tmpwidth, tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    delete[] image;

    glBindTexture (GL_TEXTURE_CUBE_MAP, idT);

    image = loadPPM(negative_z, tmpwidth, tmpheight);

    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB,	GL_UNSIGNED_BYTE, image);

    glBindTexture (GL_TEXTURE_CUBE_MAP, idT);

    delete[] image;

    image = loadPPM(negative_y, tmpwidth, tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glBindTexture (GL_TEXTURE_CUBE_MAP, idT);

    delete[] image;

    image = loadPPM(positive_x, tmpwidth, tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB,	GL_UNSIGNED_BYTE, image);

    glBindTexture (GL_TEXTURE_CUBE_MAP, idT);

    delete[] image;

    image = loadPPM(positive_y, tmpwidth, tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glBindTexture (GL_TEXTURE_CUBE_MAP, idT);

    delete[] image;

    image = loadPPM(positive_z,tmpwidth,tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB,	GL_UNSIGNED_BYTE, image);


    delete[] image;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



}
void CubeMap::initSimpleCubeMap(const char* negative_x,
                         const char* negative_z,
                         const char* negative_y,
                         const char* positive_x,
                         const char* positive_y,
                         const char* positive_z)
{
		
    glGenTextures(1, &idTex);

    
		/************************ Texture 1 *************************/
		if(idTex == 0){
      std::cerr << "Identifiant de texture incorrect" << std::endl;
      return;
    }
		loadTextures(negative_x, negative_z, negative_y, positive_x, positive_y, positive_z, idTex);
		
		
   	// create a displaylist
    /*displayListId=glGenLists(1);
    if(displayListId==0) std::cerr << "erreur d'initialisation de la liste " << std::endl;
    glNewList(displayListId, GL_COMPILE);
  	
  		//drawSphere(10.,30,30);

      //drawSphereMap(10.0);
    glEndList();*/
    glBindTexture (GL_TEXTURE_CUBE_MAP, 0);


}

void CubeMap::initDoubleCubeMap(const char* negative_1_x,
                         const char* negative_1_z,
                         const char* negative_1_y,
                         const char* positive_1_x,
                         const char* positive_1_y,
                         const char* positive_1_z,
                         const char* negative_2_x,
                         const char* negative_2_z,
                         const char* negative_2_y,
                         const char* positive_2_x,
                         const char* positive_2_y,
                         const char* positive_2_z)
{
		
    glGenTextures(1, &idTex);
    

    
		/************************ Texture 1 *************************/
		//std::cout<< "idtex0 " << idTex << std::endl;
		if(idTex == 0){
      std::cerr << "Identifiant de texture incorrect" << std::endl;
      return;
    }
		loadTextures(negative_1_x, negative_1_z, negative_1_y, positive_1_x, positive_1_y, positive_1_z, idTex);
		
		
		glGenTextures(1, &idTex2);
		/************************ Texture 2 *************************/
		//std::cout<< "idtex1 " << idTex2 << std::endl;
		if(idTex2 ==0){
      std::cerr << "Identifiant de texture incorrect" << std::endl;
      return;
    }
		loadTextures(negative_2_x, negative_2_z, negative_2_y, positive_2_x, positive_2_y, positive_2_z, idTex2);


   	// create a displaylist
    /*displayListId=glGenLists(1);
    if(displayListId==0) std::cerr << "erreur d'initialisation de la liste " << std::endl;
    glNewList(displayListId, GL_COMPILE);
  	
  		//drawSphere(10.,30,30);

      //drawSphereMap(10.0);
    glEndList();*/
    glBindTexture (GL_TEXTURE_CUBE_MAP, 0);


}

void CubeMap::display()
{
				//std::cout<< "avant idtex0 " << idTex << " idtex1 " << idTex2<< std::endl;

		drawSphere(12.5,40,40);
				//std::cout<< "apres idtex0 " << idTex << " idtex1 " << idTex2<< std::endl;

 //glCallList(displayListId);
}

const GLuint CubeMap::getIdTex() const { return this->idTex; }
const GLuint CubeMap::getIdTex2() const { return idTex2; }
