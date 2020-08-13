#include "GLoid.h"

#include "WhatUC.h"
#include "Crosshair.h"
#include "Ball.h"
#include "Pill.h"
#include "Brick.h"
#include "Particle.h"
#include "Shot.h"
#include "Vaus.h"

Ball::Ball(Game &g) : cross(g){
    game = &g;
    speed = Point3f();
    nextbounce = Point3f();
    nextspeed = Point3f();
    initspeed = Point3f(initX, initX, 2*initX);
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

Ball& Ball::animate(double secPerFrame){
    float U = ZERO;
    Vaus vaus = game->getVaus();
    if(!launched){// Hasn't been launched yet
        U = 2*launchspeed.res3f();
        setPlace(vaus.prevx + vaus.size.x * launchspeed.x / U,
                 vaus.prevy + vaus.size.y * launchspeed.y / U,
                -vaus.size.z / 2 - rad);

        cross.update(place, launchspeed);
        if(vaus.fire){
            launch();
        }
    } else {
        place.x += speed.x * secPerFrame;
        place.y += speed.y * secPerFrame;
        place.z -= speed.z * secPerFrame;

        if(place.z < SCENE_MIN - SCENE_MAX) {
           place.z += rad;
           bounce(AXIS_Z);
        }
        if(place.x > SCENE_MAX) {
           place.x -= rad;
           bounce(AXIS_X);
        }
        if(place.x < SCENE_MIN){
           place.x += rad;
           bounce(AXIS_X);
        }
        if(place.y > SCENE_MAX){
           place.y -= rad;
           bounce(AXIS_Y);
        }
        if(place.y < SCENE_MIN){
           place.y += rad;
           bounce(AXIS_Y);
        }
        cross.update(place, speed);
    }
 /*  float M, U, R, U2;
   point3f randspeed, bouncespeed, center;
   int found, i = 0;
   int score = 0;
   int axisX, axisY, axisZ;
   point3i rel;
   float AR;

   if(active)
   {
      // Hasn't been launched yet
      if(!launched)
      {
         U = 2 * res3f(launchspeed.x, launchspeed.y, launchspeed.z);

         setplace(Vaus[0].prevx + Vaus[0].size.x * launchspeed.x / U,
                  Vaus[0].prevy + Vaus[0].size.y * launchspeed.y / U,
                 -Vaus[0].size.z / 2 - rad);

         nextaxis = bounce(launchspeed);
      }
      else
      {
         U = res3f(nextspeed.x, nextspeed.y, nextspeed.z);

         rel = brickcoords(place);

         axisZ = rel.Z + 1;
         axisX = rel.X;
         axisY = rel.Y;

         // Bounce from brick
         if(Brick[axisX][axisY][axisZ].active)
         {
            score += Brick[axisX][axisY][axisZ].hit();
            PlaySDLSound(WAV_BOUNCE1);

            switch(nextaxis)
            {
               case 1:
                  speed.x = -speed.x;
                  place.x -= rad;
                  break;

               case -1:
                  speed.x = -speed.x;
                  place.x += rad;
                  break;

               case 2:
                  speed.y = -speed.y;
                  place.y -= rad;
                  break;

               case -2:
                  speed.y = -speed.y;
                  place.y += rad;
                  break;

               case 3:
                  speed.z = -speed.z;
                  place.z -= rad;
                  break;

               case -3:
                  speed.z = -speed.z;
                  place.z += rad;
                  break;
            }
            nextaxis = bounce(speed);
         }

         // If it hits an alien, destroy it and ricochet randomly
         for(i = 0; i < ALIENNUM; i++)
         {
            AR = res3f(Alien[i].place.x - place.x,
                       Alien[i].place.y - place.y,
                       Alien[i].place.z - place.z);

            if(Alien[i].active && (AR <= rad + Alien[i].rad))
            {
               score += SCOREALIEN;
               do//if random speed is too slow on any axis, try again
                 randspeed = rand3f(U);
               while(abs(randspeed.z) < MINSPEED || abs(randspeed.x) < MINSPEED ||abs(randspeed.y) < MINSPEED);
               nextaxis = bounce(randspeed);
               speed.x = randspeed.x;
               speed.y = randspeed.y;
               speed.z = randspeed.z;
               
               Alien[i].die();
            }
         }

         // If it hits the Vaus
         if(place.z >= Vaus[0].size.z/2 - rad)
         {
            R = rad + Vaus[0].base_rad;
            M = res3f(place.x - Vaus[0].place.x, place.y - Vaus[0].place.y, 0.0f);

            // Bounce the ball off the Vaus
            if(M < R)
            {
               // Play bounce sound
               PlaySDLSound(WAV_BOUNCE0);

               // reverse Z travel
               speed.z = -speed.z;

               // If the fire button is down, bounce spherically; otherwise, bounce flat
               if(Vaus[0].fire)
               {
                  // The magnitude of the velocity
                  U = sqrt(6) * 10.0f;

                  // Find the center of Vaus
                  center.x = Vaus[0].place.x;
                  center.y = Vaus[0].place.y;
                  center.z = Vaus[0].base_rad;

                  // Calculate the vector from the center of Vaus to the ball
                  bouncespeed = chase(center, place, U);

                  // Add the initial velocity to the bounce velocity
                  speed.x += bouncespeed.x;
                  speed.y += bouncespeed.y;
                  speed.z += bouncespeed.z;

                  // Calculate the new resultant 
                  U2 = res3f(speed.x, speed.y, speed.z);

                  // Normalize with the old one
                  speed.x = U / U2 * speed.x;
                  speed.y = U / U2 * speed.y;
                  speed.z = U / U2 * speed.z;                  
               }

               if(Bonus_mode == G)
                  reinit(speed);

               place.z = -Vaus[0].size.z/2-rad;
               nextaxis = bounce(speed);
            }
         }

         // If the ball flies off the screen, it is lost
         if(place.z > 1.0f)
         {
            active = FALSE;
            found = FALSE;

            for(i = 0; i < BALLNUM; i++)
            {
               if(Ball[i].active)
               {
                  found = TRUE;
                  break;
               }
            }

            // If there are no more active balls, lose a life
            if(!found)
            {
               Vaus[0].active = FALSE;

               for(i = 0; i < VAUS_PARTICLES; i++)
               {
                  Vaus[0].explosion[i].active = TRUE;
                  Vaus[0].explosion[i].explode(Vaus[0].place,
                     Vaus[0].pal, VAUS_COLORS, Vaus[0].particle_rad);
               }
            }
         }

         // Set the ball speed
         if(Bonus_mode == S)
         {
            place.x += speed.x * ms10/1414;
            place.y += speed.y * ms10/1414;
            place.z -= speed.z * ms10/1414;
         }
         else
         {
            place.x += speed.x * ms10/1000;
            place.y += speed.y * ms10/1000;
            place.z -= speed.z * ms10/1000;
         }

         if(place.z < SCENE_MIN - SCENE_MAX)
         {
            place.z +=rad;
            speed.z = -speed.z;
            nextaxis = bounce(speed);
         }

         if(place.x > SCENE_MAX)
         {
            place.x -= rad;
            speed.x = -speed.x;
            nextaxis = bounce(speed);
         }

         if(place.x < SCENE_MIN)
         {
            place.x += rad;
            speed.x = -speed.x;
            nextaxis = bounce(speed);
         }
         if(place.y > SCENE_MAX)
         {
            place.y -= rad;
            speed.y = -speed.y;
            nextaxis = bounce(speed);
         }

         if(place.y < SCENE_MIN)
         {
            place.y += rad;
            speed.y = -speed.y;
            nextaxis = bounce(speed);
         }
      }
   }
   return score;*/
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
    if(place.dist(pl) < base_rad + sz.res3f()){
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
