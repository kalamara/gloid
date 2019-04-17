/* MAIN APPLICATION HEADER: defines application specific types, classes,
 * variables, constants, includes headers
 * and in general stuff that is used frequently but changes infrequently.*/

#ifndef _GLOID_H
#define _GLOID_H

//DEFINE DEBUG 1

#define APP_NAME     "GLoid II is SDL/OpenGL Arkanoid " // The App Name And Caption
#define APP_VERSION  0x020500

/****** CHANGELOG IN GLOID 2: **********************************************
 1. Loading textures via SDL                                              OK
 2. Sound via SDL.                                                        OK
 3. 3ds loading sith stdio                                                OK
 4. Timing and fps counting via SDL.                                      OK
 5. Preemptive collision detection and raycasting crosshairs              OK
 6. Particle System for explosions                                        OK
 8. Tetrahedron, Cube, Rhombicuboctahedron( ! ) with standard OpenGL      OK
 9. Redo hiscore system with stdio                                        OK
 11. Animate alien #4,                                                    OK
 13. Fix double balls bug                                                 OK
 14. Win32 installer                                                      OK
 15. Update html                                                          OK
 16. Upload, post                                                         OK
 17. Linux and Mac source packages thx. Thomas Kircher <tkircher@gnu.org> OK
 18. Thomas also fixed some other bugs                                    OK
 19. Transparent and 3d fonts (for pills)                                 OK
 **********GLOID 2.1: *****************************************************
 1. massive C++17 rewrite and unit tests
 2. cmake build system                                                    OK
 3. bugs and glitches fixed
****************************************************************************/


//GLoid core
#include "Point.h"
#include "Text.h"
#include "Engine.h"
#include "Step.h"
#include "World.h"
#include "Game.h"
#include "model/WhatUC.h"


#else



#endif  // _GLOID_H
