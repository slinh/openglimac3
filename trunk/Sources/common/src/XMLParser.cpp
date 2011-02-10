#include "../../lib/tinyxml/tinyxml.h"
#include "../../Scene.hpp"
#include "../../Loader.hpp"
#include "../../Shaders.hpp"
#include "../../vector3d.h"
#include <iostream>
#include <string>

void loadXML(const char * pFilename, std::vector<Scene*> & sceneList, Loader & loader)
{
  TiXmlDocument doc(pFilename);
  if(!doc.LoadFile())
  {
      std::cerr << "erreur lors du chargement" << std::endl;
      std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
      return;
  }
  
  TiXmlHandle hdl(&doc);
  
  TiXmlElement* loaderTag = hdl.FirstChild( "demo" ).FirstChild( "loader" ).Element();
	TiXmlHandle hLoader = TiXmlHandle(loaderTag);
	
	// define texture
	TiXmlElement* textureLoaderTag = hLoader.FirstChild( "texload" ).Element();
	while(NULL != textureLoaderTag)
	{
		int texRepeat = -1 ;
		textureLoaderTag->QueryIntAttribute("repeat", &texRepeat);
      
		std::string textureLoaderName = std::string(textureLoaderTag->GetText());
		TextureLoader* tmpTextureLoader;
		
		if( texRepeat > 0)
		{	
			tmpTextureLoader = new TextureLoader(textureLoaderName, true);
		}
		else
		{
			tmpTextureLoader = new TextureLoader(textureLoaderName);
		}
		
		loader.addTextureLoader(tmpTextureLoader);
		
		textureLoaderTag=textureLoaderTag->NextSiblingElement("texload");
	
	}
	
	// define obj
	TiXmlElement* objLoaderTag = hLoader.FirstChild( "objload" ).Element();
	while(NULL != objLoaderTag)
	{
		std::string objLoaderName = std::string(objLoaderTag->GetText());
		ObjLoader* tmpObjLoader = new ObjLoader(objLoaderName);
	
		loader.addObjLoader(tmpObjLoader);
		
		objLoaderTag=objLoaderTag->NextSiblingElement("objload");
		
	}

  
  TiXmlElement* sceneTag = hdl.FirstChild( "demo" ).FirstChild( "scene" ).Element();
  // first scene is main
  TypeScene typeS = MAIN;
  
  std::cout << "lecture xml" << std::endl;

  while(sceneTag != NULL)
  {   
    TiXmlHandle hScene = TiXmlHandle(sceneTag);

    // create a new scene and insert to global scene
    Scene* scene = new Scene(typeS);
    sceneList.push_back(scene);

    TiXmlElement* shaderTag = hScene.FirstChild( "shader" ).Element();
    while(NULL != shaderTag)
    {

			
			std::string value = std::string(shaderTag->GetText());

			//TODO : complete with all kinds of shaders
			if(value == "PARALLAX")
				scene->setTypeShader() = PARALLAX;
			else if(value == "ENV")
				scene->setTypeShader() = ENV;
			else if(value == "CUBEMAP")
				scene->setTypeShader() = CUBEMAP;
			else if(value == "NORMALSPEC")
				scene->setTypeShader() = NORMALSPEC;
			else if(value == "TOON")
				scene->setTypeShader() = TOON;
			else if(value == "SHADOW")
				scene->setTypeShader() = SHADOW;
	//		else if(value == "BUMP")
	//			scene->setTypeShader() = BUMP;
			else if(value == "ALPHA")
				scene->setTypeShader() = ALPHA;
			
			shaderTag = shaderTag->NextSiblingElement("shader");
    }

    
    // light
    TiXmlElement* lightTag = hScene.FirstChild( "light" ).Element();
    float xl, yl, zl, wl;
    if(lightTag != NULL)
    {
      lightTag->QueryFloatAttribute("x", &xl);
      lightTag->QueryFloatAttribute("y", &yl);
      lightTag->QueryFloatAttribute("z", &zl);
      lightTag->QueryFloatAttribute("w", &wl);
      
      scene->setLightPosition(xl, yl, zl, wl);
    }
    
    // tinylight
    TiXmlElement* tinylightTag = hScene.FirstChild( "tinylight" ).Element();
    float sizetl, xtl, ytl, ztl, pastl;
    if(tinylightTag != NULL)
    {
			tinylightTag->QueryFloatAttribute("size", &sizetl);
      tinylightTag->QueryFloatAttribute("x", &xtl);
      tinylightTag->QueryFloatAttribute("y", &ytl);
      tinylightTag->QueryFloatAttribute("z", &ztl);
      tinylightTag->QueryFloatAttribute("pas", &pastl);
      
      scene->setTinyLightPosition(sizetl, xtl, ytl, ztl);
      scene->setTinyLightPas(pastl);
      scene->setTinyLightActive(true);
    }
    
    
    TiXmlElement* objTag = hScene.FirstChild( "obj" ).Element();

    while(NULL != objTag)
    {
      TiXmlHandle hObj = TiXmlHandle(objTag);

			int objLoaderId;
      objTag->QueryIntAttribute("objid", &objLoaderId);

			// get the good model in loader :
			ObjLoader* tmpObjLoader = loader.getObjLoader(objLoaderId);
     
      Obj * object = new Obj(tmpObjLoader);

      int idObj;
      objTag->QueryIntAttribute("id", &idObj);
      object->setId() = idObj;
      
      int idAssociatedScene = -1 ;
      objTag->QueryIntAttribute("scene", &idAssociatedScene);
      object->setAssociatedScene() = idAssociatedScene;

			if( idAssociatedScene > 0 ) 
			{
				object->setType() = HOUSE;
			}
			else
			{
				object->setType() = SETTING;
			}
		
      

      TiXmlElement* modelTag = hObj.FirstChild( "model" ).Element();

      scene->setObjList().push_back(object);


      TiXmlElement* scaleTag = hObj.FirstChild( "scale" ).Element();
//      std::cout << scaleTag->Value() << std::endl;

      // set the scale of the object
      float x, y, z;
      scaleTag->QueryFloatAttribute("x", &x);
      scaleTag->QueryFloatAttribute("y", &y);
      scaleTag->QueryFloatAttribute("z", &z);
		
		vector3df pouet = vector3df(x, y, z);	
		
		object->setScale(x, y, z);
//      std::cout << "float " << x << ", "<< y << ", " << z << std::endl;	
		//std::cout << "pouet " << pouet.X << " i " << pouet.Y << std::endl;
//		std::cout << "debdheb " << object->getScale().X << " i " << object->getScale().Y << std::endl;

      TiXmlElement* rotationTag = hObj.FirstChild( "rotation" ).Element();
//      std::cout << rotationTag->Value() << std::endl;

      // set the scale of the object
      float angle;
      rotationTag->QueryFloatAttribute("x", &x);
      rotationTag->QueryFloatAttribute("y", &y);
      rotationTag->QueryFloatAttribute("z", &z);
      rotationTag->QueryFloatAttribute("angle", &angle);
      object->setAxeRotate(x, y, z);
      object->setAngleRotation(angle);
//      std::cout << "float " << x << ", "<< y << ", " << z << ", angle : " << angle << std::endl;


      TiXmlElement* translateTag = hObj.FirstChild( "translate" ).Element();
//      std::cout << scaleTag->Value() << std::endl;

      // set the scale of the object
      translateTag->QueryFloatAttribute("x", &x);
      translateTag->QueryFloatAttribute("y", &y);
      translateTag->QueryFloatAttribute("z", &z);
			object->setTranslate(x, y, z);

      TiXmlElement* texTag = hObj.FirstChild( "texture" ).Element();
      while(NULL != texTag)
      {      
				int texLoaderId;
				texTag->QueryIntAttribute("texid", &texLoaderId);

				// get the good model in loader :
				TextureLoader* tmpTextureLoader = loader.getTextureLoader(texLoaderId);

        // create texture files and push to the object texture list
        int id;
        texTag->QueryIntAttribute("id", &id);
        std::string str = texTag->GetText();
         
        //Texture * texture = new Texture(id, str);
        Texture * texture = new Texture(id, tmpTextureLoader);
        object->setTextures().push_back(texture);

        texTag=texTag->NextSiblingElement("texture");
      }
      
		
      objTag=objTag->NextSiblingElement("obj");
    }
    
    typeS = OTHER;
    sceneTag=sceneTag->NextSiblingElement("scene");
  }
  
  
    std::cout << "FIN lecture xml" << std::endl;
}
