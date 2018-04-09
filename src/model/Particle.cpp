#include <list>
#include "GLoid.h"
#include "WhatUC.h"
#include "Particle.h"
#include "Functions.h"

using namespace Functions;

Particle::Particle(const point3f_t where,
                   const point3f_t color,
                   float len){
    setPlace(where->x, where->y, where->z);
    life_total = rand() % life_max;
    life_fraction = 1.0f;
    speed = rand3f(speed_max);
    rotspeed = rand3f(rot_max);
    rotation = rand3f(rot_max);
    rgb = new point3f(color);
    side = len;
}

Particle::~Particle(){
    delete speed;
    delete rotation;
    delete rotspeed;
    delete rgb;
}

void Particle::display(){

}


Particle& Particle::animate(double secPerFrame){

    
    return *this;
}
