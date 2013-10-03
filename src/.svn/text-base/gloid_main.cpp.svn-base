/*
_______________________________________________________________________________
 
  LINKED LIBRARIES
  ================

  LIBRARY	PURPOSE			FUNCTIONS USED

  opengl32.lib	OpenGL API		glEnable(), glBindTexture()...
  glu32.lib	OpenGL Utilities	gluBuild2DMipmaps(), gluLookAt()...
  SDL
  SDLmain
  SDLmixer
  SDL_ttf
_______________________________________________________________________________
*/

#include "gloid.h"

#ifndef CDS_FULLSCREEN		// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4	// Compilers. By Defining It This Way,
#endif				// We Can Avoid Errors

// Global variables

// Sound buffer: SDL only works with double-buffering, so NUM_BUFFERS = 2
struct sample
{
   Uint8 *data;
   Uint32 dpos;
   Uint32 dlen;
}
Soundbuffer[NUM_BUFFERS];

GLuint TextureID[N_BMP+1];  // Array of texture IDs

// Global display engine parameters
point3f Pcamera;      // Camera coordinates
Uint32 Tic;           // Game timer
SDL_Surface *Screen;  // Reference to SDL Screen

char Version[6]         = "2.2";
BOOL Option_mipmap      = FALSE;
BOOL Option_vsync       = TRUE;
BOOL Option_billboard   = FALSE;
BOOL Option_smooth      = TRUE;
BOOL Option_multisample = TRUE;

float Theta = 0;
float Phi = 0;

unsigned int Screen_W = 1024;
unsigned int Screen_H = 768;

// The default bits per pixel
#ifdef WIN32 
unsigned int Screen_BPP = 32;
#else 
#ifdef __MACOSX__
unsigned int Screen_BPP = 32;
#else
unsigned int Screen_BPP = 24;
#endif
#endif
unsigned int Font_Size 	= 48;

S_AppStatus AppStatus;

// The error log file
FILE *ErrorLog;

SDL_AudioSpec Audio;    

// The index of the sound sample currently playing
int Sampleidx;

TTF_Font *DejaVuSans;

// Game color palette
SDL_Color Palette[N_RGB_COLORS] =
   {{198, 198, 198, 128},  // Grey
    {255, 128, 255, 128},  // Pink
    {255,   0,   0, 128},  // Blue
    {  0,   0, 255, 128},  // Red
    {  0, 255,   0, 128},  // Green
    {255, 255,   0, 128},  // Magenta
    {  0, 224, 255, 128},  // Orange
    {  0, 255, 255, 128},  // Yellow
    {255, 255, 255, 128},  // White
    {  0,   0,   0, 128}}; // Black

const char* PillLabels = "PBELGDS";

text2d Displayed_text[TEXTLINES];

// Array of game textures
SDL_Surface Texture[N_BMP+1];

// Array of sound effects
SDL_AudioCVT  Wave[N_WAV];

// SDL display information
const SDL_VideoInfo* Desktop;

double msPerFrame[10];

// Minimum number of milliseconds per frame
double MinmsPF = 40;

char Hallofame[MAXLINE];
char Intro[MAXLINE];

char Initials[MAXLINE] = "AK";

unsigned int Hiscore = 50000;
unsigned int Lives = 5;
unsigned int Bonus_mode = 0;
unsigned int Sound = 0;
unsigned int Level = 0;
unsigned int Level_type = 0;
unsigned int Brick_count = 0;
unsigned int Totalscore = 0;

extern unsigned int Level_type;
extern balls Ball[BALLNUM];
extern vauses Vaus[2];
extern pills Pill[INTX+1][INTY+1][INTZ+1];
extern bricks Brick[INTX+1][INTY+1][INTZ+1];
extern aliens Alien[ALIENNUM];
extern shots Shot[SHOTNUM];

