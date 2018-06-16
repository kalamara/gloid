#include "GLoid.h"

#include "game/Play.h"
#include "WhatUC.h"
#include "Vaus.h"

#include "Pill.h"

Pill::Pill(const Point3f &where,
           Game *g,
           Play* p){
    game = g;
    gameplay = p;
    setSize(5.0f, 2*rad, 2*rad);
    setPlace(where.x,
             where.y,
             where.z);
    SDL_Color b;
    SDL_Color f;
    base = gluNewQuadric();
    type = roulette(gameplay->isHiScoring());
    col.x = Palette[type].b/255;
    col.y = Palette[type].g/255;
    col.z = Palette[type].r/255;

    b = Palette[type];
    f = Palette[RGB_BLACK];

    label = PillLabels[type];

    surf = SDL_CreateRGBSurface(
                0,
                game->getFontSize(),
                game->getFontSize(),
                game->getScreen()->BPP,
                0x00ff0000,
                0x0000ff00,
                0x000000ff,
                0xff000000);

    //memset(&text,0,sizeof(text2d));

    textSurf = SDL_CreateRGBSurface(
                0,
                game->getFontSize(),
                game->getFontSize(),
                game->getScreen()->BPP,
                0x00ff0000,
                0x0000ff00,
                0x000000ff,
                0xff000000); //TODO: or maybe print2d ??

    SDL_SetColorKey(
                textSurf,
                SDL_SRCCOLORKEY | SDL_RLEACCEL,
                SDL_MapRGBA(textSurf->format, 0, 0, 0, 0));
//    text = new text2d(f,b);
//    text = text->print(-1, label);

    SDL_FillRect(
                surf,
                NULL,
                SDL_MapRGBA(surf->format, b.r, b.g, b.b, 128));
    SDL_BlitSurface(textSurf, 0, surf, 0);
}

// Deallocate SDL_Surface data
Pill::~Pill(){
    SDL_FreeSurface(surf);
    SDL_FreeSurface(textSurf);
    gluDeleteQuadric(base);
    delete text;
}

void Pill::display(){

    if(active){
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glColor3f(col.x, col.y, col.z);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTranslatef(place.x,place.y, place.z);
        gluQuadricTexture(base, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     game->getFontSize(),
                     game->getFontSize(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE,
                     surf->pixels);
        glPushMatrix();
        glPushMatrix();
        glRotatef(rotx, -1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glTranslatef(len/2, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(base, rad, rad, len, 12, 12);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glColor3f(col.x, col.y, col.z);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(0, 0.0f, 0.0f);
        gluSphere(base, rad, 12, 12);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(len, 0.0f, 0.0f);
        gluSphere(base, rad, 12, 12);
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }
}

Pill& Pill::animate(double secPerFrame){

    if(active){
        Vaus * v = gameplay->getVaus();
        if((place.z >= v->size.z/2 - rad)
                && collides(v->place.x - v->rad,
                            v->place.x + v->rad,
                            v->place.y + v->rad,
                            v->place.y - v->rad)) {
            score += SCOREBONUS;
            active = false;
            v->reset(); //maybe game->resetVaus();
            gameplay->setBonusMode(type);

            if(type == P){
                gameplay->incLives();
                game->playSound(WAV_KANONAKI);
            }
            if(type == D){
                gameplay->divideBalls();
            }
        }else{
            if(place.z > 1.0f){
                active = false;
            }
        }
    }
    place.z -= speedz * secPerFrame;
    rotx += rotspeed * secPerFrame;

    return *this;
}

// Generate a random bonus pill
int Pill::roulette(bool hiScore)
{
    int i = rand();
    // A low-scoring game, 83.5% chance of no bonus pill
    if(hiScore){
        if(i < RAND_MAX / 20)          // 5%
            return D;
        if(i < 3 * (RAND_MAX / 40))    // 2.5%
            return L;
        if(i < RAND_MAX / 10)          // 2.5%
            return G;
        if(i < RAND_MAX / 8)           // 2.5%
            return E;
        if(i < 3 * (RAND_MAX / 20))    // 2.5%
            return S;
        if(i < 4 * (RAND_MAX / 25))    // 1%
            return B;
        if(i < 33 * (RAND_MAX / 200))  // 0.5%
            return P;
    }else{// A high-scoring game, 76% chance of no bonus pill
        if(i < RAND_MAX / 20)          // 5%
            return L;
        if(i < RAND_MAX / 10)          // 5%
            return E;
        if(i < 3 * (RAND_MAX / 20))    // 5%
            return G;
        if(i < RAND_MAX / 5)           // 5%
            return S;
        if(i < 9 * (RAND_MAX / 40))    // 2.5%
            return D;
        if(i < 47 * (RAND_MAX / 200))  // 1%
            return P;
        if(i < 6 * (RAND_MAX / 25))    // 0.5%
            return B;
    }
    return -1;
}

bool Pill::collides(float left,
                    float right,
                    float up,
                    float down){
    float l = place.x - size.x/2;
    float r = place.x + size.x/2;
    float u = place.y + rad;
    float d = place.y - rad;
    if((((l <= left) && (left <= r)) ||
        ((l <= right) && (right <= r))) &&
            (((d <= down) && (down <= u)) ||
             ((d <= up) && (up <= u)))){
        return true;
    }else{
        if((((left <= l) && (l <= right)) ||
            ((left <= r) && (r <= right))) &&
                (((down <= d) && (d <= up)) ||
                 ((down <= u) && (u <= up)))){
            return true;
        }else{
            return false;
        }
    }
}
