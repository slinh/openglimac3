#ifndef __SCENE_HPP__
#define __SCENE_HPP__

//#define USE_GEOMETRY_SHADER
#ifdef __APPLE__
	#include "./glew.h"
	
#else
	#include <GL/glew.h>
#endif

#include <iostream>
#include <string>
#include <vector>

#include "Shaders.hpp"
#include "Obj.hpp"
#include "HeightField.hpp"


typedef enum _typeScene
{
	MAIN,
	OTHER
}TypeScene;

class Scene
{
  public:
    Scene():typeS(OTHER), typeShader(PARALLAX), objList(NULL){}
	
	Scene(TypeScene type);
      
    Scene(const TypeShader & typeShader)
      :typeShader(typeShader){};  
    
    ~Scene();
    
    void draw();
    void loadXML(std::string xmlFile);
    
    inline const TypeShader & getTypeShader() const {return typeShader;};
    inline TypeShader & setTypeShader() {return typeShader;};

    inline const TypeScene & getTypeScene() const {return typeS;};
    inline TypeScene & setTypeScene() {return typeS;};
    
    inline const std::vector<Obj*> & getObjList() const {return objList;};
    inline std::vector<Obj*> & setObjList() {return objList;};	
	
    inline void setContentHouse(Obj* obj) { contentHouse = obj; };
    inline const Obj & getContentHouse() { return *contentHouse; };

    inline GLfloat* getLightPosition(){ return lightPosition; };
    void setLightPosition(float x, float y, float z, float w );

    void lightUp();
    void lightDown();
    void lightRight();
    void lightLeft();
    void lightFront();
    void lightBack();

    void initGL();
    void display();
    
  protected:
    TypeScene 			    typeS;
    TypeShader 			    typeShader;    //  shader
    std::vector<Obj*> 	objList;       //  obj models vector
    HeightField* 		    heightfield;   //  map of the field
	  Obj*			          contentHouse;  // house    
    GLfloat*				    lightPosition;
	
//    SphereMap * sphereMap;      //  spheremap of the field    
	
    
  
};

#endif
