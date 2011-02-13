#ifndef __GAME_H__
#define __GAME_H__

#include "Scene.hpp"
#include "CubeMap.hpp"
#include "Loader.hpp"

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
  
  inline const Loader & getLoader()const{ return *loader; }
  inline Loader & setLoader(){ return *loader; }
  
  inline const vector3df & getCamera()const{ return camera; }
  inline vector3df & setCamera(){ return camera; }

  inline const vector3df & getDirection()const{ return direction; }
  inline vector3df & setDirection(){ return direction; }

  inline const CubeMap * getChurch()const{ return church; }
  inline void setChurch(CubeMap * church){ this->church = church; }

  inline const CubeMap * getSky()const{ return sky; }
  void setSky(CubeMap * sky);
  
  inline const int & getCurrentScene()const{ return currentScene; }
  inline int & setCurrentScene(){ return currentScene; }
  
  inline const float & getTimer()const{ return timer; }
  inline float & setTimer(){ return timer; }

  inline void setProgramObject(GLhandleARB* program) { programObject=program; };
  inline GLhandleARB* getProgramObject() { return programObject; };

  inline void setInvMat(float* invmat) { this->invmat = invmat; };

  inline GLfloat* setLightprojectionmatrix() { return lightprojectionmatrix; };

  inline void setShadowtexid(GLuint shadowtexid) { this->shadowtexid = shadowtexid; };
  inline GLuint getShadowtexid() { return shadowtexid; };

  inline GLuint getDisplayListId() {return displayListId; };

  inline void setShadowbufferid(GLuint shadowbufferid) { this->shadowbufferid = shadowbufferid; };

  inline void setWindowSize(int windowwidth, int windowheight) { this->windowwidth = windowwidth;
  this->windowheight = windowheight;};

  static void mat_inverse (float *in, float *out);
  void setContentHouse();
  
  GLfloat* getLightPosition();
  void displaySky(void);
  void initShadowGL();
  void FBO();
  void displayMatrix(GLfloat * m);
  bool checkFramebufferStatus(void);
  void multMatrix4x4(float* m1, float* m2, float* res);
  void drawShadow(bool shaders = false);
  void drawShadow2(bool shaders = false);
  void displayShadow(void);
  void displayFBO(void);
	void idleGL(void);
  void lightUp();
  void lightDown();
  void lightRight();
  void lightLeft();
  
  void lightFront();
  void lightBack();
  void initTripleBillboard();

protected:
  std::vector<Scene*> 	sceneList;
  Loader*				loader;

  vector3df 			camera;
  vector3df 			direction;

  CubeMap*        church;
  CubeMap*        sky;
  int             currentScene;
  GLhandleARB* 		programObject;
  float*          invmat;

  GLuint   shadowbufferid;
  float           angle;
  GLfloat lightprojectionmatrix[16];
  GLfloat lightmodelviewmatrix[16];
  GLfloat transformationmatrix[16];
  GLfloat shadowmatrix[16];
  GLfloat biasmatrix[16];

  GLuint shadowtexid;
  int windowwidth;
  int windowheight;

	//Timer
	float timer;
  GLuint displayListId;
  GLuint idTexBillboard;

};

#endif
