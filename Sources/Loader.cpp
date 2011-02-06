#include "Loader.hpp"

void Loader::addTextureLoader(TextureLoader* textureLoader)
{
	textureList.push_back(textureLoader);
}

void Loader::addObjLoader(ObjLoader* objLoader)
{
	objList.push_back(objLoader);
}

TextureLoader * Loader::getTextureLoader(unsigned int id)const
{
	if(id < textureList.size())
	{
		return textureList[id];
	}
	else return NULL;
}

ObjLoader * Loader::getObjLoader(unsigned int id)const
{
	if(id < objList.size())
	{
		return objList[id];
	}
	else return NULL;
}

void Loader::initTextures()
{
	for(unsigned int i=0; i<textureList.size(); ++i)
	{
		textureList[i]->create();
	}
}

void Loader::initObj()
{
	for(unsigned int i=0; i<objList.size(); ++i)
	{
		objList[i]->initGL();
	}
}