// Initialize OpenGL
BOOL InitGL(SDL_Surface *S)
{
   static float ambient[] = {0.319, 0.319, 0.319, 1.0};
   static float diffuse[] = {0.725, 0.725, 0.725, 0.0};

   //static float lmodel_ambient[] = {0.1, 0.1, 0.1, 0.0};
   //static float lmodel_localviewer[] = {1.0};

   static float mat_shininess[] = {10.0};
   static float mat_specular[] = {0.8, 0.8, 0.8, 1.0};
   static float mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
   static float mat_diffuse[] = {0.4, 0.4, 0.4, 1.0};
   static float mat_emission[] = {0.2f, 0.2f, 0.2f, 0.0f};
   //static float spot_direction[] = {-1.0, -1.0, 0.0};

   // Black background
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Set up depth buffer
   glClearDepth(1.0f);

   // Set the type of depth testing
   glDepthFunc(GL_LEQUAL);

   if(Option_smooth)
      glShadeModel(GL_SMOOTH);
   else
      glShadeModel(GL_FLAT);

   // Set perspective calculations to most accurate
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   // Set point smoothing to nicest
   glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

   // Set the blending function
   glBlendFunc(GL_SRC_ALPHA,GL_ONE);

   // Enable multisampling
   #ifndef WIN32 
   if(Option_multisample)
      glEnable(GL_MULTISAMPLE_ARB);
   #endif
   // Enable alpha blending
   glEnable(GL_BLEND);

   // Enable texture mapping
   glEnable(GL_TEXTURE_2D);

   // Set up the lighting parameters
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

   // Set up the OpenGL view
   glViewport(0, 0, (GLsizei)(Screen_W),(GLsizei)(Screen_H));

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90.0f, (GLfloat)(Screen_W)/(GLfloat)(Screen_H), 0.1f, 500.0f);


   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, -SCENE_MAX, 0.0, 1.0,  0.0);

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   return true;
}

// Main initialization
BOOL Initialize()
{
   int bpp, n, nbuf, i;
   Uint32 Vflags;

   int modes[][2] = {
      {2048,1152},
      {1920, 1200},
      {1600, 1200},
      {1280, 1024},
      {1280, 768},
      {1280, 720},
      {1024, 768},
      {1024, 600},
      {800, 600},
      {720, 576},
      {720, 480},
      {640, 480}
   };
 
   // Set the application to visible
   AppStatus.visible = TRUE;
   AppStatus.mouse_focus = TRUE;
   AppStatus.keyboard_focus = TRUE;

   // Initialize the error log file
   InitErrorLog();

   // Read the high scores
   get_hiscore();

   // Initialize the SDL library
   if(SDL_Init(SDL_INIT_VIDEO| SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0)
   {
      Log("Unable to open SDL: %s\n", SDL_GetError());
      exit(1);
   }

   
   Vflags = SDL_OPENGLBLIT | SDL_HWSURFACE | SDL_FULLSCREEN;
   Screen = NULL;
   n = 0;
#if ((SDL_MAJOR_VERSION > 1) || (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) || (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 && SDL_PATCHLEVEL > 9))
   Desktop = SDL_GetVideoInfo();
   Screen_W = Desktop->current_w;
   Screen_H = Desktop->current_h;
   for(bpp = 32; bpp > 8; bpp -= 8)
      {
         Screen = SDL_SetVideoMode(Screen_W , Screen_H , bpp, Vflags);

         if(Screen != NULL)
         {
            Screen_BPP = bpp;
            break;
         }
      }

#else 
	Log("libSDL v.%d.%d.%d doesn't support SDL_GetVideoInfo(), testing video modes... \n",SDL_MAJOR_VERSION, SDL_MINOR_VERSION,SDL_PATCHLEVEL);
 // Hunt for video modes
   	while((Screen == NULL) && (n < 12))
   	{
      	for(bpp = 32; bpp > 8; bpp -= 8)
      	{
         Screen = SDL_SetVideoMode(modes[n][0], modes[n][1], bpp, Vflags);

         if(Screen != NULL)
         {
            Screen_W = modes[n][0];
            Screen_H = modes[n][1];
            Screen_BPP = bpp;
            break;
         }
      }
      n++;
   }

#endif
SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, Screen_BPP);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
   SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

Font_Size = Screen_H/16;

  
   if(Screen == NULL)
   {
      Log("Can't open an SDL Screen: %s\n", SDL_GetError());
      exit(1);
   }

   // Check whether multisample has been set
   SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &nbuf);

   if(nbuf == 0)
      Option_multisample = FALSE;

   if(!InitGL(Screen))
   {
      Log("Can't init GL: %s\n", SDL_GetError());
      exit(1);
   }

   // Hide system cursor
   SDL_ShowCursor(0);

   // Initialize rand()
   srand(SDL_GetTicks());

   // Initialize sound
   initSound();

   // Initialize timer
   Tic = SDL_GetTicks();

   // Initialize timer filter
   for(i = 0; i < 10; i++)
      msPerFrame[i] = 10;

   // Initialise Truetype font, point size 22
   if(!initFont(22))
      return FALSE;

   cleanText();

   // Initialize camera
   Pcamera.x = 0.0f; 
   Pcamera.y = 0.0f; 
   Pcamera.z = SCENE_AIR;
	
   Ball[0].active = TRUE;

   return TRUE;
}

