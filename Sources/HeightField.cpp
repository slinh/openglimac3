#include "Game.hpp"
#include "HeightField.hpp"
#include "TextureLoader.hpp"
#include <stdio.h>
#include <string>
#include <vector>

HeightField::HeightField(std::string hFileName, const int hWidth, const int hHeight) : height(hHeight), width(hWidth), filename(hFileName)
{
        scale = vector3df(0.05, 0.03, 0.05);
}

HeightField::~HeightField()
{
	for( unsigned int i=0; i<textures.size(); ++i )
	{
		delete textures[i];
	}
}

void HeightField::init(void){	

	FILE *fp;
	
	fp = fopen(filename.c_str(), "rb");
	fread(hHeightField, 1, width * height, fp);
	fclose(fp);



	Game & game = Game::Instance();
	TextureLoader* tmpLoader = game.getLoader().getTextureLoader(24);
			
	Texture * tmp = new Texture(0, tmpLoader);
	textures.push_back(tmp);
	
	// create displaylist
	displayListId=glGenLists(1);
	if(displayListId==0)
		std::cerr << "erreur d'initialisation de la liste " << std::endl;
	glNewList(displayListId, GL_COMPILE);
		draw();
	glEndList();
	
}
void HeightField::draw(void){
    int nIndex = 0;
    float flX;
    float flZ;
    //VBO
    hLOD = 8;
    vhVertexCount = (int)(height * height * 6) / (hLOD * hLOD);
    std::cout << "vhVertexCount : " << vhVertexCount << std::endl;
    vhVertices = new Vert[vhVertexCount];
    vhTexCoords = new TexCoord[vhVertexCount];

    for (int hMapX = 0; hMapX < width; hMapX+=hLOD){
        for (int hMapZ = 0; hMapZ < height; hMapZ+=hLOD){
            for (int nTri = 0; nTri < 6; nTri++){
                flX = (float)hMapX + ((nTri == 1 || nTri == 2 || nTri == 5) ? hLOD : 0);
                flZ = (float)hMapZ + ((nTri == 2 || nTri == 4 || nTri == 5) ? hLOD : 0);

                vhVertices[nIndex].x = flX;
                vhVertices[nIndex].y = hHeightField[(int)flX][(int)flZ];
                vhVertices[nIndex].z = flZ;

                vhTexCoords[nIndex].u = flX / 512;
                vhTexCoords[nIndex].v = flZ / 512;
                nIndex++;
            }
        }
    }

    glGenBuffersARB(1, &vhVBOVertices);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vhVBOVertices);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vhVertexCount * 3 * sizeof(float), vhVertices, GL_STATIC_DRAW_ARB);

    glGenBuffersARB(1, &vhVBOTexCoords);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vhVBOTexCoords);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vhVertexCount * 2 * sizeof(float), vhTexCoords, GL_STATIC_DRAW_ARB);

    delete [] vhVertices;
    vhVertices = NULL;

    delete [] vhTexCoords;
    vhTexCoords = NULL;
     //VBO


    glDisable(GL_CULL_FACE);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    for( unsigned int i=0; i<textures.size(); ++i )
    {
        //glActiveTexture(GL_TEXTURE0);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        textures[i]->bind();
    }

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vhVBOTexCoords);
    glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vhVBOVertices);
    glVertexPointer(3, GL_FLOAT, 0, (char *) NULL);
    std::cout << "vhVertexCount : " << vhVertexCount << std::endl;
    glPushMatrix();
        //X->x, Y->z, Z->y
        glTranslatef(-13., -.6, -13.);
        glScalef(scale.X, scale.Y, scale.Z);
        glDrawArrays(GL_TRIANGLES, 0, vhVertexCount);
    glPopMatrix();
    std::cout << "Test : "<< std::endl;
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

/*void HeightField::draw(void){
	
        #ifndef __NO_SHADER__
		//glUseProgramObjectARB(0);
	#endif
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, tID[0]);
	for( unsigned int i=0; i<textures.size(); ++i )
	{
	//	glActiveTexture(GL_TEXTURE0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		textures[i]->bind();
	}
	
	
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	
	glPushMatrix();
	//X->x, Y->z, Z->y
	glTranslatef(-13., -.6, -13.);
	glScalef(scale.X, scale.Y, scale.Z);
	
	
	for (int hMapX = 0; hMapX < width; hMapX++){
        for (int hMapZ = 0; hMapZ < height; hMapZ++){
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f((float)hMapX / width, (float)hMapZ / height);
			glVertex3f(hMapX, hHeightField[hMapX][hMapZ], hMapZ);
			glTexCoord2f((float)hMapX / width, (float)(hMapZ + 1) / height) ;
			glVertex3f(hMapX, hHeightField[hMapX][hMapZ + 1], hMapZ + 1);
			glTexCoord2f((float)(hMapX + 1) / width, (float)hMapZ / height);
			glVertex3f(hMapX + 1, hHeightField[hMapX + 1][hMapZ], hMapZ);
			glTexCoord2f((float)(hMapX + 1) / width, (float)(hMapZ + 1) / height);
			glVertex3f(hMapX + 1, hHeightField[hMapX + 1][hMapZ + 1], hMapZ + 1);
			glEnd();
		}
	}
	glPopMatrix();
	
    for( unsigned int i=0; i<textures.size(); ++i )
    {
      textures[i]->unbind();
    }

	glDisable(GL_TEXTURE_2D);
}*/

void HeightField::display(void){
	glCallList(displayListId);
}
