#include "GLoid.h"

#include "model/WhatUC.h"
#include "model/Ball.h"
#include "model/Brick.h"

#include "Play.h"

Play::Play(Game &g){
    game = &g;
    type = STEP_PLAY;
    Hud::setGame(g);
    go.splash = true;
    up1.alignment = ALIGN_LEFT;
    score.alignment = ALIGN_RIGHT;
}

Play::~Play(){

}

Play & Play::draw(){

    drawText(go);
    drawText(up1);
    drawText(score);
    //draw down left hud

    SDL_GL_SwapBuffers();

    return *this;
}

Play & Play::update(){
    if(phase==PLAY_GO){
        clearText();
        phase ++;
        printText("GO",go, 0);
    }
    printText("1UP", up1, 0);
    std::stringstream scorestr;
    scorestr << SCORELIFE;
    printText(scorestr.str(), score, 0);

    return *this;
}

int Play::next(){
    return STEP_PLAY;
}

