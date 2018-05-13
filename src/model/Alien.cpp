#include "GLoid.h"
#include "Point.h"
#include "WhatUC.h"

#include "World.h"
#include "Engine.h"
#include "Game.h"

#include "Vaus.h"
#include "Ball.h"
#include "Brick.h"
#include "Alien.h"

Alien::Alien(Game * g){
    setPlace(ZERO, ZERO, SCENE_MIN - SCENE_MAX + ALIENHOME);

    /*for(i = 0; i< ALIEN_PARTICLES; i++){
            explosion[i] = particles();
        }*/
    speed = Point3f();
    game = g;
    setSize(4*baseRad, 4*baseRad, 4*baseRad);
    base = gluNewQuadric();
}

Alien::~Alien(){
    gluDeleteQuadric(base);
}

void Alien::display(){
    type = game->levelType();

    if(active){

        glPushMatrix();
        glTranslatef(place.x, place.y, place.z);

        switch(type){
        // Alien 1
        case 0:
            glPushMatrix();
            glColor3f(ZERO, ZERO, ONE);
            glTranslatef(ZERO, ZERO, ZERO);
            glRotatef(roty, ZERO, ONE, ZERO);
            glRotatef(-90.0f, ONE, ZERO, ZERO);
            glRotatef(-30.0f, ZERO, ONE, ZERO);
            glPushMatrix();
            glTranslatef(ZERO, ZERO, ZERO);
            gluQuadricDrawStyle(base, GLU_FILL);
            gluCylinder(base, ONE, ZERO, 2.0f, 12, 12);
            gluQuadricOrientation(base, GLU_INSIDE);
            gluDisk(base, 0, ONE, 12, 12);
            gluQuadricOrientation(base, GLU_OUTSIDE);
            glPopMatrix();
            glPushMatrix();
            gluDisk(base, 0, ONE, 12, 12);
            glTranslatef(-ONE, ZERO, -ONE);
            gluQuadricDrawStyle(base, GLU_FILL);
            gluCylinder(base, 1.5f, ONE, ONE, 12, 12);
            gluQuadricOrientation(base, GLU_INSIDE);
            gluDisk(base, 0, 1.5f, 12, 12);
            gluQuadricOrientation(base, GLU_OUTSIDE);
            glPopMatrix();
            glPopMatrix();
            break;
            // Alien 2
        case 1:
            
            glPushMatrix();
            glRotatef(roty, ZERO, ONE, ZERO);
            glRotatef(-45.0f, ZERO, ONE, ZERO);
            glRotatef(15.0f, ZERO, ONE, ZERO);
            glTranslatef(ZERO, ZERO, ZERO);
            glPushMatrix();
            glColor3f( ZERO, ONE, ZERO); //green
            solidTetrahedron(6.0f);
            glPopMatrix();

            glPushMatrix();
            glTranslatef ( ZERO, ZERO, ZERO );
            glColor3f( ONE, ZERO, ZERO); //red
            gluSphere(base, 2.0f, 12, 12);
            glPopMatrix();
            glPopMatrix();
            break;

            // Alien 3
        case 2:
            glPushMatrix();
            glRotatef(roty, ZERO, ONE, ZERO);
            glPushMatrix();
            glTranslatef(ZERO, 1.5f, ZERO);
            glColor3f(ONE, ZERO, ZERO);
            gluSphere(base, ONE, 12, 12);
            glPopMatrix();
            glPushMatrix();
            glColor3f(ZERO, ONE, ZERO);
            glTranslatef(0.866f, ZERO, ZERO);
            gluSphere(base, ONE, 12, 12);
            glPopMatrix();
            glPushMatrix();
            glColor3f(ZERO, ZERO, ONE);
            glTranslatef(-0.866f, ZERO, ZERO);
            gluSphere(base, ONE, 12, 12);
            glPopMatrix();
            glPopMatrix();
            break;

            // Alien 4
        case 3:
            if((game->now() % 4000) < 1000)
            {
                glPushMatrix();
                glTranslatef(ZERO, ZERO, ZERO);
                glColor3f(ONE, ZERO, ZERO);//red
                gluSphere(base, 2.0f, 12, 12);
                glPopMatrix();
            }
            else
            {
                glPushMatrix();
                glTranslatef(ZERO, ZERO, ZERO);
                glRotatef(45, ZERO, ONE, ZERO);
                glRotatef(60, ONE, ZERO, ZERO);
                glRotatef(roty, ZERO, ONE, ZERO);
                glColor3f(ONE, ZERO, ZERO);
                solidCube(4.0f);
                glPopMatrix();
            }
            break;

        default:
            break;
        }
        glPopMatrix();
    }
}