void cleanText()
{
   memset(Displayed_text, 0, sizeof(text2d)*TEXTLINES);	
}

// Load TTF font with point size <size> using SDL_ttf
int initFont(int size)
{
   char path[MAX_PATH];

   if(TTF_Init() == -1)
   {
      Log(TTF_GetError());
      Log(": Unable to initialize SDL_ttf\n");
      return FALSE;
   }

   memset(path, 0, MAX_PATH);
   snprintf(path, MAX_PATH, "%s/DejaVuSans.ttf", WORKPATH);

   DejaVuSans = TTF_OpenFont(path, size);

   if(DejaVuSans == NULL)
   {
      Log(TTF_GetError());
      Log(": Unable to Load Font\n");
      return FALSE;
   }

   TTF_SetFontStyle(DejaVuSans, TTF_STYLE_BOLD);

   return TRUE;		
}

// Initialize the sound system
void initSound()
{
   // 16-bit stereo audio at 22kHz
   Audio.freq = 22050;
   Audio.format = AUDIO_S16;
   Audio.channels = 2;
   Audio.samples = 512;
   Audio.callback = mixer;
   Audio.userdata = NULL;

   // Open the audio device
   if(SDL_OpenAudio(&Audio, NULL) < 0)
   {
      Log("Unable to open audio: %s\n", SDL_GetError());
      exit(1);
   }

   // Start playing whatever is in the buffer
   SDL_PauseAudio(0);
}

// Initialize error logging
BOOL InitErrorLog(void)
{
   char path[MAX_PATH];

   memset(path, 0, MAX_PATH);
   snprintf(path, MAX_PATH, "%s/%s", WORKPATH, LOG_FILE);

   // If we can't open LOG_FILE for error logging
   if(!(ErrorLog = fopen(path, "w")))
   {
      perror("Can't init Logfile!\n");
      exit(1);
   }
#ifdef DEBUG
   Log("%s V%s -- Log Init...\n\n", APP_NAME, APP_VERSION);
#endif
   return TRUE;
}

void CloseErrorLog(void)
{
#ifdef DEBUG
   Log("\n-- Closing Log...\n");
#endif
   if(ErrorLog)
      fclose(ErrorLog);

   return;
}

// Uninitialize everything before exit
void Deinitialize (void)
{
   SDL_CloseAudio();
   CloseErrorLog();
}

// SDL audio mixing
void mixer(void *udata, Uint8 *stream, int len)
{
   int i;
   int amount;

   for(i = 0; i<NUM_BUFFERS; ++i ) 
   {
      // How much is left to play?
      amount = Soundbuffer[i].dlen-Soundbuffer[i].dpos;

      if(amount > len) 
         amount = len;

      SDL_MixAudio(stream, &Soundbuffer[i].data[Soundbuffer[i].dpos],
                   amount, SDL_MIX_MAXVOLUME);

      Soundbuffer[i].dpos += amount;
   }
}

