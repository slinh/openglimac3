#ifndef __GAME_H__
#define __GAME_H__

#include "Scene.hpp"
#include "CubeMap.hpp"

class Game
{

public:
  static Game & Instance();
  
private:
  Game();

 
public:
  ~Game();
  
  void initGL();
  void display();
  void checkCurrentScene();
 
  inline const std::vector<Scene*> & getSceneList()const{ return sceneList; }
  inline std::vector<Scene*> & setSceneList(){ return sceneList; }
  
  inline const vector3df & getCamera()const{ return camera; }
  inline vector3df & setCamera(){ return camera; }

  inline const CubeMap * getChurch()const{ return church; }
  inline void setChurch(CubeMap * church){ this->church = church; }

  inline const CubeMap * getSky()const{ return sky; }
  inline void setSky(CubeMap * sky){ this->sky = sky; }
  
  inline const int & getCurrentScene()const{ return currentScene; }
  inline int & setCurrentScene(){ return currentScene; }

  inline void setProgramObject(GLhandleARB* program) { programObject=program; };
  inline GLhandleARB* getProgramObject() { return programObject; };

  inline void setInvMat(float* invmat) { this->invmat = invmat; };
  inline void setBiasmatrix(float* mat) { this->biasmatrix = mat; };

  inline void setShadowtexid(GLuint shadowtexid) { this->shadowtexid = shadowtexid; };
  inline GLuint getShadowtexid() { return shadowtexid; };

  inline void setShadowbufferid(GLuint shadowbufferid) { this->shadowbufferid = shadowbufferid; };

  inline void setWindowSize(int windowwidth, int windowheight) { this->windowwidth = windowwidth;
    this->windowheight = windowheight;};

  static void mat_inverse (float *in, float *out);
  void setContentHouse();
  
  GLfloat* getLightPosition();
  void displaySky(void);
  void initShadowGL();
  bool checkFramebufferStatus(void);
  void multMatrix4x4(float* m1, float* m2, float* res);
  void drawShadow(bool shaders = false);
  void shadowTest(void);

  void lightUp();
  void lightDown();
  void lightRight();
  void lightLeft();
  
  void lightFront();
  void lightBack();

protected:
  std::vector<Scene*> 	sceneList;

  vector3df 			camera;
  CubeMap*        church;
  CubeMap*        sky;
  int             currentScene;
  GLhandleARB* 		programObject;
  float*          invmat;

  unsigned int    shadowbufferid;
  float           angle;
  GLfloat lightprojectionmatrix[16];
  GLfloat lightmodelviewmatrix[16];
  GLfloat transformationmatrix[16];

  GLfloat shadowmatrix[16];
  GLuint shadowtexid;
  float*  biasmatrix;
  int windowwidth;
  int windowheight;



};

#endif
