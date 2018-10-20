/* MAIN APPLICATION HEADER: defines application specific types, classes,
 * variables, constants, includes headers
 * and in general stuff that is used frequently but changes infrequently.*/

#ifndef _GLOID_H
#define _GLOID_H
#define DEBUG 1

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

#include "Engine.h"
#include "Step.h"
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