// Play a sound effect
BOOL PlaySDLSound(int wavidx)
{
   int index;
   //Uint8 *data;
   //Uint32 dlen;

   // Look for an available sound slot 
   for (index = 0; index < NUM_BUFFERS; ++index) 
   {
      if(Soundbuffer[index].dpos == Soundbuffer[index].dlen) 
         break;
   }

   // Nothing available
   if(index == NUM_BUFFERS)
        return FALSE;

   // Put the sound in the buffer
   Soundbuffer[index].data = Wave[wavidx].buf;
   Soundbuffer[index].dlen = Wave[wavidx].len_cvt;
   Soundbuffer[index].dpos = 0;

   return TRUE;
}

// Game update function
int Update(Uint32 timepassed, Uint8* keys, mousecntl* mouse)
{
   static int alienspawn, i, j, k, alienflag, shotflag;
   static Uint32 shot_tic, toc, shotspawn;
   static unsigned short initialised, paused;
   //extern double ms10;
   int score = 0;

   toc = SDL_GetTicks();

   // Init spawn counters in the first 100 milisec 
   if(initialised == FALSE)
   {
      alienspawn = 0;
      initialised = TRUE;
      paused = FALSE;
   }

   if(alienspawn > 65534)
      alienspawn = 1;

   if(shotspawn > 65534)
      shotspawn = 1;

   if(keys)
   {

      // Arrow keys change the camera angle
      if(keys[SDLK_LEFT])
      {
         Phi += 0.02f;
         Pcamera.x  = sin(Phi) * (SCENE_AIR + 2 * SCENE_MAX);
         Pcamera.z = cos(Phi) * (SCENE_AIR + 2 * SCENE_MAX) - 2 * SCENE_MAX;
      }

      if(keys[SDLK_RIGHT])
      {
         Phi -= 0.02f;
         Pcamera.x  = sin(Phi) * (SCENE_AIR + 2 * SCENE_MAX);
         Pcamera.z = cos(Phi) * (SCENE_AIR + 2 * SCENE_MAX) - 2 * SCENE_MAX;
      }

      if(keys[SDLK_UP])
      {
         Theta += 0.02f;
         Pcamera.y  = sin(Theta) * (SCENE_AIR + 2 * SCENE_MAX);
         Pcamera.z = cos(Theta) * (SCENE_AIR + 2 * SCENE_MAX) - 2 * SCENE_MAX;
      }

      if(keys[SDLK_DOWN])
      {
         Theta -= 0.02f;
         Pcamera.y  = sin(Theta) * (SCENE_AIR + 2 * SCENE_MAX);
         Pcamera.z = cos(Theta) * (SCENE_AIR + 2 * SCENE_MAX) - 2 * SCENE_MAX;
      }

      // Force a reshape
      if(keys[SDLK_r])
      {
	 keys[SDLK_r] = FALSE;
         ReshapeGL(Screen_W, Screen_H);
      }
      // Toggle between full screen and windowed display modes
      if(keys[SDLK_f])
      {
	 keys[SDLK_f] = FALSE;
         ToggleFullscreen();
      }
      // Pause the game
      if(keys[SDLK_p])
      {
         keys[SDLK_p] = FALSE;                    
         paused = TogglePause(paused);
      }

      // Decrease the frame rate
      if(keys[SDLK_KP_PLUS])
      {
	 keys[SDLK_KP_PLUS] = FALSE;
         if(MinmsPF>10)
            MinmsPF--;

         Displayed_text[HUD_FPS].timestamp = toc;
         updateHUD();
      }

      // Increase the frame rate
      if(keys[SDLK_KP_MINUS])
      {
	 keys[SDLK_KP_MINUS] = FALSE;
         if(MinmsPF<100)
            MinmsPF++;

         Displayed_text[HUD_FPS].timestamp = toc;	
         updateHUD();
      }
   }

   if(paused == FALSE)
   {
      // Update pills
      for(i = INTX; i >= 0; i--)
         for(j = INTY; j >= 0; j--)
            for(k = INTZ; k >=0; k--)
               if(Pill[i][j][k].active)
                  score += Pill[i][j][k].animate();

      // Update shots
      for(i = 0; i < SHOTNUM; i++)
         if(Shot[i].active)
            score += Shot[i].animate(&Brick_count);

      // Update vaus
      Vaus[0].animate(mouse->x, mouse->y);

      if(mouse->leftclick)
      {
         Vaus[0].fire = TRUE;

         for(i = 0; i < BALLNUM; i++)
            if(Ball[i].active &&!Ball[i].launched)
            {
               Ball[i].launch();
               break;
            }
      }
      else
         Vaus[0].fire = FALSE;

      // Update particles
      for(i = 0; i < VAUS_PARTICLES; i++)
         if(Vaus[0].explosion[i].active)
            Vaus[0].explosion[i].animate();

      // Spawn an alien every ten seconds
      if(((toc % 10000) < 1000) && (alienflag == TRUE))
      {
         alienflag = FALSE;
         j = alienspawn % ALIENNUM;
         alienspawn++;

         if(!Alien[j].active)
         {
            Alien[j].active = TRUE;
            Alien[j].place.z = (SCENE_MIN - SCENE_MAX + ALIENHOME);
         }
      }

      if((toc % 10000) > 1000)
         alienflag = TRUE;

      // Update aliens
      for(i = 0; i < ALIENNUM; i++)
      {
         if(Alien[i].active)
            Alien[i].animate();

         for(j = 0; j <ALIEN_PARTICLES; j++)
            if(Alien[i].explosion[j].active)
               Alien[i].explosion[j].animate();
      }

      // Update balls
      for(i = 0; i < BALLNUM; i++)
         if(Ball[i].active)
            score += Ball[i].animate(&Brick_count);

      // Laser shots. TODO: should this be in the block below?
      if(!mouse->leftclick)
      {
         shotspawn = 0;
         shot_tic = SDL_GetTicks();
      }

      // Fire lasers every 500 milliseconds
      if((Bonus_mode == L) && (mouse->leftclick) &&
         (toc - shot_tic > shotspawn * 500)) {
         shotflag = FALSE;
         shoot();
         shotspawn++;
      }
   }
   return score;
}

