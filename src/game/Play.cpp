#include "GLoid.h"

#include "model/WhatUC.h"
#include "model/Ball.h"
#include "model/Brick.h"

#include "Play.h"

Play::Play(){
    type = STEP_PLAY;
}

Play::~Play(){

}

Play * Play::draw(){
    return this;
}

Play * Play::update(){
    return this;
}

Play * Play::next(){
    return this;
}

