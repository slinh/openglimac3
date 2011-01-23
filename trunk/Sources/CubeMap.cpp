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

void CubeMap::initCubeMap(const char* negative_x,
                                           const char* negative_z,
                                           const char* negative_y,
                                           const char* positive_x,
                                           const char* positive_y,
                                           const char* positive_z
                                          )
{

    glGenTextures(1, &idTex);

    // Texture init ok
    if(idTex == 0){
      std::cerr << "Identifiant de texture incorrect" << std::endl;
      return;
    }

    unsigned int tmpwidth, tmpheight;
    unsigned char * image = loadPPM(negative_x, tmpwidth, tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glBindTexture (GL_TEXTURE_CUBE_MAP, idTex);
//    std::cout << "glBindTexture (GL_TEXTURE_CUBE_MAP, idTex);" << idTex << std::endl;
    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    delete[] image;

    image = loadPPM(negative_z, tmpwidth, tmpheight);

    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glBindTexture (GL_TEXTURE_CUBE_MAP, idTex);
    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB,	GL_UNSIGNED_BYTE, image);


    delete[] image;

    image = loadPPM(negative_y, tmpwidth, tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }
    drawSphere(10.,30,30);

    glBindTexture (GL_TEXTURE_CUBE_MAP, idTex);
    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);


    delete[] image;

    image = loadPPM(positive_x, tmpwidth, tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glBindTexture (GL_TEXTURE_CUBE_MAP, idTex);
    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB,	GL_UNSIGNED_BYTE, image);


    delete[] image;

    image = loadPPM(positive_y, tmpwidth, tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glBindTexture (GL_TEXTURE_CUBE_MAP, idTex);
    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);


    delete[] image;

    image = loadPPM(positive_z,tmpwidth,tmpheight);
    //image = loadPPM("textures/arch_posz.ppm",tmpwidth,tmpheight);
    if(image==0){
    std::cerr << "Erreur au chargement le l'image" << std::endl;
    exit(0);
    }

    glBindTexture (GL_TEXTURE_CUBE_MAP, idTex);
    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB,	GL_UNSIGNED_BYTE, image);


    delete[] image;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		// create a displaylist
    /*displayListId=glGenLists(1);
 //glBindTexture(GL_TEXTURE_CUBE_MAP,texturesid[3]);
 //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    if(displayListId==0) std::cerr << "erreur d'initialisation de la liste " << std::endl;
    glNewList(displayListId, GL_COMPILE);
  	
  		//drawSphere(10.,30,30);

      //drawSphereMap(10.0);
    glEndList();*/
    glBindTexture (GL_TEXTURE_CUBE_MAP, 0);



}

void CubeMap::display()
{
		drawSphere(10.,30,30);

 //glCallList(displayListId);
}
