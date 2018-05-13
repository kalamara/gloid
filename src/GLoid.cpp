// gloid_base.cpp : Defines the entry point for the application.

/***********************************************
*                                              *
*    Jeff Molofee's Revised OpenGL Basecode    *
*  Huge Thanks To Maxwell Sayles & Peter Puck  *
*            http:// nehe.gamedev.net          *
*                     2001                     *
*                                              *
***********************************************/

#include "GLoid.h"
#include "Engine.h"
#include "World.h"
#include "Game.h"

// Text buffer for on-screen messages
// char Textbuffer[TEXTLINES][MAXLINE];  --> to engine

// Terminate application
//void TerminateApplication(void) --> to engine
//{
//   static SDL_Event Q;

//   Q.type = SDL_QUIT;

//   // Push an SDL_QUIT event into the event queue
//   if(SDL_PushEvent(&Q) == -1)
//   {
//      Log("SDL_QUIT event can't be pushed: %s\n", SDL_GetError());
//      exit(1);
//   }
//}

//// Toggle between full-screen and windowed modes --> to engine
//void ToggleFullscreen(void)
//{
//   SDL_Surface *screen;
//   Uint32 flags;
//extern unsigned int Screen_BPP;
//   // Get a handle to the screen
//   screen = SDL_GetVideoSurface();

//   // Save the current screen flags
//   flags = screen->flags;

//   // Try to toggle the screen mode
//   screen = SDL_SetVideoMode(Screen_W, Screen_H, Screen_BPP,screen->flags ^ SDL_FULLSCREEN);

//   if(screen == NULL)
//   {
//      screen = SDL_SetVideoMode(0, 0, 0, flags);
//      Log("Unable to toggle fullscreen: %s\n", SDL_GetError());

//   }
//   //SDL_Delay(MinmsPF);
//         InitGL(screen);
//   return;
//}

// Toggle between a paused and unpaused game state
//BOOL TogglePause(BOOL flag) --> to engine (?)
//{
//     Uint32 now;
//   if(flag == FALSE)
//   {
//      flag = TRUE;
//      printText(TRUE, &Displayed_text[HUD_PAWS], Palette[RGB_WHITE],
//                Palette[RGB_BLACK], Screen_W / 2, Font_Size -Screen_H / 2, "PAUSED");
//      SDL_Delay(MinmsPF);
//   }
//   else
//   {
//      flag = FALSE;
//      cleanText();
//      updateHUD();
//      //make sure popups don't pop up
//       now = SDL_GetTicks();
//       Displayed_text[HUD_GO].timestamp = now - Displayed_text[HUD_GO].lifetime;
//       Displayed_text[HUD_LEVEL].timestamp = now- Displayed_text[HUD_LEVEL].lifetime;
//       Displayed_text[HUD_FPS].timestamp = now - Displayed_text[HUD_FPS].lifetime;
//   }
//   return flag;
//}

//// Reshape the OpenGL window when it's moved or resized  --> to engine
//void ReshapeGL(int width, int height)
//{
//   // Reset the current viewport
//   glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));

//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();

//   // Calculate the aspect ratio of the window
//   gluPerspective(45.0f, (GLfloat)(width)/ (GLfloat)(height), 0.1f, 500.0f);

//   Pcamera.x = 0;
//   Pcamera.y = 0;
//   Pcamera.z = SCENE_AIR;  // Center of rotation = SCENE_AIR - SCENE_MAX

//   Phi = 0.0;
//   Theta = 0.0;

//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();

//   // Set the perspective
//   gluLookAt(Pcamera.x, Pcamera.y, Pcamera.z, 0.0, 0.0, -SCENE_MAX,
//             0.0, 1.0, 0.0);
//}

// Load the game sounds, textures, and first level
//void GameLoading( )
//{
//   static BOOL loadflag;
          
//#ifdef DEBUG
//   Log("Loading game\n");
//#endif

//   if(!loadflag)
//   {
//      cleanText();
//      printText(TRUE, &Displayed_text[0], Palette[RGB_WHITE],
//                Palette[RGB_BLACK], 0, 0, "Loading sounds...");
//#ifdef DEBUG
//      Log("Loading sounds...\n");
//#endif
//      loadSounds();

//      DrawOpenGLText(&Displayed_text[0]);
//      SDL_GL_SwapBuffers();

