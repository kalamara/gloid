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
#include "game/Loading.h"


int main(int argc, char **argv)
{
    auto game = Game();
//maybe useless    Loading * step = new Loading(game);
    auto sdlv = version(SDL_MAJOR_VERSION,
                                  SDL_MINOR_VERSION,
                                  SDL_PATCHLEVEL);
    game.withSdlGlVideo(sdlv)
                .withSdlTtf("./DejaVuSans.ttf")
                .withSdlAudio(22050, 2, 0)
                .withOpenGl();
    int i = 0;
    while(game.looping()){
          game.loop();
          i++;
     }

 // exit() calls SDL_Quit()
   exit(0);

   return 0;
}
