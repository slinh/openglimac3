#Project type
TEMPLATE = app

#Binary
TARGET = tequilapin


#Sources dir
DEPENDPATH += . 

#Includes dirs
INCLUDEPATH += .

#Where to build
DESTDIR +=   

#QT additional config
CONFIG+= qt opengl stl exceptions thread warn_on debug #debug_and_release
#QT config
QT += core xml gui opengl

#DISTFILES
DISTFILES +=   

#Defines
DEFINES +=

#Added libs
LIBS += -ldl -lGL -lGLU -lGLEW -lglut -L 



#Where to build MOCs
MOC_DIR = src/AUTOGEN

#Where to build .o
OBJECTS_DIR = .o

#Where to build .hpp from .ui
UI_DIR = 

#Where to build .cpp from .qrc
RCC_DIR = 


#Headers
HEADERS += Obj.hpp \
		   Bbox.hpp \
		   Texture.hpp \
		   vector3d.h \
       ppm.hpp \
       HeightField.hpp \
       Scene.hpp \
       Game.hpp \
       Shaders.hpp \
       CubeMap.hpp \
       lib/tinyxml/tinyxml.h \
       lib/tinyxml/tinystr.h \
       common/include/XMLParser.hpp \
       Draw.hpp \
       main.hpp \
       utils.hpp \
       TextureLoader.hpp \
       ObjLoader.hpp \
       Loader.hpp \
       Timer.hpp \
       Singleton.h \
       Billboard.hpp \
       TripleBillboard.hpp \

#UIs
FORMS += 

#Ressources
RESOURCES = 

#Sources
SOURCES += Obj.cpp \
		   Bbox.cpp \
       Texture.cpp \
       ppm.cpp \
       HeightField.cpp \
       Scene.cpp \
       Game.cpp \
       Shaders.cpp \
       CubeMap.cpp \
       lib/tinyxml/tinyxml.cpp \
       lib/tinyxml/tinystr.cpp \
       lib/tinyxml/tinyxmlerror.cpp \
       lib/tinyxml/tinyxmlparser.cpp \
       common/src/XMLParser.cpp \
       Draw.cpp \
       main.cpp \
       utils.cpp \
       TextureLoader.cpp \
       ObjLoader.cpp \
       Loader.cpp \
       Timer.cpp \
       Billboard.cpp \
       TripleBillboard.cpp \
