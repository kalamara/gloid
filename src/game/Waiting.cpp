#include <algorithm>

#include "GLoid.h"
#include "Waiting.h"

Waiting::Waiting(Game & g){
    game = &g;
    type = STEP_WAITING;
    textSpacingPercent = 150;
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
    }
    if(text.empty()){
        if(phase == WAIT_RDY){
            textOffset = 8;
            logo = game->getTexture("gloid");
            printText("Press fire to play...");
        }else{
            textOffset = 0;
            std::for_each(begin(game->hiscore),
                          end(game->hiscore),
                          [this](auto line){
                std::stringstream str;
                str << line.first << "    " << line.second;
                printText(str.str());
            });
        }
    }
    return *this;
}

// Draw GLoid logo
void Waiting::drawLogo()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, logo.first);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, logo.second.w, logo.second.h,
                0, GL_RGB, GL_UNSIGNED_BYTE, logo.second.pixels);

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
    }
    drawText();
    return *this;
}