// Game logic
int GameLogic(void)
{
   //int i, j, k;
   static int life_lost;
   static int won;  // 1 = passed level, 0 = playing, -1 = lost
   static unsigned int hundreds;  // Current score you need to get an extra life
   int step = PLAY;   // Game logic only runs while the game is playing
   static int level;

   // Determine if we get an extra life
   if(Totalscore <= SCORELIFE)
      hundreds = 1;
   else
   {
      if(Totalscore > SCORELIFE * hundreds)
      {
         Lives++;
         hundreds++;
      }
   }

   // If spaceship is not active, we lose a life
    if(!(Vaus[0].active)) 
      life_lost = TRUE;

   if(life_lost)
   {
      ReshapeGL(Screen_W, Screen_H);
      Totalscore += SCOREBONUS;
      Lives--;

      life_lost = FALSE;

      // Play "lose" sound effect
      PlaySDLSound(WAV_LOSE);
      resetlevel(FALSE);//reset, leave bricks alone
   }

   // Determine if we won or lost the level
   if(Lives == 0)
      won = -1;
   else
   {
      if(Brick_count == 0)
         won = 1;
      else
      {
         if(Vaus[0].warping)
         {
            Vaus[0].warping = FALSE;
            won = 1;
         }
         else
            won = 0;
      }
   }

   switch(won)
   {
      // We lost
      case -1:
         ReshapeGL(Screen_W, Screen_H);
         resetlevel(TRUE);	
         // reset and clean up bricks
         step = WAITING;
         Brick_count = generateLevel(level);
         Lives = 5;
         won = 0;

         // If we have a high score, we can enter our initials
         if(Totalscore >= Hiscore)
            step = WIN;
         else
         {
            step = WAITING;
            Totalscore = 0;
         }
         break;

      // We won
      case 1:
         ReshapeGL(Screen_W, Screen_H);

         // Advance a level
         level++;

         level = (level > MAXLEVEL)?(0):(level);
         Level_type = level%LEVELTYPES;
         Totalscore += SCOREWIN;

         resetlevel(TRUE);
         // reset and clean up bricks
         won = 0;

         // Load the next level and start playing
         Brick_count = generateLevel(level);

         step = GO;
         Level = level;
         break;

      default: 
         break;
   }
   return step;
}

