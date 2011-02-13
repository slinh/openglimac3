#ifndef DRAW_HPP
#define DRAW_HPP
#include "vector3d.h"

void drawRepere(void);
void drawSphere(const float& radius,
    const unsigned int& stepx,
    const unsigned int& stepy);
void createCarre(void);
void createCarreTest(void);
void createCube(vector3df color = vector3df(1.0, 1.0, 1.0));

void createWall(void);
void createRoof(void);
void createGround(void);

#endif