//      // Load first level
//      Brick_count = generateLevel(Level);
//      printText(TRUE, &Displayed_text[1], Palette[RGB_WHITE],
//                Palette[RGB_BLACK], 0, -Font_Size, "Loading textures...");
//#ifdef DEBUG
//      Log("Loading textures...\n");
//#endif
//      LoadTextures();
//      loadflag = TRUE;
//   }
//   DrawOpenGLText(&Displayed_text[0]);
//   DrawOpenGLText(&Displayed_text[1]);
//   SDL_GL_SwapBuffers();
//}

//BOOL GameHalloFame()
//{
//   int k;
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
//   for(k = 0; (k < TEXTLINES) && Textbuffer[k][0]; k++)
//      printText(TRUE, &Displayed_text[k], Palette[RGB_WHITE],
//                Palette[RGB_BLACK], 0, -k * Font_Size , "%s", Textbuffer[k]);
//   for(k = 0; k < TEXTLINES && Textbuffer[k][0];k++)
//      DrawOpenGLText(&Displayed_text[k]);

//   SDL_GL_SwapBuffers();
//   return FALSE;
//}

//BOOL GameReady()
//{
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//   printText(TRUE, &Displayed_text[0], Palette[RGB_WHITE], Palette[RGB_BLACK],
//             Screen_W / 3, Font_Size - 3 * Screen_H / 4,
//             "Press fire to play...");

//   ready_display(Screen);

//   DrawOpenGLText(&Displayed_text[0]);
//   SDL_GL_SwapBuffers();

//   return TRUE;
//}

// Play the game intro
//void GameIntro()
//{
//   static int j, k;
//   int i;
//   static BOOL introflag;
//   static char line[MAXLINE];

//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//   if(!introflag)
//   {
//      j = 0;
//      k = 0;

//      if(!PlaySDLSound(WAV_INTRO))
//         Log("Could not play sound %d\n", WAV_INTRO);

//#ifdef DEBUG
//      Log("Playing intro\n");
//#endif
//      introflag = TRUE;
//      cleanText();
//      memset(line,0,MAXLINE);
//   }

//   if((k < TEXTLINES) && (Textbuffer[k][0] != 0))
//   {
//      SDL_Delay(MinmsPF);

//      if(Textbuffer[k][j] != 0)
//      {
//         line[j] = Textbuffer[k][j];
//         j++;
//         printText(TRUE, &Displayed_text[k], Palette[RGB_WHITE],
//                   Palette[RGB_BLACK], 0, -k * Font_Size, "%s", line);

//         DrawOpenGLText(&Displayed_text[k]);
//      }
//      else
//      {
//         memset(line, 0, MAXLINE);
//         j = 0;
//         k++;
//      }

//      for(i = 0; i < k; i++)
//         DrawOpenGLText(&Displayed_text[i]);

//      SDL_GL_SwapBuffers();
//   }

//   if(k >= TEXTLINES)
//      introflag = FALSE;
//}

// Start the game
//void GameGo(void)
//{
//   extern vauses Vaus[2];
//   Uint32 now;
//   int i;

//   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   cleanText();
//   updateHUD();

//   now = SDL_GetTicks();
//   Displayed_text[HUD_GO].timestamp = now;
//   Displayed_text[HUD_LEVEL].timestamp = now;
//   Displayed_text[HUD_FPS].timestamp = now;

//   for(i = 0; i < VAUS_PARTICLES; i++)
//      Vaus[0].explosion[i].active = FALSE;

//   if(!PlaySDLSound(WAV_GO))
//      Log("Could not play sound %d\n", WAV_GO);
 
//#ifdef DEBUG
//   Log("GO!\n");
//#endif
//}

// The game is over!
//BOOL GameOver(Uint32 timestamp)
//{
//   Uint32 now;
//   now = SDL_GetTicks();
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   cleanText();

//   printText(TRUE, &Displayed_text[0], Palette[RGB_WHITE],Palette[RGB_BLACK],
//             Screen_W / 3, Font_Size - 3 * Screen_H / 4, "GAME OVER");

//   DrawOpenGLText(&Displayed_text[0]);
//   SDL_GL_SwapBuffers();
 
//   if(now > timestamp + 1000)
//          return TRUE;
//   else return FALSE;
//}

//BOOL GameWin(Uint8* keys)
//{
//   static BOOL winflag;
//   static char initials[4];
//   static int cursor;
//   int i, j, n;