// Draw the game scene
void Draw (SDL_Surface* surf, Uint32 now)
{
   int i, j, k;
   static float position_player[] = {-20.0, 20.0, -10.0};

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable (GL_TEXTURE_2D);

   // Reset the OpenGL viewport
   glViewport(0, 0, (GLsizei)(Screen_W),(GLsizei)(Screen_H));

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set the perspective
   gluPerspective(90.0f, 1, 0.1f, 100.0f);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Set the camera position
   gluLookAt(Pcamera.x, Pcamera.y, Pcamera.z, 0.0, 0.0, -SCENE_MAX,
             0.0, 1.0, 0.0);

   glRotatef(0, 1.0, 0.0, 0.0);
   glRotatef(0, 0.0, 1.0, 0.0);

   glTranslatef(-Pcamera.x, -Pcamera.y, 0);

   // Set the position of the light source
   glLightfv(GL_LIGHT0, GL_POSITION, position_player);

   display_background();

   if(Bonus_mode == B)
      display_warp();

   glTranslatef(0.0f, 0.0f, -SCENE_AIR);
		
   glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
   glEnable(GL_CULL_FACE);
   glFrontFace(GL_CCW);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);	
   glEnable(GL_COLOR_MATERIAL);

   // Display balls
   for(i = 0; i < BALLNUM; i++)
   {
      if((Ball[i].active) && !(Vaus[0].explosion[0].active))
         Ball[i].display();
   }

   // Display bricks
   for(i = INTX; i >= 0; i--)
   {
      for(j = INTY; j >= 0; j--)
      {
         for(k = INTZ; k >= 0; k--)
         {
            if(Brick[i][j][k].active)
            {
               glPushMatrix();
               glTranslatef(Brick[i][j][k].place.x,
                            Brick[i][j][k].place.y,
                            Brick[i][j][k].place.z);
               Brick[i][j][k].display(now);
               glPopMatrix();
            }
         }
      }
   }

   // Display pills
   for(i = INTX; i >= 0; i--)
   {
      for(j = INTY; j >= 0; j--)
      {
         for(k = INTZ; k >=0; k--)
         {
            if(Pill[i][j][k].active)
               Pill[i][j][k].display();
         }
      }
   }

   // Display aliens
   for(i = 0; i < ALIENNUM; i++)
   {
      if(Alien[i].active)
         Alien[i].display();
   }
		
   // Display laser shots
   for(i = 0; i < SHOTNUM; i++)
   {
      if(Shot[i].active)
         Shot[i].display();
   }

   // Display Vaus
   if(!(Vaus[0].explosion[0].active))
   {
      Vaus[0].display();
   }
   glDisable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);

   // Display targets
   for(i = 0; (i < BALLNUM) && (Ball[i].active); i++)
   {
      if(Ball[i].speed.z < 0)     
         display_cross(Ball[i].nextaxis, Ball[i].nextbounce, BMP_TARGET);
      else
      {
         if(!Ball[i].launched)
            display_cross(Ball[i].nextaxis, Ball[i].nextbounce, BMP_CROSSHAIRS);
      }
   }

   // Draw Vaus particles
   for(i = 0; i < VAUS_PARTICLES; i++)
   {
      if(Vaus[0].explosion[i].active)
         Vaus[0].explosion[i].display(Vaus[0].particle_rad);
   }

   // Draw alien particles
   for(i = 0; i < ALIENNUM; i++)
   {
      for(j = 0; j < ALIEN_PARTICLES; j++)
      {
         if(Alien[i].explosion[j].active)
            Alien[i].explosion[j].display(Alien[i].particle_rad);		
      }
   }

   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   glDisable(GL_CULL_FACE);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Set camera position
   gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, -SCENE_MAX, 0.0, 1.0, 0.0);

   // Display lives left
   display_lives(surf);

   // Display messages
   for(i = 0; i < N_HUD; i++)
   {
      if(Displayed_text[i].msg[0])
      {
         if((!Displayed_text[i].timestamp) || 
            (now - Displayed_text[i].timestamp < Displayed_text[i].lifetime))
            DrawOpenGLText(&Displayed_text[i]);
      }
   }

   // If paused, display PAUSED
   if(Displayed_text[HUD_PAWS].msg[0])
      DrawOpenGLText(&Displayed_text[HUD_PAWS]);                           

   glDisable(GL_LIGHTING);
   glFlush();
}

