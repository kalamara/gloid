#include "GLoid.h"
#include "WhatUC.h"
#include "Ball.h"
#include "Functions.h"

using namespace Functions;

Ball::Ball(){
    speed = new point3f(initspeed);
}

Ball::~Ball(){
    delete speed;

}

void Ball::display(){

}

Ball& Ball::animate(double secPerFrame){
    return *this;
}