Alien &Alien::animate(double secPerFrame){
    Point3i rel = Point3i();
    Point3f alienspeed = Point3f();
    float M;
    float R;

    if(active){
        // If the alien flies off the screen, it dies
        if(place.z > ZERO){
            die();
            Vaus * v = game->getVaus();
            R = baseRad + v->rad;
            Point3f distance(place.x - v->place.x,
                            place.y - v->place.y,
                            ZERO);
            M = distance.res3f();//distance resultant
            if(M < R){
                game->killVaus();
                /*vaus->active = false;
                for(i = 0; i < VAUS_PARTICLES; i++){
               vaus->explosion[i].active = TRUE;
               vaus->explosion[i].explode(vaus->place, vaus->pal, VAUS_COLORS, vaus->particle_rad);
            }*/
            }
        }else{
            //FIXME: rel = Functions::coords(&place);
            int i = 0;
            Ball * ball = game->getActiveBall();
            if(ball){
                float ballspeed = ball->speed.res3f();
                alienspeed = place.chase(ball->place,ballspeed);
            }
            speed = speed.deepcopy(alienspeed);
            Brick * brick = game->getBrickAt(place);
            if(brick){
                speed.x = -speed.x;
                speed.y = -speed.y;
                speed.z = -speed.z;
                place.z -= size.z;
            }
            if(place.x >= SCENE_MAX - size.x/2){
                // a safer but more verbose way to confine aliens within the game scene.
                if(speed.x > ZERO){
                    speed.x = -speed.x;
                    place.x += speed.x * size.x/abs(speed.x);
                }
            }
            if(place.x <= SCENE_MIN + size.x/2){
                if(speed.x < ZERO){
                    speed.x = -speed.x;
                    place.x += speed.x * size.x/abs(speed.x);
                }
            }
            if(place.y >= SCENE_MAX - size.y/2){
                if(speed.y > ZERO){
                    speed.y = -speed.y;
                    place.y += speed.y * size.y/abs(speed.y);
                }
            }
            if(place.y <= SCENE_MIN + size.y/2){
                if(speed.y < ZERO){
                    speed.y = -speed.y;
                    place.y += speed.y * size.y/abs(speed.y);
                }
            }
            if(place.z <= SCENE_MIN - SCENE_MAX + size.z/2){
                if(speed.z < ZERO){
                    speed.z = - speed.z;
                    place.z += speed.z * size.z/abs(speed.z);
                }
            }
            place.x += speed.x * secPerFrame;
            place.y += speed.y * secPerFrame;
            place.z += zSpeed * secPerFrame;
            roty += rotSpeed * secPerFrame;
        }
    }
    return *this;
}

// Aliens blow up when they die
void Alien::die()
{
    int j;
    active = false;

    //for(j = 0; j < ALIEN_PARTICLES; j++)
    //{
    // explosion[j].active = TRUE;
    // explosion[j].explode(place, pal, 3, particle_rad);

    //}
    Game::playSound(WAV_ALIEN);
}

