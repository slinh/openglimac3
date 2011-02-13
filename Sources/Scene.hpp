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
    inline Obj & setContentHouse() { return *contentHouse; };
    inline const Obj & getContentHouse() { return *contentHouse; };

    inline GLfloat* getLightPosition(){ return lightPosition; };
    void setLightPosition(float x, float y, float z, float w );

    inline GLfloat* getTinyLightPosition(){ return tinyLightPosition; };
    void setTinyLightPosition(float size, float x, float y, float z);
    
    inline const float & getTinyLightSize() { return tinyLightSize; };
    
    inline const float & getTinyLightPas(){ return tinyLightPas; };
    void setTinyLightPas(float pas);

    inline const float & getLightPas(){ return lightPas; };

	  inline const float & getTinyLightRadian(){ return tinyLightRadian; };
    inline float & setTinyLightRadian(){ return tinyLightRadian; };

	  inline const float & getLightRadian(){ return lightRadian; };
    inline float & setLightRadian(){ return lightRadian; };

    inline const bool & getTinyLightActive()const{ return isTinyLightActive; };
    void setTinyLightActive(bool b);
        
		void updateLight(void);

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
    
		// Tiny Light
    GLfloat*						tinyLightPosition;
    float								tinyLightSize;
    float								tinyLightRadian;
    float								lightRadian;
    float 							lightPas;
    float								tinyLightPas;
    bool								isTinyLightActive;
    
    
	
//    SphereMap * sphereMap;      //  spheremap of the field    
	
    
  
};

#endif
