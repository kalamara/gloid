#include "GLoid.h"

#include "WhatUC.h"
#include "Crosshair.h"
#include "Ball.h"
#include "Pill.h"
#include "Brick.h"
#include "Particle.h"
#include "Shot.h"
#include "Vaus.h"
#include "Alien.h"

Ball::Ball(Game &g) : cross(g){
    game = &g;
    speed = Point3f();
    nextbounce = Point3f();
    nextspeed = Point3f();
    initspeed = Point3f(-initX, -initX, -2.0f * initX);
    launchspeed = Point3f(initspeed);
    active = true;
    rad = base_rad;
    setSize(2*rad, 2*rad, 2*rad);
}

Ball::~Ball(){

}

void Ball::display(){
    base = gluNewQuadric();
    if(active){

        glPushMatrix();
            glTranslatef(place.x, place.y, place.z);
            glPushMatrix();
                glColor3f(grey, grey, grey);
                gluSphere(base, rad, slices, stacks);
            glPopMatrix();
        glPopMatrix();

    }
    gluDeleteQuadric(base);
}

Ball& Ball::restrain(){
    if(place.z < SCENE_MIN - SCENE_MAX){
        if(speed.z < 0){
            speed.z = -speed.z;
        }
        place.z = SCENE_MIN - SCENE_MAX;
    }
    if(place.x > SCENE_MAX){
        if(speed.x > 0){
            speed.x = -speed.x;
        }
        place.x = SCENE_MAX;
    }
    if(place.x < SCENE_MIN){
        if(speed.x < 0){
            speed.x = -speed.x;
        }
        place.x =  SCENE_MIN;
    }
    if(place.y > SCENE_MAX){
        if(speed.y > 0){
            speed.y = -speed.y;
        }
        place.y =  SCENE_MAX;
    }
    if(place.y < SCENE_MIN){
        if(speed.y < 0){
            speed.y = -speed.y;
        }
        place.y =  SCENE_MIN;
    }
    return *this;
}

Ball& Ball::animate(double secPerFrame){
    float U = ZERO;
    Vaus vaus = game->getVaus();
    // Hasn't been launched yet
    if(!launched){
        U = 2*launchspeed.res3f();
        setPlace(vaus.prevx + vaus.size.x * launchspeed.x / U,
                 vaus.prevy + vaus.size.y * launchspeed.y / U,
                -vaus.size.z / 2 - rad);

        cross.update(place, launchspeed);
        if(vaus.fire){
            launch();
        }
    } else {
        //keep it in the box!
        auto brick = game->getBrickAt(cross.place, cross.axis);

        if(brick && collides(brick.value().place, brick.value().size) ){
            bounce(cross.axis);
            auto b = brick.value().hit();
            game->bricks[Brick::toBrick(cross.place, cross.axis)] = b;
        }/*
        // If it hits an alien, destroy it and ricochet randomly
        for(auto a: game->aliens){
            if(collides(a.place, a.size)){
                bounce();
                //a = a.die();
            }
        }*/
        // If it hits the Vaus
        if(collides(game->vaus->place, game->vaus->size)){
            bounce(AXIS_Z);
        }
        // If the ball flies off the screen, it is lost
        // Set the ball speed
        place.x += speed.x * secPerFrame;
        place.y += speed.y * secPerFrame;
        place.z += speed.z * secPerFrame;

        restrain();
        cross.update(place, speed);
    }
    return *this;
}

Ball& Ball::reinit(const Point3f &init){
    if(!active){
          active = true;
    }
    launched = false;
    speed = Point3f();
    nextbounce = Point3f();
    launchspeed = launchspeed.deepcopy(init);
    nextspeed = nextspeed.deepcopy(launchspeed);

    return *this;
}

// Launch ball
Ball& Ball::launch(){
   if(!launched){
      launched = true;
      speed = speed.deepcopy(launchspeed);
      game->playSound("launch");
      cross.launched = true;
   }
   return * this;
}

Ball Ball::getBall(Game * g){
    return Ball(*g);
}

bool Ball::collides(const Point3f pl,  Point3f sz){
    auto diff = Point3f(pl).sub3f(place);

    if(diff.proj3f(speed) > FLOAT_PRECISION
    && diff.res3f() < base_rad + sz.res3f()){
        return true;
    }
    return false;
}

Ball& Ball::bounce(int axis){

        switch(axis){
        case AXIS_X:
        case -AXIS_X:
            speed.x = -speed.x;
            break;
        case AXIS_Y:
        case -AXIS_Y:
            speed.y = -speed.y;
            break;
        case AXIS_Z:
        case -AXIS_Z:
            speed.z = -speed.z;
            break;
        default:
            break;
        }

    return * this;
}

Ball& Ball::bounce(const Point3f pl, const Point3f sz){

        if(abs(sz.x)<FLOAT_PRECISION){

            return bounce(AXIS_X);
        }
        if(abs(sz.y)<FLOAT_PRECISION){

            return bounce(AXIS_Y);
        }
        if(abs(sz.z)<FLOAT_PRECISION){

            return bounce(AXIS_Z);
        }
        //initial speed norm
        auto n = speed.res3f();
        //find distance
        auto dst = Point3f(place).sub3f(pl);
        //calculate speed projection on distance
        auto l = speed.proj3f(dst);
        auto prj = Point3f(dst).mul3f(l);

        //vertical component to projection
        auto vrc = Point3f(speed).sub3f(prj);

        //scale and subtract from initial speed
        speed = vrc.sub3f(prj);
        speed.norm3f(n);
        speed.scale3f( Point3f(ONE / sz.x, ONE / sz.y, ONE / sz.z));

    return * this;
}

Ball& Ball::bounce(){

        speed = Point3f(speed.res3f());
        if(speed.res3f() <3 * MINSPEED){
            speed = initspeed;
        }

    return * this;
}
