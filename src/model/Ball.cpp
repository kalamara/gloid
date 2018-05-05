#include "GLoid.h"
#include "Point.h"
#include "WhatUC.h"

#include "World.h"
#include "Engine.h"
#include "Game.h"

#include "Ball.h"
#include "Brick.h"


Ball::Ball(Game *g){
    game = g;
    speed = Point3f();
    nextbounce = Point3f();
    nextspeed = Point3f();
    initspeed = Point3f(initX, initX, 2*initX);
    launchspeed = Point3f(initspeed);
    active = false;
    rad = base_rad;
    setSize(2*rad, 2*rad, 2*rad);
    base = gluNewQuadric();
}

Ball::~Ball(){
    gluDeleteQuadric(base);
}

void Ball::display(){

    if(active){

        glPushMatrix();
            glTranslatef(place.x, place.y, place.z);
            glPushMatrix();
                glColor3f(grey, grey, grey);
                gluSphere(base, rad, slices, stacks);
            glPopMatrix();
        glPopMatrix();

    }
}

Ball& Ball::animate(double secPerFrame){
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
      game->playSound(WAV_LAUNCH);
   }
   return * this;
}
