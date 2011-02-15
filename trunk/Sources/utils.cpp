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
#include <assert.h>

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

void drawString(float x, float y, void* font, const std::string& str)
{
        assert(font);
        glPushAttrib(GL_ENABLE_BIT);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
//	glRasterPos2f(pos.x, pos.y);
        glWindowPos2f(x, y);

        for(GLuint i=0; i<str.size(); i++)
                glutBitmapCharacter(font, str[i]);

        glPopAttrib();
}
