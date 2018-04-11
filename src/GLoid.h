/* MAIN APPLICATION HEADER: defines application specific types, classes, variables, constants, includes headers.*/

#ifndef _GLOID_H
#define _GLOID_H

// Constants

#define PI 3.141525
#define ROOT2 1.414213

#define FLOAT_PRECISION 0.000001
#define ONE 1.0f
#define ZERO 0.0f
#define FULL_CIRCLE 360.0f;

#define RED     {1.0f, 0.0f, 0.0f}
#define GREEN   {0.0f, 1.0f, 0.0f}
#define BLUE    {0.0f, 0.0f, 1.0f}
#define GRAY    {0.3f, 0.3f, 0.3f}
#define WHITE   {1.0f, 1.0f, 1.0f}
#define YELLOW  {1.0f, 1.0f, 0.0f}
#define CYAN    {0.5f, 0.5f, 0.1f}
#define MAGENTA {1.0f, 0.0f, 1.0f}
#define BLACK   {0.0f, 0.0f, 0.0f}
#define PALE_GRAY    {0.5f, 0.5f, 0.5f}


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

// Name of the log file
#define LOG_FILE  "log.txt"

#define APP_NAME     "GLoid II is SDL/OpenGL Arkanoid " // The App Name And Caption
#define APP_VERSION  "2.5"

#define TEXTLINES  16  // Maximum number of lines of text

#define TRUE  1
#define FALSE 0
#define BOOL unsigned int

#ifndef WIN32	
 #define USHORT unsigned short
 #define MAX_PATH 256
 #define WORKPATH "/usr/local/share/gloid"
#else 
 #define WORKPATH "."
#endif

#define MINSPEED  1.0f//minimum speed on any axis = 5% of initial speed on Z
// Maximum string length
#define MAXLINE 256

// Number of sound channels (Stereo)
#define NUM_BUFFERS 2

#define VAUS_PARTICLES  128  //asplozans!
#define ALIEN_PARTICLES 1024

#define VAUS_COLORS  4
#define ALIEN_COLORS 3

#define GRAVITY	10.0f

#define SCENE_MAX   17.5f // Up, right, in                
#define	SCENE_MIN  -17.5f // Down, left, out
#define SCENE_AIR   11.7f // Camera distance

// Libraries

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

// The default bits per pixel
#ifdef WIN32
#define SCREEN_BPP 32
#else 
#ifdef __MACOSX__
#define SCREEN_BPP 32
#else
#define SCREEN_BPP 24
#endif
#endif

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
****************************************************************************/

// ENUMERATIONS

// Bonus pills
enum{
   P,       // Grey:    Player life (kanonaki) 
   B,       // Pink:    Boost to next level
   E,       // Blue:    Enlarge
   L,       // Red:     Lazer cannon
   G,       // Green:   Glue
   D,       // Magenta: Divide balls
   S,       // Orange:  Slow
   N_PILLS  // Number of pills
};

// Colors
enum{
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
};

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

// Game states
enum{
   LOADING,
   WAITING,
   INTRO,
   GO,
   PLAY,
   WIN
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

// Datatype definitions

typedef struct point3f{
   float x;
   float y;
   float z;
   point3f(float a, float b, float c){
       x = a;
       y = b;
       z = c;
   }
   point3f(){
       x = ZERO;
       y = ZERO;
       z = ZERO;
   }
   point3f(const point3f* other){
       x = other->x;
       y = other->y;
       z = other->z;
   }

   bool eq(const point3f other){
    return x - other.x <= FLOAT_PRECISION
        && y - other.y <= FLOAT_PRECISION
        && z - other.z <= FLOAT_PRECISION;
   }
   point3f * deepcopy(const point3f* other){
       x = other->x;
       y = other->y;
       z = other->z;

       return this;
   }
} * point3f_t;

typedef struct point3i{
   int X;
   int Y;
   int Z;
   point3i(int a, int b, int c){
       X = a;
       Y = b;
       Z = c;
   }
   point3i(){
       X = 0;
       Y = 0;
       Z = 0;
   }
   point3i(const point3i* other){
       X = other->X;
       Y = other->Y;
       Z = other->Z;
   }
} * point3i_t;

// Mouse struct
typedef struct mousecntl{
   int x;
   int y;
   BOOL leftclick;
} * mousecntl_t;

// Application runtime data
typedef struct appstatus{
   BOOL visible;         // Is the application visible or iconified?
   BOOL mouse_focus;     // Is the mouse cursor inside the window?
   BOOL keyboard_focus;  // Does our application have keyboard focus?
} * appstatus_t;

typedef struct text2d{
   SDL_Surface *T;
   SDL_Rect src;//obsolete 
   char msg[MAXLINE];
   Uint32 timestamp;  // If timestamp is zero, the message is always on.
   Uint32 lifetime;   // If (ticks - timestamp) > lifetime, the popup dies.
} * text2d_t;

typedef struct hallentry
{
   char* initials;
   int score;
} * hallentry_t;

#else
class balls: public WhatUC
{
public:
   float rad;
   BOOL	launched;
   int nextaxis;
   point3f speed, nextbounce, nextspeed, initspeed, launchspeed;

   void	display(void);
   int animate(unsigned int*);
   void launch();
   void reinit(point3f);
   int bounce(point3f);

