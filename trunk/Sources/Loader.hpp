#ifndef __LOADER_HPP__
#define __LOADER_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "ObjLoader.hpp"
#include "TextureLoader.hpp"

class Loader {
	
	protected:
		std::vector<TextureLoader*> textureList;
		std::vector<ObjLoader*> objList;
		
	public:
		Loader(){};
		~Loader(){};
		
		void addTextureLoader(TextureLoader* textureLoader);
		void addObjLoader(ObjLoader* objLoader);
	
		TextureLoader * getTextureLoader(unsigned int id)const;
		ObjLoader * getObjLoader(unsigned int id)const;
	
		void initTextures();
		void initObj();
};

#endif
