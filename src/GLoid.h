/* MAIN APPLICATION HEADER: defines application specific types, classes,
 * variables, constants, includes headers
 * and in general stuff that is used frequently but changes infrequently.*/

#ifndef _GLOID_H
#define _GLOID_H
#define DEBUG 1

// Constants

#define PI 3.141525f
#define ROOT2 1.414213f
#define ROOT3 1.732051f

#define FLOAT_PRECISION 0.000001f
#define ONE 1.0f
#define ZERO 0.0f
#define HALF_CIRCLE 180.0f

//#define GREEN       {0.0f, 1.0f, 0.0f}
//#define BLUE        {0.0f, 0.0f, 1.0f}
//#define GRAY        {0.3f, 0.3f, 0.3f}
//#define WHITE       {1.0f, 1.0f, 1.0f}
//#define YELLOW      {1.0f, 1.0f, 0.0f}
//#define CYAN        {0.5f, 0.5f, 0.1f}
//#define MAGENTA     {1.0f, 0.0f, 1.0f}
//#define BLACK       {0.0f, 0.0f, 0.0f}
//#define PALE_GRAY   {0.5f, 0.5f, 0.5f}

//*****->to Engine*****/
// Number of sound channels (Stereo)
#define NUM_BUFFERS 2

#define GRAVITY	10.0f

#define SCENE_MAX   17.5f // Up, right, in
#define	SCENE_MIN  -17.5f // Down, left, out
#define SCENE_AIR   11.7f // Camera distance

//*****->to Game*****/
#define ALIENHOME  3  // Where aliens spawn
#define BALLNUM    4  // Maximum number of balls in play
#define ALIENNUM  10  // Maximum number of aliens
#define SHOTNUM   32  // Maximum number of shots

// Dimensions of level, in bricks
#define INTX   6
#define INTY   6
#define INTZ  13

#define BRIKTYPES   2  // Types of bricks
#define LEVELTYPES  4  // Types of levels
#define MAXLEVEL   11  // Maximum level

#define SCORELIFE  100000  // The score required for an extra life
#define SCOREBONUS   1000  // Score if you lose or catch a pill
#define SCOREWIN    10000  // Score if you pass a level
#define SCOREALIEN    100  // Score if you kill an alien
#define SCOREBRICK     70  // Score if you break a brick

#define APP_NAME     "GLoid II is SDL/OpenGL Arkanoid " // The App Name And Caption
#define APP_VERSION  0x020500

//#define TEXTLINES  16  // Maximum number of lines of tex
//#define TRUE  1
//#define FALSE 0
//#define BOOL unsigned int
/*
#ifndef WIN32	
 #define USHORT unsigned short
 #define MAX_PATH 256
//TODO: can we use C++17 <filesystem> here?
 #define WORKPATH "/usr/local/share/gloid"
#else 
 #define WORKPATH "."
#endif
*/
#define MINSPEED  1.0f//minimum speed on any axis = 5% of initial speed on Z
// Maximum string length
#define MAXLINE 256

#define VAUS_PARTICLES  128  //asplozans!
#define ALIEN_PARTICLES 1024

#define VAUS_COLORS  4
#define ALIEN_COLORS 3

// Libraries
#include <list>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

// If we're under MSVC, tell the linker to look for these libraries.
#ifdef WIN32
 #pragma comment(lib, "OpenGL32.lib")
 #pragma comment(lib, "GLu32.lib")
 #pragma comment(lib, "SDLmain.lib")
 #pragma comment(lib, "SDL.lib")
#endif

// Includes
#ifdef WIN32
 #include <windows.h>
#else
#include <stdarg.h>
#include <ctype.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef __MACOSX__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_audio.h>

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
 1. massive rewrite and unit tests                                        OK
 2. cmake build system                                                    OK
 3. bugs and glitches fixed
****************************************************************************/

// ENUMERATIONS