//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//   if(!winflag)
//   {
//      cursor = 0;
//      memset(initials, 0, 4);
//      initials[0] = '.';

//      cleanText();

//      printText(TRUE, &Displayed_text[0], Palette[RGB_WHITE],
//                Palette[RGB_BLACK], 0, 0, "CONGRATULATIONS");

//      printText(TRUE, &Displayed_text[1], Palette[RGB_WHITE],
//                Palette[RGB_BLACK], 0, -Font_Size,
//                "You have entered the High Score Hall Of Fame!");

//      printText(TRUE, &Displayed_text[2], Palette[RGB_WHITE],
//                Palette[RGB_BLACK], 0, -2 * Font_Size, "Enter your initials:");

//      printText(TRUE, &Displayed_text[3], Palette[RGB_WHITE],
//                Palette[RGB_BLACK], 0, -3 * Font_Size, " %s", initials);
//#ifdef DEBUG
//      Log("You set the high score!\n");
//#endif
//      winflag = TRUE;
//   }

//   if(keys)
//   {
//      n = strlen(initials) - 1;

//      // Save initials and return
//      if(keys[SDLK_RETURN])
//      {
//         initials[n] = 0;
//         set_hiscore(initials);
//         get_hiscore();
//         Totalscore = 0;
//         winflag = FALSE;
//         return TRUE;
//      }

//      // Delete last character
//      if(keys[SDLK_DELETE] || keys[SDLK_BACKSPACE])
//      {
//         if(n > 0)
//         {
//            initials[n-1] = '.';

//            if(n <= 3)
//               initials[n] = 0;

//            n--;
//         }
//         printText(TRUE, &Displayed_text[3], Palette[RGB_WHITE],
//                   Palette[RGB_BLACK], 0, -3 * Font_Size, " %s", initials);

//         // If either of these was pressed, reset them both
//         keys[SDLK_DELETE] = FALSE;
//         keys[SDLK_BACKSPACE] = FALSE;
//      }

//      // If a lowercase letter was entered, include it in the initials
//      for(i = SDLK_a; i <= SDLK_z; i++)
//      {
//         if(keys[i])
//         {
//            j = i  - SDLK_a;

//            if((n >= 0) && (n < 3))
//            {
//               initials[n] = j + 'A';

//               if(n <= 2)
//                  initials[n+1] = '.';
//            }
//            printText(TRUE, &Displayed_text[3], Palette[RGB_WHITE],
//                      Palette[RGB_BLACK], 0, -3 * Font_Size, " %s", initials);

//            keys[i] = FALSE;
//         }
//      }
//   }

//   for(i = 0; i < 4; i++)
//   {
//      if(Displayed_text[i].msg[0])
//         DrawOpenGLText(&Displayed_text[i]);
//   }
//   SDL_GL_SwapBuffers();

//   return FALSE;
//} --> to game