void updateHUD()
{
   extern double ms10;

   printText(TRUE, &Displayed_text[HUD_1UP], Palette[RGB_RED],
             Palette[RGB_BLACK], 0, 0, "1UP");

   printText(TRUE, &Displayed_text[HUD_HILABEL], Palette[RGB_RED],
             Palette[RGB_BLACK], Screen_W - 5 * Font_Size, 0,
             "HIGH SCORE");

   printText(TRUE, &Displayed_text[HUD_SCORE], Palette[RGB_WHITE],
             Palette[RGB_BLACK], 0, -Font_Size, "%d", Totalscore);

   printText(TRUE, &Displayed_text[HUD_HISCORE], Palette[RGB_WHITE],
             Palette[RGB_BLACK], Screen_W - 5 * Font_Size, -Font_Size,
             "%d", Hiscore);

   printText(TRUE, &Displayed_text[HUD_LEVEL], Palette[RGB_WHITE],
             Palette[RGB_BLACK], Screen_W / 2, -Screen_H / 2,
             "Level %d", Level + 1);

   printText(TRUE, &Displayed_text[HUD_GO], Palette[RGB_WHITE],
             Palette[RGB_BLACK], Screen_W / 2, -Font_Size - Screen_H / 2,
             "GO!");

   printText(TRUE, &Displayed_text[HUD_FPS], Palette[RGB_WHITE],
             Palette[RGB_BLACK], 0, -Screen_H, "FPS: %g Average: %g",
             1000.0 / MinmsPF, 500.0 / ms10);

   Displayed_text[HUD_GO].lifetime = 1000;
   Displayed_text[HUD_LEVEL].lifetime = 1000;
   Displayed_text[HUD_FPS].lifetime = 500;
}

// Draw SDL_ttf rendered text to an SDL_Surface
void DrawSDLText(SDL_Surface* S, text2d* text)
{
   SDL_Rect dummy;

   memset(&dummy, 0, sizeof(SDL_Rect));

   SDL_FillRect(S, &dummy, SDL_MapRGBA(S->format, 0, 0, 0, 0));
   SDL_BlitSurface(text->T, NULL, S, &(text->src));
   SDL_UpdateRects(S, 1, &(text->src));	
} 

// Draw SDL_ttf rendered text to an OpenGL texture
void DrawOpenGLText(text2d* text)
{
   SDL_Surface *s, *p;
   int h, w;
   float xx, yy;

   p = text->T;
   w = nextpoweroftwo(p->w);
   h = nextpoweroftwo(p->h);

   xx = SCENE_MIN + 2 * SCENE_MAX * text->src.x / Screen_W;
   yy = -SCENE_MIN + 2 * SCENE_MAX * text->src.y / Screen_H;

   s = SDL_CreateRGBSurface(p->flags, w, h, Screen_BPP,
                            0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

   SDL_SetColorKey(s, SDL_SRCCOLORKEY|SDL_RLEACCEL,
                   SDL_MapRGBA(s->format, 0, 0, 0, 0));

   SDL_BlitSurface(p, 0, s, 0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TextureID[N_BMP]);//use warp ID + 1
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, s->pixels);

   glColor3f(1.0f, 1.0f, 1.0f);        

   glBegin(GL_QUADS);
   glTexCoord2f(1.0f, 1.0f);
   glVertex3f(xx + Font_Size * w / Screen_W,
              yy - Font_Size * h / Screen_H, -SCENE_AIR);
   glTexCoord2f(1.0f, 0.0f);
   glVertex3f(xx + Font_Size * w / Screen_W, yy, -SCENE_AIR);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(xx, yy, -SCENE_AIR);
   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(xx, yy - Font_Size * h / Screen_H, -SCENE_AIR);
   glEnd();

   glDisable(GL_TEXTURE_2D);
   SDL_FreeSurface(s);
}   

