#include "GLoid.h"
#include "WhatUC.h"
#include "Ball.h"
#include "Functions.h"

using namespace Functions;

Ball::Ball(){
    speed = new point3f(ZERO, ZERO, ZERO);
    nextbounce = new point3f(ZERO, ZERO, ZERO);
    launchspeed = new point3f(initspeed);
    active = FALSE;
    launched = FALSE;
    rad = base_rad;
    setSize(2*rad, 2*rad, 2*rad);


}

Ball::~Ball(){
    delete speed;
    delete launchspeed;
    delete nextbounce;
}

void Ball::display(){

}

Ball& Ball::animate(double secPerFrame){
    return *this;
}
