#include <algorithm>

#include "GLoid.h"
#include "Hud.h"
#include "Waiting.h"
#define WAITING_HALLOFAME 0
#define WAITING_PROMPT 1
Waiting::Waiting(Game & g){
    game = &g;
    type = STEP_WAITING;
    Hud::setGame(g);
}

Waiting::~Waiting(){

}

int Waiting::next(){
    if(game->keyPressed(SDLK_SPACE)
    || game->getMouse()->leftclick){

        return STEP_INTRO;
    }
    return STEP_WAITING;
}

int Waiting::flip(unsigned int millis){
    if((millis % (2 * FLIP_PHASE)) >= FLIP_PHASE){
        return WAIT_HOF;
    }
    return WAIT_RDY;
}

Waiting & Waiting::update(){
    int newphase = flip(game->toc() - game->tic);
    if(newphase != phase){
        phase = newphase;
        clearText();
        if(phase == WAIT_RDY){
            logo = game->getTexture("gloid");
            printText("Press fire to play...", prompt, White, 0);
        }else{
            std::for_each(begin(game->hiscore),
                          end(game->hiscore),
                          [this](auto line){
                std::stringstream str;
                str << line.first << "    " << line.second;
                printText(str.str(), hallOfame);
            });
        }
    }
    return *this;
}

// Draw GLoid logo
void Waiting::drawLogo(){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, logo.first);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGB,
                logo.second.w,
                logo.second.h,
                0,
                GL_BGR,
                GL_UNSIGNED_BYTE,
                logo.second.pixels);

   glBegin(GL_QUADS);
   glTexCoord2f(ONE, ZERO);
   glVertex3f(SCENE_MAX/3, SCENE_MAX/4, -SCENE_AIR);
   glTexCoord2f(ONE, ONE);
   glVertex3f(SCENE_MAX/3, SCENE_MIN/4, -SCENE_AIR);
   glTexCoord2f(ZERO, ONE);
   glVertex3f(SCENE_MIN/3, SCENE_MIN/4, -SCENE_AIR);
   glTexCoord2f(ZERO, ZERO);
   glVertex3f(SCENE_MIN/3, SCENE_MAX/4, -SCENE_AIR);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}


Waiting & Waiting::draw(){

    if(phase == WAIT_RDY){
        drawLogo();
        //draw all text
        drawText(prompt);
    }else{
        drawText(hallOfame);
    }
    SDL_GL_SwapBuffers();
    return *this;
}




