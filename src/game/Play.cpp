#include "GLoid.h"

#include "model/WhatUC.h"
#include "model/Ball.h"
#include "model/Brick.h"
#include "Hud.h"
#include "Play.h"
#define SPLASH_LIFE 1000

Play::Play(Game &g){
    game = &g;
    type = STEP_PLAY;
    Hud::setGame(g);
    go.splash = true;
    go.lifetime = SPLASH_LIFE;
    up1.alignment = ALIGN_LEFT;
    score.alignment = ALIGN_RIGHT;

    warp = game->getTexture("warp");
}

Play::~Play(){

}

Play & Play::draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable (GL_TEXTURE_2D);

    // Reset the OpenGL viewport
    glViewport(0, 0,
               (GLsizei)(game->getScreen()->W),
               (GLsizei)(game->getScreen()->H));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the perspective
    gluPerspective(90.0f, 1, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the camera position
    gluLookAt(cameraPos.x,
              cameraPos.y,
              cameraPos.z,
              ZERO,
              ZERO,
              -SCENE_MAX,
              ZERO,
              ONE,
              ZERO);

    glRotatef(0, ONE, ZERO, ZERO);
    glRotatef(0, ZERO, ONE, ZERO);

    glTranslatef(-cameraPos.x, -cameraPos.y, 0);

    // Set the position of the light source
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    displayBackground();

    if(game->bonusMode == B)
        displayWarp();

    glTranslatef(ZERO, ZERO, -SCENE_AIR);

    glColor4f(ZERO, ZERO, ZERO, ONE);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
//display 3d stuff
/* . . . */
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glColor4f(ONE, ONE, ONE, ONE);
    glDisable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set camera position
    gluLookAt(ZERO,
             ZERO,
             10.0,
             ZERO,
             ZERO,
             -SCENE_MAX,
             ZERO,
             ONE,
             ZERO);


    drawText(score);
    //draw down left hud

    drawText(up1);
    drawText(go);
    SDL_GL_SwapBuffers();

    return *this;
}

Play & Play::update(){
    if(phase==PLAY_GO){
        clearText();
        phase ++;
        printText("GO",go);
        game->playSound("go");

    }else{

        printText("1UP", up1, Red, 0);
        std::stringstream scorestr;
        scorestr << game->score;
        printText(scorestr.str(), up1, White, 1);

        printText("HIGH SCORE", score, Red, 0);
        std::stringstream histr;
        histr << SCORELIFE;
        printText(histr.str(), score, White, 1);
    }
    return *this;
}

int Play::next(){
    return STEP_PLAY;
}


// Display background
void Play::displayBackground(){
   // float step = 10.0f;
   background = game->getTexture(game->getBackgroundTextureFilename());
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, background.first);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D,
                0,
                3,
                background.second.w,
                background.second.h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                background.second.pixels);

   // Back face
   glBegin(GL_QUADS);
   glTexCoord2f(ZERO, ZERO);
   glVertex3f(SCENE_MIN, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(ZERO, BGD_SIZE);
   glVertex3f(SCENE_MIN, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, BGD_SIZE);
   glVertex3f(SCENE_MAX, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, ZERO);
   glVertex3f(SCENE_MAX, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glEnd();

   // Top face
   glBegin(GL_QUADS);
   glTexCoord2f(ZERO, ZERO);
   glVertex3f(SCENE_MIN, SCENE_MAX, -SCENE_AIR);
   glTexCoord2f(ZERO, BGD_SIZE);
   glVertex3f(SCENE_MIN, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, BGD_SIZE);
   glVertex3f(SCENE_MAX, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, ZERO);
   glVertex3f(SCENE_MAX, SCENE_MAX, -SCENE_AIR);
   glEnd();

   // Bottom face
   glBegin(GL_QUADS);
   glTexCoord2f(ZERO, ZERO);
   glVertex3f(SCENE_MIN, SCENE_MIN, -SCENE_AIR);
   glTexCoord2f(ZERO, BGD_SIZE);
   glVertex3f(SCENE_MIN, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, BGD_SIZE);
   glVertex3f(SCENE_MAX, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, ZERO);
   glVertex3f(SCENE_MAX, SCENE_MIN, -SCENE_AIR);
   glEnd();

   // Right face
   glBegin(GL_QUADS);
   glTexCoord2f(ZERO, ZERO);
   glVertex3f(SCENE_MAX, SCENE_MIN, -SCENE_AIR);
   glTexCoord2f(ZERO, BGD_SIZE);
   glVertex3f(SCENE_MAX, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, BGD_SIZE);
   glVertex3f(SCENE_MAX, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, ZERO);
   glVertex3f(SCENE_MAX, SCENE_MAX, -SCENE_AIR);
   glEnd();

   // Left face
   glBegin(GL_QUADS);
   glTexCoord2f(ZERO, ZERO);
   glVertex3f(SCENE_MIN, SCENE_MIN, -SCENE_AIR);
   glTexCoord2f(ZERO, BGD_SIZE);
   glVertex3f(SCENE_MIN, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, BGD_SIZE);
   glVertex3f(SCENE_MIN, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(BGD_SIZE, ZERO);
   glVertex3f(SCENE_MIN, SCENE_MAX, -SCENE_AIR);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

// Display warp effect
void Play::displayWarp(){

   float one, zero;

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, warp.first);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGB,
                warp.second.w,
                warp.second.h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                warp.second.pixels);

   glPushMatrix();

   // Flip texture in alternating frames
   if(flipWarp){
      one = ONE;
      zero = ZERO;
      flipWarp = false;
   }else{
      one = ZERO;
      zero = ONE;
      flipWarp = true;
   }

   glBegin(GL_QUADS);
   glTexCoord2f(one, zero);
   glVertex3f(SCENE_MAX, SCENE_MIN, -(SCENE_AIR + WARP_SIZE));

   glTexCoord2f(one, one);
   glVertex3f(SCENE_MAX, SCENE_MIN + WARP_SIZE, -(SCENE_AIR + WARP_SIZE));

   glTexCoord2f(zero, one);
   glVertex3f(SCENE_MAX, SCENE_MIN + WARP_SIZE, -SCENE_AIR);

   glTexCoord2f(zero, zero);
   glVertex3f(SCENE_MAX, SCENE_MIN, -SCENE_AIR);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
