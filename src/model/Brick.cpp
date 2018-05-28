#include "GLoid.h"
#include "Point.h"
#include "WhatUC.h"

#include "World.h"
#include "Engine.h"
#include "Game.h"

#include "Brick.h"

Brick::Brick(Game *g,
             const Point3f &color,
             const Point3i &coords,
             int t){
    active = true; //FALSE;
    type = t;
    Point3f gold = GOLD;
    Point3f silver = SILVER;
    switch(type){
    case BRIK_GOLDEN:
        hit_counter = 0;
        rgb =  Point3f(gold);
        break;

    case BRIK_SILVER:
        hit_counter = 2;
        rgb =  Point3f(silver);
        break;

    default:
        hit_counter = 1;
        rgb = Point3f(color);
        break;
    }
    setSize(side, side, depth);
    Point3f where = Point3f(coords);
    setPlace(where.x, where.y, where.z);
}

Brick::~Brick(){
}

// Brick was hit by ball or shot
int Brick::hit(){

    extern unsigned int Brick_count;
    int score = 0;/*
   int X;
   int Y;
   int Z;
   double tic;

   X = (int)((place.x - SCENE_MIN - 2.5f)/5);
   Y = (int)((place.y - SCENE_MIN - 2.5f)/5);
   Z = (int)((place.z + 1.25f)/2.5f);

   tic = SDL_GetTicks();

   if(active)
   {
      if(hit_counter != 1)
         hit_effect = tic;

      // Not a gold brick
      if(hit_counter > 0)
      {
         hit_counter--;
         if(hit_counter == 0)
         {
            score = SCOREBRICK;
            active = FALSE;
            Brick_count--;

            // Randomly determine bonus pill
            Pill[X][Y][Z].init();

            if(Pill[X][Y][Z].type > -1)
               Pill[X][Y][Z].active = TRUE;

            Pill[X][Y][Z].setplace(place.x,place.y,place.z);
         }
      }
   }*/
    return score;
}

// Display bricks
void Brick::display(){
    if(active){
        glEnable(GL_BLEND);
        glPushMatrix();
        glRotatef(HALF_CIRCLE, ZERO, ZERO, ONE);
        glRotatef(HALF_CIRCLE, ZERO, ONE, ZERO);
        glScalef(size.x, size.y, size.z);

        // Normal brick
        if(type == 0 || game->toc() - hit_effect < duration){
            glDisable(GL_BLEND);
            glColor3f(rgb.x, rgb.y, rgb.z);
        }else{
            hit_effect = 0;
            glColor4f(rgb.x, rgb.y, rgb.z, opacity);
        }
        solidRhombik(ONE);
        glPopMatrix();
        glDisable(GL_BLEND);
    }
}

Brick& Brick::animate(double secPerFrame){
    return *this;
}

Point3f Brick::computeVertex(int tog,
                             int dir,
                             int dim,
                             int neg,
                             Point3f& vertex)
{
    float unary = side/2.0f;	//unit
    float big = unary*1.5f;
    switch(dim){
    case 0:
        vertex.z = (1-2*dir)*big;
        if(dir==0){
            vertex.y = (2*neg-1)*unary;
            vertex.x = -vertex.y*(2*tog-1);
        }else{
            vertex.x = (2*neg-1)*unary;
            vertex.y = -vertex.x*(2*tog-1);
        }
        break;
    case 1:
        vertex.y = (1-2*dir)*big;
        if(dir==0){
            vertex.x = (2*neg-1)*unary;
            vertex.z = -vertex.x*(2*tog-1);
        }else{
            vertex.z = (2*neg-1)*unary;
            vertex.x = -vertex.z*(2*tog-1);
        }
        break;
    case 2:
        vertex.x =(1-2*dir)*big;
        if(dir==0){
            vertex.z = (2*neg-1)*unary;
            vertex.y = -vertex.z*(2*tog-1);
        }else{
            vertex.y = (2*neg-1)*unary;
            vertex.z = -vertex.y*(2*tog-1);
        }
        break;
    }
    return vertex;
}

