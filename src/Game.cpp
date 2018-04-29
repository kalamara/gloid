#include "GLoid.h"
#include "Game.h"

// Render text to an SDL_Surface
void Game::printText(bool option,
                     text2d* text,
                     SDL_Color fg,
                     SDL_Color bg,
                     int x,
                     int y,
                     const char* buf, ...){
  // extern TTF_Font *DejaVuSans;
   va_list Arg;

   va_start(Arg, buf);
   vsprintf(text->msg,buf, Arg);
   va_end(Arg);
/*
   if(option)
      text->T = TTF_RenderText_Shaded(DejaVuSans, text->msg, fg, bg);
   else
      text->T = TTF_RenderText_Blended(DejaVuSans, text->msg, fg);
*/
   text->src.w = text->T->w;
   text->src.h = text->T->h;
   text->src.x = x;
   text->src.y = y;
}
