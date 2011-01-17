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
  inline CubeMap * setChurch(CubeMap * church){ this->church = church; }
  
  inline const int & getCurrentScene()const{ return currentScene; }
  inline int & setCurrentScene(){ return currentScene; }

  inline void setProgramObject(GLhandleARB* program) { programObject=program; };
  inline GLhandleARB* getProgramObject() { return programObject; };

  inline void setInvMat(float* invmat) { this->invmat = invmat; };

  static void mat_inverse (float *in, float *out);
  void setContentHouse();
  
  GLfloat* getLightPosition();
  
  void lightUp();
  void lightDown();
  void lightRight();
  void lightLeft();
  
  void lightFront();
  void lightBack();

protected:
  std::vector<Scene*> 	sceneList;

	vector3df 				    camera;	
  CubeMap*          church;
  int                      currentScene;
  GLhandleARB* 		programObject;
  float*                  invmat;

};

#endif