// Reset the state of game objects
void resetlevel(BOOL cleanbricks)
{
   int i, j, k;

   Bonus_mode = P;

   Vaus[0].reset();
   Ball[0].reinit(Ball[0].initspeed);

   Tic = SDL_GetTicks();

   for(i = 0; i < ALIENNUM; i++)
   {
      if(Alien[i].active)
         Alien[i].active = FALSE;
   }

   for(i = 1; i < BALLNUM; i++)
   {
      if(Ball[i].active)
         Ball[i].active = FALSE;
   }

   for(i = 0; i < SHOTNUM; i++)
   {
      if(Shot[i].active)
         Shot[i].active = FALSE;
   }

   for(i = INTX; i >= 0; i--)
   {
      for(j = INTY; j >= 0; j--)
      {
         for(k = INTZ; k >=0; k--)
         {
            if(Pill[i][j][k].active)
               Pill[i][j][k].active = FALSE;
         }
      }
   }
   if(cleanbricks)
   {// Clean up bricks
         for(i = INTX; i >= 0; i--)
            for(j = INTY; j >= 0; j--)
               for(k = INTZ; k >= 0; k--)
                  Brick[i][j][k].active=FALSE;
   }
}

// Put more balls into play
void divideballs(void)
{
   int i;

   for(i = 0; i < BALLNUM; i++)
   {
      if(!Ball[i].active)
      {
         Ball[i].active = TRUE;
         Ball[i].setplace(Ball[0].place.x, Ball[0].place.y, Ball[0].place.z);

         switch(i)
         {
            case 1:
               Ball[i].launchspeed.x = - Ball[0].speed.x;
               break;

            case 2:
               Ball[i].launchspeed.y = - Ball[0].speed.y;
               break;

            case 3:
               Ball[i].launchspeed.z = - Ball[0].speed.z;
               break;

            default:
               break;
         }
         Ball[i].launch();
      }
   }
}

// Fire laser cannons
void shoot()
{
   int i, j, k;

   for(j = 0, k = 0; j < 4; j++)
   {
      for(i = 0; i < SHOTNUM; i++)
      {
         if(!Shot[i].active)
         {
            k = i;
            Shot[k].active = TRUE;
            break;
         }
      }

      // There are no more shots left
      if(i == SHOTNUM)
         return;

      switch(j)
      {
         case 0:
            Shot[k].setplace(Vaus[0].place.x + Vaus[0].base_rad,
                             Vaus[0].place.y + Vaus[0].base_rad,
                             Vaus[0].place.z);
            break;

         case 1:
            Shot[k].setplace(Vaus[0].place.x + Vaus[0].base_rad,
                             Vaus[0].place.y - Vaus[0].base_rad,
                             Vaus[0].place.z);
            break;

         case 2:
            Shot[k].setplace(Vaus[0].place.x - Vaus[0].base_rad,
                             Vaus[0].place.y - Vaus[0].base_rad,
                             Vaus[0].place.z);
            break;

         case 3:
            Shot[k].setplace(Vaus[0].place.x - Vaus[0].base_rad,
                             Vaus[0].place.y + Vaus[0].base_rad,
                             Vaus[0].place.z);
            break;

         default:
            break;
      }
   }
   // Play laser cannon sound
   PlaySDLSound(WAV_SHOT);
}