// Colors
typedef enum{
   RGB_GREY,
   RGB_PINK,
   RGB_BLUE,
   RGB_RED,
   RGB_GREEN,
   RGB_MAGENTA,
   RGB_ORANGE,
   RGB_YELLOW,
   RGB_WHITE,
   RGB_BLACK,
   N_RGB_COLORS
}RGB_COLORS;

// Bitmaps
enum{
   BMP_BG_1,
   BMP_BG_2,
   BMP_BG_3,
   BMP_BG_4,
   BMP_LOGO,
   BMP_VAUS,
   BMP_TARGET,
   BMP_CROSSHAIRS,
   BMP_WARP,
   N_BMP
};

// Sounds
enum{
   WAV_ALIEN,
   WAV_BOUNCE0,
   WAV_BOUNCE1,
   WAV_ENLARGE,
   WAV_GO,
   WAV_INTRO,
   WAV_KANONAKI,
   WAV_LAUNCH,
   WAV_LOSE,
   WAV_SHOT,
   WAV_WARP,
   N_WAV 
};

// Parser tokens
enum{
   PARSE_BRIK,
   PARSE_POS,
   PARSE_COL
};

// Messages
enum{
   HUD_1UP,
   HUD_SCORE,
   HUD_HILABEL,
   HUD_HISCORE,
   HUD_LEVEL,
   HUD_GO,
   HUD_PAWS,
   HUD_FPS,
   N_HUD
};

typedef struct hallentry
{
   char* initials;
   int score;
} * hallentry_t;

//GLoid core
#include "Point.h"
#include "World.h"
#include "Text.h"
#include "Step.h"
#include "Engine.h"
#include "Game.h"

#else

// Function prototypes

// Basic functions

// Terminate the application
void TerminateApplication(void);

// General display functions

void ToggleFullscreen(void);  // Toggle Fullscreen/Windowed Mode
BOOL TogglePause(BOOL);
BOOL CreateWindowGL(int, int, int, Uint32);  // Create the OpenGL window
void ReshapeGL(int, int);
void updateHUD();

// Initialization
BOOL InitGL(SDL_Surface *);  // Initialize OpenGL scene
BOOL Initialize();           // Initialize game
int initFont(int);
void cleanText();
void initSound();
BOOL InitErrorLog();   // Initialize The Error Log
void CloseErrorLog();  // Close The Error Log
void Deinitialize();   // Uninitialize game

// Core game engine
int Update(Uint32, Uint8*, mousecntl*); // Update motion
void Draw(SDL_Surface *, Uint32);       // Draw the game scene
int GameLogic();                        // Run game logic

void GameLoading();
BOOL GameReady( );
BOOL GameHalloFame( );
void GameIntro();
void GameGo(int);
BOOL GameOver(Uint32);
BOOL GameWin(Uint8 *);
void resetlevel(BOOL);
void divideballs();
void shoot();

// Text display functions
void printText(BOOL, text2d*, SDL_Color, SDL_Color, int, int, const char* buf, ...);
void DrawSDLText(SDL_Surface*, text2d*);
void DrawOpenGLText(text2d*);

// Given an int, calculate the next-largest power of two
int nextpoweroftwo(int);

// Calculate a moving average
double moving_average(double, double*, int);


// Clean up data structures
void cleanup(void);

// High score functions
int  get_hiscore(void);
void set_hiscore(char* initials);

// Log a message to the log file
int Log(const char*, ...);

// Loading functions

// Load textures
BOOL LoadTextures(void);
BOOL LoadTextureBMP(char* file, USHORT texture_id, int num_texture);

// Load levels
int generateLevel(int);

// Load high scores
void loadScore(void);

// Load the display
void loading_display(char *);
void ready_display(SDL_Surface *);

// Load sounds
int loadSounds(void);
int loadText(const char * , char **);

// Display functions
void display_background(void);
void display_lives(SDL_Surface *);
void display_warp(void);
void display_cross(int , point3f, int);
void null_display(SDL_Surface * );
#endif  // _GLOID_H