void Alien::solidCube(float side){
// Draw a solid cube
   side = side / 2.0f;

   glBegin(GL_QUADS);

   // Front Face
   glNormal3f(ZERO, ZERO, ONE);     // Normal Facing Forward
   glVertex3f(-side, -side,  side);  // Bottom Left Of The Texture and Quad
   glVertex3f( side, -side,  side);  // Bottom Right Of The Texture and Quad
   glVertex3f( side,  side,  side);  // Top Right Of The Texture and Quad
   glVertex3f(-side,  side,  side);  // Top Left Of The Texture and Quad

   // Back Face
   glNormal3f(ZERO, ZERO,-ONE);     // Normal Facing Away
   glVertex3f(-side, -side, -side);  // Bottom Right Of The Texture and Quad
   glVertex3f(-side,  side, -side);  // Top Right Of The Texture and Quad
   glVertex3f( side,  side, -side);  // Top Left Of The Texture and Quad
   glVertex3f( side, -side, -side);  // Bottom Left Of The Texture and Quad

   // Top Face
   glNormal3f(ZERO, ONE, ZERO);     // Normal Facing Up
   glVertex3f(-side,  side, -side);  // Top Left Of The Texture and Quad
   glVertex3f(-side,  side,  side);  // Bottom Left Of The Texture and Quad
   glVertex3f( side,  side,  side);  // Bottom Right Of The Texture and Quad
   glVertex3f( side,  side, -side);  // Top Right Of The Texture and Quad

   // Bottom Face
   glNormal3f(ZERO,-ONE, ZERO);     // Normal Facing Down
   glVertex3f(-side, -side, -side);  // Top Right Of The Texture and Quad
   glVertex3f( side, -side, -side);  // Top Left Of The Texture and Quad
   glVertex3f( side, -side,  side);  // Bottom Left Of The Texture and Quad
   glVertex3f(-side, -side,  side);  // Bottom Right Of The Texture and Quad

   // Right face
   glNormal3f(ONE, ZERO, ZERO);     // Normal Facing Right
   glVertex3f(side, -side, -side);   // Bottom Right Of The Texture and Quad
   glVertex3f(side,  side, -side);   // Top Right Of The Texture and Quad
   glVertex3f(side,  side,  side);   // Top Left Of The Texture and Quad
   glVertex3f(side, -side,  side);   // Bottom Left Of The Texture and Quad

   // Left Face
   glNormal3f(-ONE, ZERO, ZERO);    // Normal Facing Left
   glVertex3f(-side, -side, -side);  // Bottom Left Of The Texture and Quad
   glVertex3f(-side, -side,  side);  // Bottom Right Of The Texture and Quad
   glVertex3f(-side,  side,  side);  // Top Right Of The Texture and Quad
   glVertex3f(-side,  side, -side);  // Top Left Of The Texture and Quad

   glEnd();
}

void Alien::solidTetrahedron(float side)
{
   side = side / 2.0f;

   glBegin(GL_TRIANGLES);

   // Back Face
   glNormal3f(ZERO, -ONE/2.0f, -ONE);   // Normal Facing Away and 1/2 down
   glVertex3f(ZERO,  side,  side);  // Top front
   glVertex3f(-side, -side,  ZERO);  // bottom Left
   glVertex3f(side, -side,  ZERO);  // Bottom Right

   // Front Face
   glNormal3f(ZERO, -ONE/2.0f, ONE);    // Normal Facing Forward and 1/2 down
   glVertex3f(ZERO,  side, -side);  // Top Back
   glVertex3f(side, -side,  ZERO);  // Bottom Right
   glVertex3f(-side, -side,  ZERO);  // Bottom Left

   // Left Face
   glNormal3f(-ONE, ONE/2.0f, ZERO);    // Normal Facing Left and 1/2 up
   glVertex3f(ZERO,  side,  side);  // Top Front
   glVertex3f(side, -side,  ZERO);  // Bottom Right
   glVertex3f(ZERO,  side, -side);  // Top Back

   // Right face
   glNormal3f(ONE, ONE/2.0f, ZERO);     // Normal Facing Right and 1/2 up
   glVertex3f(ZERO,  side, -side);  // Top Back
   glVertex3f(-side, -side,  ZERO);  // Bottom Left
   glVertex3f(ZERO,  side,  side);  // Top Front

   glEnd();
}

