#ifndef __OBJ_H__
#define __OBJ_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "Texture.hpp"
#include "Bbox.hpp"
#include "vector3d.h"

/* vector */
typedef float vec3_t[3];
typedef float vec4_t[4];


/* vertex */
typedef struct
{
  vec4_t xyzw;

} obj_vertex_t;


/* texture coordinates */
typedef struct
{
  vec3_t uvw;

} obj_texCoord_t;


/* normal vector */
typedef struct
{
  vec3_t ijk;

} obj_normal_t;


/* polygon */
typedef struct
{
  GLenum type;        /* primitive type */
  int num_elems;      /* number of vertices */

  int *vert_indices;  /* vertex indices */
  int *uvw_indices;   /* texture coordinate indices */
  int *norm_indices;  /* normal vector indices */

} obj_face_t;


typedef enum _typeObj
{
	HOUSE,
	SETTING
} TypeObj;

class Obj {
	
protected:
	int num_verts;              /* number of vertices */
	int num_texCoords;          /* number of texture coords. */
	int num_normals;            /* number of normal vectors */
	int num_faces;              /* number of polygons */

	int has_texCoords;          /* has texture coordinates? */
	int has_normals;            /* has normal vectors? */

	std::vector<obj_vertex_t *> 	vertices;	  /* vertex list        */
	std::vector<obj_texCoord_t *>	texCoords;	/* tex. coord. list   */
	std::vector<obj_normal_t *> 	normals;    /* normal vector list */
	std::vector<obj_face_t *> 		faces;      /* model's polygons   */

// textures
	std::vector<Texture *> 			  textures;
	
  Bbox *                        bbox;
	
	// infos XML
  std::string                   modelFileName;
  int                           id;
  GLint                         displayListId;
  
  int                           idShader;
  vector3df                     scale;
  vector3df						translate;
  vector3df                     axeRotate;
  float                         angleRotation;
  TypeObj						typeObj;
  int							associatedScene;
	
public:
  Obj();
	~Obj();
	
	int FirstPass (FILE *fp);
	int SecondPass (FILE *fp);
	int ReadOBJModel (const char *filename);
	void RenderOBJModel ();

/* getter - setters */
public:
  inline const int & getId() const { return id;}
  inline int & setId() { return id;}
  

  inline const Bbox & getBbox() const { return *bbox; }
  inline Bbox & setBbox() { return *bbox; }

  inline const std::string & getModelFileName() const { return modelFileName;}
  inline std::string & setModelFileName() { return modelFileName;}

  inline const std::vector<Texture *> & getTextures() const { return textures;}
  inline std::vector<Texture *> & setTextures() { return textures;}

  inline const vector3df & getScale() const { return scale;}
  inline vector3df & setScale() { return scale;}
  
  void setScale(float x, float y, float z);	
	
  inline const vector3df & getTranslate() const { return translate;}
  inline vector3df & setTranslate() { return translate;}

  void setTranslate(float x, float y, float z);		
	
  inline const vector3df & getAxeRotate() const { return axeRotate;}
  inline vector3df & setAxeRotate() { return axeRotate;}
	
  void setAxeRotate(float x, float y, float z);	

  inline const float & getAngleRotation() const { return angleRotation;}
  inline float  & setAngleRotation() { return angleRotation;}
  
  void setAngleRotation(float angle); 

  inline const TypeObj & getType() const { return typeObj;}
  inline TypeObj & setType() { return typeObj;}
  
  inline const int & getAssociatedScene() const { return associatedScene;}
  inline int & setAssociatedScene() { return associatedScene;}

  void initGL();
  void display();
  void displayList();
  void draw();

};


/* obj model structure 
typedef struct
{} obj_model_t;
*/



#endif
