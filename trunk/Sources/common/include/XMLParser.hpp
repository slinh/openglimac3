#ifndef DOMPARSER_H
#define DOMPARSER_H
#include "../../lib/tinyxml/tinyxml.h"
#include "../../Scene.hpp"


void loadXML(const char * pFileName, std::vector<Scene*> & sceneList, Loader & loader);


#endif
