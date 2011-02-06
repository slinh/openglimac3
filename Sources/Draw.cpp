#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include "Draw.hpp"

void createCarre(void)
{

  glBegin(GL_QUADS);
//  glColor3f(1.0f,1.0f,1.0f);
  glNormal3f(0.0f,0.0f,1.0f);
  glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 0.0f, 0.0f);
  glVertex3f(-0.5f, 0.5f, 0.0f);
  glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 4.0f);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 0.0f, 0.0f);
  glVertex3f(-0.5f,-0.5f, 0.0f);
  glMultiTexCoord2f(GL_TEXTURE0, 4.0f, 4.0f);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 0.0f, 0.0f);
  glVertex3f( 0.5f,-0.5f, 0.0f);
  glMultiTexCoord2f(GL_TEXTURE0, 4.0f, 0.0f);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 0.0f, 0.0f);
  glVertex3f( 0.5f, 0.5f, 0.0f);
  glEnd();


  
}

void createCarreTest(void)
{

  glBegin(GL_QUADS);
//  glColor3f(1.0f,1.0f,1.0f);
  glNormal3f(0.0f,0.0f,1.0f);
  glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 0.0f, 0.0f);
  glVertex3f(-0.5f, 0.5f, 0.0f);
  glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 0.0f, 0.0f);
  glVertex3f(-0.5f,-0.5f, 0.0f);
  glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 0.0f, 0.0f);
  glVertex3f( 0.5f,-0.5f, 0.0f);
  glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 0.0f, 0.0f);
  glVertex3f( 0.5f, 0.5f, 0.0f);
  glEnd();



}

void createCube(vector3df color)
{
	
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_LIGHTING);
	
	//glColor4f(0.19,0.14,0.15,0.1);
//	glColor3f(color.X, color.Y, color.Z);
	
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.5);	
    glRotatef(180.0,1.0,0.0,0.0);
    createCarre();
	glPopMatrix();

	//Carre vert du bas
	glPushMatrix();
		glTranslatef(0.0, -0.5, 0.0);
		glRotatef(-90.0,1.0,0.0,0.0);
		createCarre();
	glPopMatrix();
	
	//Carre bleu-violet du haut
	glPushMatrix();
		glTranslatef(0.0, 0.5, 0.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		createCarre();
	glPopMatrix();
	
	//Carre gris noir de droite
	glPushMatrix();
		glTranslatef(0.5, 0.0, 0.0);
		glRotatef(-90,0.0,1.0,0.0);
		createCarre();
	glPopMatrix();
	
	//Carré orange de gauche
	glPushMatrix();
		//Rotation de 90 degres
		glTranslatef(-0.5, 0.0, 0.0);
		glRotatef(90,0.0,1.0,0.0);
		createCarre();
	glPopMatrix();
	
	//Carre violet du fond
	glPushMatrix();	
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(0.0,0.0,1.0,0.0);
		createCarre();
	glPopMatrix();

}

void createWall(void)
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.5);	
    glRotatef(180.0,1.0,0.0,0.0);
    createCarre();
	glPopMatrix();
	
	/*
	//Carre vert du bas
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0);
	glRotatef(-90.0,1.0,0.0,0.0);
	createCarre();
	glPopMatrix();
	*/
	/*
	//Carre bleu-violet du haut
	glPushMatrix();
	glTranslatef(0.0, 0.5, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	createCarre();
	glPopMatrix();*/
	
	//Carre gris noir de droite
	glPushMatrix();
	glTranslatef(0.5, 0.0, 0.0);
	glRotatef(-90,0.0,1.0,0.0);
	createCarre();
	glPopMatrix();
	
	//Carré orange de gauche
	glPushMatrix();
	//Rotation de 90 degres
	glTranslatef(-0.5, 0.0, 0.0);
	glRotatef(90,0.0,1.0,0.0);
	createCarre();
	glPopMatrix();
	
	//Carre violet du fond
	glPushMatrix();	
	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(0.0,0.0,1.0,0.0);
	createCarre();
	glPopMatrix();
}

void createRoof(void)
{
	//Carre bleu-violet du haut
	glPushMatrix();
	glTranslatef(0.0, 0.5, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	createCarre();
	glPopMatrix();
}

void createGround(void)
{
	//Carre vert du bas
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0);
	glRotatef(-90.0,1.0,0.0,0.0);
	createCarre();
	glPopMatrix();
}

void drawSphere(const float& radius,
    const unsigned int& stepx,
    const unsigned int& stepy)
{
  glDisable(GL_CULL_FACE);
  float anglex = 2. * M_PI / float(stepx);
  float angley = M_PI / float(stepy);

  float texx = 1.0 / float(stepx);
  float texy = 1.0 / float(stepy);

  for(unsigned int i = 0; i < stepy; ++i){
    glBegin(GL_TRIANGLE_STRIP);
    for(unsigned int j = 0; j < stepx + 1; ++j){
      glMultiTexCoord2f(GL_TEXTURE0, texx*j,(texy)*i);
      float x = radius*sin(angley*i)*sin(anglex*j);
      float y = radius*cos(angley*i);
      float z = radius*sin(angley*i)*cos(anglex*j);
      float norm = sqrtf(x*x+y*y+z*z);
      glNormal3f(x/norm,y/norm,z/norm);
      glVertex3f(x, y, z);
      x = radius*sin(angley*(i+1))*sin(anglex*j);
      y = radius*cos(angley*(i+1));
      z = radius*sin(angley*(i+1))*cos(anglex*j);
      glMultiTexCoord2f(GL_TEXTURE0, texx*j,(texy)*(i+1));
      glNormal3f(x/norm,y/norm,z/norm);
      glVertex3f(x, y, z);
    }
    glEnd();
  }
  glEnable(GL_CULL_FACE);

}
