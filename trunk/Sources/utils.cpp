#include "utils.hpp"



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


void checkGLError(int line)
{
  #ifdef CHECK_ERRORS
  GLenum err = glGetError();
  if(err!=GL_NO_ERROR){
    std::cerr << "Erreur GL "<<line<<" :" << std::endl;
    std::cerr << gluErrorString(err) << std::endl;
  }
  #endif
}

