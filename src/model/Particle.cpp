#include "GLoid.h"
#include "WhatUC.h"
#include "Particle.h"
#include "Functions.h"

using namespace Functions;

Particle::Particle(const point3f_t where,
                   const point3f_t color,
                   float len){
    setPlace(where->x, where->y, where->z);
    life_total = rand() % life_max + 1;
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
    glPushMatrix();
    glTranslatef(place.x, place.y, place.z);
    glRotatef(rotation->x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation->y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation->z, 0.0f, 0.0f, 1.0f);

    glColor4f(rgb->x, rgb->y, rgb->z, life_fraction * life_fraction);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(0.0f, side, 0.0f);
    glVertex3f(0.0f,-side, 0.0f);
    glVertex3f(side, 0.0f, 0.0f);
    glEnd();

    glPopMatrix();
}


Particle& Particle::animate(double secPerFrame){
    place.x += speed->x * secPerFrame;
    place.y += speed->y * secPerFrame;
    place.z -= speed->z * secPerFrame;

    rotation->x += rotspeed->x * secPerFrame;
    rotation->y += rotspeed->y * secPerFrame;
    rotation->z -= rotspeed->z * secPerFrame;

    life_fraction -= secPerFrame*1000/life_total;
    if(life_fraction < FLOAT_PRECISION){
          active = FALSE;
    }
    return *this;
}