void Brick::solidRhombik(float side){

    Point3f vertice[24], face[6];
    int path[18]  = {3,0,2,1,18,19,17,16,6,7,5,4,23,20,22,21,3,0};
    int toppath[19] = {17,11,18,10, 2,10,3,9,22, 8,23, 5,6, 8,17,11,10, 8, 9};
    int botpath[19] = {15,12,14,13,16,12,7,4,20,12,21,15,0,14,1 ,14,19,13,16};
    int facepath[4] = {0,4,1,5};
    /*	//FYI
        // Front Face
        vertice[0] = {-unary, -unary,  big};
        vertice[1] = {unary, -unary,  big};
        vertice[2] = {unary,  unary,  big};
        vertice[3] = {-unary,  unary,  big};
        // Back Face
        vertice[4] = {-unary, -unary, -big};
        vertice[5] = {-unary,  unary, -big};
        vertice[6] = {unary,  unary, -big};
        vertice[7] = { unary, -unary, -big};
        // Top Face
        vertice[8] = {-unary,  big, -unary};
        vertice[9] = {-unary,  big,  unary};
        vertice[10] = { unary,  big,  unary};
        vertice[11] = {unary,  big, -unary};
        // Bottom Face
        vertice[12] = {-unary, -big, -unary};
        vertice[13] = {unary, -big, -unary};
        vertice[14] = {unary, -big,  unary};
        vertice[15] = {-unary, -big,  unary};
        // Right face
        vertice[16] = {big, -unary, -unary};
        vertice[17] = {big,  unary, -unary};
        vertice[18] = {big,  unary,  unary};
        vertice[19] = {big, -unary,  unary};
        // Left Face
        vertice[20] = {-big, -unary, -unary};
        vertice[21] = {-big, -unary,  unary};
        vertice[22] = {-big,  unary,  unary};
        vertice[23] = {-big,  unary, -unary};

        face[0] = {0.0f, 0.0f, 1.0f};
        face[1] = {0.0f, 0.0f,-1.0f};
        face[2] = {0.0f, 1.0f, 0.0f};
        face[3] = {0.0f,-1.0f, 0.0f};
        face[4] = {1.0f, 0.0f, 0.0f};
        face[5] = {-1.0f, 0.0f, 0.0f};*/

    for(int dim = 0; dim <3; dim++){
        for(int dir = 0; dir<2;dir++){
            int i = 2*dim+dir;
            switch(dim){
            case 0:	face[i].z = ONE-2*dir;
                break;
            case 1: face[i].y = ONE-2*dir;
                break;
            case 2: face[i].x = ONE-2*dir;
                break;
            }
            glNormal3f( face[i].x, face[i].y, face[i].z);		// Normal Facing
            for(int neg = 0; neg <2; neg ++){
                for(int tog = 0; tog <2; tog++){
                    int j = 4*i+2*neg+tog;
                    vertice[j] = computeVertex(tog, dir, dim, neg, vertice[j]);
                }
            }
        }
        //glEnd();					// Done Drawing Quads

        glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips
        for(int j = 0; j <18; j++){//front, left, back, right faces
            glNormal3f(face[facepath[j%4]].x,face[facepath[j%4]].y,face[facepath[j%4]].z);		// Normal Facing
            glVertex3f(vertice[path[j]].x,  vertice[path[j]].y,  vertice[path[j]].z);
            //Log(" E[%d] = (%g,%g,%g) V[%d] = (%g,%g,%g)\n", i, face[i].x, face[i].y, face[i].z, j, vertice[j].x, vertice[j].y, vertice[j].z );
        }
        glEnd();					// Done Drawing Quads
    }

    // Top Face

    glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips
    glNormal3f( 0.0f, 1.0f, 0.0f);		// Normal Facing Up
    for(int j = 0; j <19; j++){
        glVertex3f(vertice[toppath[j]].x,  vertice[toppath[j]].y,  vertice[toppath[j]].z);
        //Log(" E[%d] = (%g,%g,%g) V[%d] = (%g,%g,%g)\n", i, face[i].x, face[i].y, face[i].z, j, vertice[j].x, vertice[j].y, vertice[j].z );
    }
    glEnd();					// Done Drawing Strips

    glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips

    // Bottom Face

    glNormal3f( 0.0f,-1.0f, 0.0f);		// Normal Facing Down

    for(int j = 0; j <19; j++){
        glVertex3f(vertice[botpath[j]].x,  vertice[botpath[j]].y,  vertice[botpath[j]].z);
        //Log(" E[%d] = (%g,%g,%g) V[%d] = (%g,%g,%g)\n", i, face[i].x, face[i].y, face[i].z, j, vertice[j].x, vertice[j].y, vertice[j].z );
    }
    glEnd();					// Done Drawing Strips
}