   balls()
   {
      launched = FALSE;
      rad = 0.625f;

      setsize(2*rad, 2*rad, 2*rad);
      memset(&speed, 0, sizeof(point3f));
      memset(&nextbounce, 0, sizeof(point3f));

      initspeed.x = 10.0f;
      initspeed.y = 10.0f;
      initspeed.z = 20.0f;
      launchspeed.x = initspeed.x; 
      launchspeed.y = initspeed.y; 
      launchspeed.z = initspeed.z;
      active = FALSE;
   }

   ~balls()
   {
   }
};

class vauses: public WhatUC
{
public:
   //BOOL explosion;      // explosion occurs ?
   //int  explosion_life; // explosion duration
   BOOL armed;  // type; on lazer mode left click fires
   BOOL large;
   BOOL warping;
   BOOL fire;
	
   float speedx, speedy;
   float base_rad, prevx, prevy, particle_rad;
   point3f pal[VAUS_COLORS];
   particles explosion[VAUS_PARTICLES]; 

   void display(void);
   void animate(int, int);
   void enlarge(void);
   void reset(void);

   vauses()
   {
      int i;

      // Grey
      pal[0].x = 0.3f;
      pal[0].y = 0.3f;
      pal[0].z = 0.3f;

      // Red
      pal[1].x = 1.0f;
      pal[1].y = 0.0f;
      pal[1].z = 0.0f;

      // Blue
      pal[2].x = 0.0f;
      pal[2].y = 0.0f;
      pal[2].z = 1.0f;

      // Light blue
      pal[3].x = 0.5f;
      pal[3].y = 0.5f;
      pal[3].z = 1.0f;

      armed = FALSE;
      warping = FALSE;
      base_rad = 2.5f;
      particle_rad = 0.5f;

      setsize(2*base_rad, 2*base_rad, base_rad/2);

      speedx = speedy = 0;

      for(i = 0; i < VAUS_PARTICLES; i++)
         explosion[i] = particles();
   }

   ~vauses()
   {
   }
};

// Bonus pills
class pills: public WhatUC
{	
public:
   int   type;  // Enumerated pill type
   float speedz;
   float rotspeed;
   float rotx;
   float len, rad;  // Length and radius of pill
   point3f col;     // Pill color
   char label;
   SDL_Surface* surf;
   text2d text;

   void init();
   void display();
   int  animate(void);

   pills()
   {
      type = -1;
      active = FALSE;
      speedz = -10.0f;
      rotspeed = 360.0f;
      rotx = 0.0f;
      rad = 1.0f;
      len = 3.5f; 

      setsize(5.0f, 2*rad, 2*rad);
   }

   // Deallocate SDL_Surface data
   ~pills()
   {
      SDL_FreeSurface(surf);
      SDL_FreeSurface(text.T);
   }
};

class bricks: public WhatUC
{
public:
   Uint32  hit_effect;   // What happens when the brick is hit
   int     hit_counter;  // How many times has the brick been hit?
   int     type;         // Type of brick: normal, silver, gold: 0, 1, 2
   float   r, g, b;      // Colors imported from 3DSMax
   point3f rgb;

   int  init(float, float, float, int, int, int, int);
   int  hit();
   void display(Uint32);

   bricks()
   {
      active = FALSE;
   }

   ~bricks()
   {
   }
};

// ALIENS
class aliens: public WhatUC
{	
public:
   int     i,type;// explosion_life, type;
   point3f speed;
   float   roty,rotspeed,rad,particle_rad;
   point3f pal[ALIEN_COLORS];
   particles explosion[ALIEN_PARTICLES]; 

   void display(void);
   void animate(void);
   void die(void);

   aliens()
   {
      // White
      pal[0].x = 1.0f;
      pal[0].y = 1.0f;
      pal[0].z = 1.0f;

      // Yellow
      pal[1].x = 1.0f;
      pal[1].y = 1.0f;
      pal[1].z = 0.0f;

      // Red
      pal[2].x = 1.0f;
      pal[2].y = 0.0f;
      pal[2].z = 0.0f;

      //explosion_life = 0;
      particle_rad = 0.125f;
      place.x = 0.0f;
      place.y = 0.0f;

      setplace(0.0f, 0.0f, SCENE_MIN - SCENE_MAX + ALIENHOME);

      for(i = 0; i< ALIEN_PARTICLES; i++)
         explosion[i] = particles();

      memset(&speed, 0, sizeof(point3f));	

      roty = 0.0f;
      rotspeed = 360.0f;
      rad = 1.0f;
      setsize(4*rad, 4*rad, 4*rad);
   }

   ~aliens()
   {
   }	
};


// LAZER SHOTS
class shots: public WhatUC
{	
public:
   float speedz;

   void display(void);
   int  animate(unsigned int*);

   shots()
   {
      speedz = 40.0f;
      setsize(0.3f, 0.3f, 1.2f);
      active=FALSE;
   }

   ~shots()
   {
   }
};

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

// Sound functions
void mixer(void*, Uint8*, int);
BOOL PlaySDLSound(int);

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

// Collision detection
BOOL    collision(float pL, float pR, float pU, float pD, float sL, float sR, float sU, float sD);
point3i brickcoords(point3f);

// Calculate the norm of a 3-vector
float res3f(float, float, float);

// Chasing functions
int find_first(WhatUC *, int);
point3f chase(point3f, point3f, float);

// Generate a random vector
point3f rand3f(float);

// Given an int, calculate the next-largest power of two
int nextpoweroftwo(int);

// Calculate a moving average
double moving_average(double, double*, int);

// Probability function for bonus pills
int roulette(void);

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

// OpenGL geometry
void mySolidCube(float);
void mySolidTetrahedron(float);
void mySolidRhombik(float);
#endif  // _GLOID_H
