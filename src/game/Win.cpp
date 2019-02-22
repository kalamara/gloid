#include "GLoid.h"
#include "Hud.h"
#include "Win.h"

Win::Win(){
    type = STEP_WIN;
}

Win::~Win(){

}

Win & Win::draw(){
    return *this;
}

Win & Win::update(){
    return *this;
}

int Win::next(){
    return STEP_WIN;
}