int main(int argc, char **argv)
{

   // Initialize the game
//   if(!Initialize())  --> to engine
//   {
//      Log("App init failed: %s\n", SDL_GetError());
//      exit(1);
//   }

//   // Set the window caption --> to engine
//   SDL_WM_SetCaption(APP_NAME, NULL);

//   // Initialize OpenGL parameters
//   InitGL(Screen);

//   // Initialize game timer
//   tic = SDL_GetTicks();

    bool looping = false;
    Game game = Game();
    while(looping == true)
    {
//      if(SDL_PollEvent(&E)) --> to engine
//      {
//         switch(E.type)
//         {
//            case SDL_QUIT:
//               looping = FALSE;
//#ifdef DEBUG
//               Log("Frames skipped: %d\n", frameskips);
//               Log("Quitting!\n");
//#endif
//               break;

//            case SDL_VIDEORESIZE:
//               ReshapeGL(E.resize.w, E.resize.h);
//               break;

//            case SDL_ACTIVEEVENT:
//               // Visibility status has changed
//               if(E.active.state & SDL_APPACTIVE)
//               {
//                  if(E.active.gain)
//                     AppStatus.visible = TRUE;
//                  else
//                     AppStatus.visible = FALSE;
//               }

//               // Mouse focus status has changed
//               if(E.active.state & SDL_APPMOUSEFOCUS)
//               {
//                  if(E.active.gain)
//                     AppStatus.mouse_focus = TRUE;
//                  else
//                     AppStatus.mouse_focus = FALSE;
//               }

//               // Input focus status has changed
//               if(E.active.state & SDL_APPINPUTFOCUS)
//               {
//                  if(E.active.gain)
//                     AppStatus.keyboard_focus = TRUE;
//                  else
//                     AppStatus.keyboard_focus = FALSE;
//               }
//               break;

//            case SDL_KEYDOWN:
//               // Take a snapshot of the keyboard
//               keys = SDL_GetKeyState(NULL);
//               break;

//            case SDL_MOUSEMOTION:
//               mouse.x = E.motion.x;
//               mouse.y = E.motion.y;
//               break;

//            case SDL_MOUSEBUTTONDOWN:
//               mouse.leftclick = TRUE;
//               break;

//            case SDL_MOUSEBUTTONUP:
//               mouse.leftclick = FALSE;
//               break;

//            default:
//               break;
//         }
//      }
     // else
//      { --> to engine
//         // If the app isn't visible, don't do anything
//         if(!AppStatus.visible)
//            SDL_WaitEvent(NULL);
//         else
//         {
//            toc = SDL_GetTicks();
//             if(keys)
//             {
//                     if(keys[SDLK_ESCAPE])
//                      TerminateApplication ();
//             }
//            switch(gamestep) --> to game
//            {
//               case LOADING:
//                  GameLoading();

//                  if(mouse.leftclick)
//                     mouse.leftclick = FALSE;
//                  else
//                  {
//                     if(!loadText("hallofame.txt",(char**)Textbuffer))
//                        Log("Loading file %s failed!\n", "hallofame.txt");

//                     gamestep = WAITING;
//                  }
//                  break;

//               case WAITING:
//                  if((toc - tic > 5000) || (game_paused == 2))
//                  {
//                     tic = toc;

//                     // Display hall of fame stats when game is paused
//                     if(game_paused)
//                        game_paused = GameHalloFame();
//                     else
//                        game_paused = GameReady();
//                  }

//                  if(mouse.leftclick)
//                     gamestep = INTRO;

//                  if(gamestep == INTRO)
//                  {
//                     if(!loadText("arkanoid.txt",(char**)Textbuffer))
//                        Log("Loading file %s failed!\n", "arkanoid.txt");

//                     while(mouse.leftclick)//stall until mouse button is up
//                        mouse.leftclick = FALSE;
//                  }
//                  break;

//               case INTRO:
//                  if(toc - tic > 40)
//                  {
//                     GameIntro();
//                     tic = toc;
//                  }

//                  if (mouse.leftclick)
//                  {
//                     gamestep = GO;
//                     while(mouse.leftclick)//stall until mouse button is up
//                           mouse.leftclick = FALSE;
//                  }
//                  break;

//               case GO:
//                  GameGo();
//                  gamestep = PLAY;
//                  gameover_flag = FALSE;
//                  break;

//               case PLAY:
//                  Draw(Screen,toc);
//                  Totalscore += Update(toc-tic, keys, &mouse);
//                  gamestep = GameLogic();

//                  if(Totalscore > prevscore)
//                     updateHUD();

//                  prevscore = Totalscore;

//                  if(gamestep != PLAY)
//                  {
//                     InitGL(Screen);
//                     if(!loadText("hallofame.txt", (char**)Textbuffer))
//                        Log("Loading file %s failed!\n", "hallofame.txt");
                     
//                     if(gamestep == WAITING && !gameover_flag)
//                        gameover_flag = GameOver(tic);
//                     break;
//                  }
//                  else
//                  {
//                     // ms10 is the ten-sample moving average of the
//                     // milliseconds per frame
//                     ms10 = moving_average(toc - tic, msPerFrame, 10);
//                     delay = MinmsPF - toc + tic;

//                     // Enforce maximum frame rate
//                     if(delay > 0)
//                        SDL_Delay(delay);
//                     else
//                        frameskips++;

//                     if(frameskips > 65534)
//                        frameskips = 0;

//                     tic = toc;
//                  }
//                  SDL_GL_SwapBuffers();
//                  break;

//               case WIN:
//                  if(GameWin(keys))
//                  {
//                     gamestep = WAITING;

//                     if(!loadText("hallofame.txt",(char**)Textbuffer))
//                        Log("Loading file %s failed!\n", "hallofame.txt");
//                  }

//               default:
//                 break;
//            }
//         }
//      }
   }

   // Free allocated memory
//   Deinitialize(); --> to dtors

   // exit() calls SDL_Quit()
   exit(0);

   return 0;
}
