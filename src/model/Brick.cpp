#include "GLoid.h"

#include "WhatUC.h"
#include "Pill.h"
#include "Brick.h"

Brick::Brick(Game &g,
             const Point3f &color,
             const Point3i &coords,
             int t) : pill(Brick::fromBrick(coords), g){
    game = &g;
    active = true;
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
    Point3f where = Brick::fromBrick(coords);
    setPlace(where.x, where.y, where.z);
}

// Brick was hit by ball or shot
void Brick::hit(){
    if(active){
        if(hit_counter != 1){
           hit_effect = Engine<Game>::toc();
        }
        if(hit_counter > 0){
            hit_counter--;
            if(hit_counter == 0) {
               game->score += SCOREBRICK;
               active = false;
               game->brickCount--;
               pill.active = true;
            };
        }
    }
}

// Display bricks
void Brick::display(){
    if(active){
        glPushMatrix();
        glTranslatef(place.x,place.y,place.z);
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
        solidRhombik();
        glPopMatrix();
        glDisable(GL_BLEND);
        glPopMatrix();
    }
}

Brick& Brick::animate(double secPerFrame){
    return *this;
}

void Brick::solidRhombik(){

    for(uint8_t dim = 0; dim < 3; dim++){
        for(uint8_t dir = 0; dir < 2;dir++){
            uint8_t i = 2*dim + dir;
            glNormal3f(face[i].x,
                       face[i].y,
                       face[i].z);		// Normal Facing
        }
        glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips
        for(uint8_t j = 0; j < N_PATHS; j++){//front, left, back, right faces
            uint8_t jj = j & 3; //j mod 4
            glNormal3f(face[facepath[jj]].x,
                       face[facepath[jj]].y,
                       face[facepath[jj]].z);		// Normal Facing
            glVertex3f(vertice[path[j]].x,
                       vertice[path[j]].y,
                       vertice[path[j]].z);
        }
        glEnd();					// Done Drawing Quads
    }
    // Top Face
    glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips
    glNormal3f(ZERO, ONE, ZERO);		// Normal Facing Up
    for(uint8_t j = 0; j < N_PATHS+1; j++){
        glVertex3f(vertice[toppath[j]].x,
                   vertice[toppath[j]].y,
                   vertice[toppath[j]].z);
    }
    glEnd();					// Done Drawing Strips

    glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips

    // Bottom Face
    glNormal3f(ZERO, -ONE, ZERO);		// Normal Facing Down

    for(uint8_t j = 0; j < N_PATHS+1; j++){
        glVertex3f(vertice[botpath[j]].x,
                   vertice[botpath[j]].y,
                   vertice[botpath[j]].z);
    }
    glEnd();					// Done Drawing Strips
}

/***********************************Brick parser ******************************/

std::optional<Point3f>  Brick::getElement(std::string line, std::string header){

    if(line.compare(0, header.length(), header)){
        return {};
    }
    float x = ZERO;
    float y = ZERO;
    float z = ZERO;
    try{
        std::stringstream str(line.substr(header.length()));
        std::string xstr;
        std::string ystr;
        std::string zstr;
        str >> xstr >> ystr >> zstr;
        x = std::stof(xstr);
        y = std::stof(ystr);
        z = std::stof(zstr);
    }catch(...){
        std::cout << "exception:" << line << std::endl;
        return {};
    }
    auto p = std::make_unique<Point3f>(x,y,z);
    return *p;
}

std::optional<Point3i> Brick::getPos(std::string line){

    std::optional<Point3f> fl = getElement(line, "TM_POS");
    if(fl){

        return Point3i((int)fl.value().x/10.0f,
                       (int)fl.value().y/10.0f,
                       (int)fl.value().z/10.0f);
    }else{
        return {};
    }
}

std::optional<Point3f> Brick::getColor(std::string line){

    return getElement(line, "WIREFRAME_COLOR");
}

std::optional<Brick> Brick::getBrick(std::istream & ifs, Game * game){
    std::string geom    = "";
    std::string posstr  = "";
    std::string colstr  = "";

    while(geom.compare(0, 10,"GEOMOBJECT")
    && std::getline(ifs, geom, '*'));

    std::getline(ifs, posstr, '*'); //TM_POS
    auto pos = getPos(posstr);
    std::getline(ifs, colstr, '*'); //WIREFRAME_COLOR
    auto col = getColor(colstr);

    if(pos
    && col){
        unsigned int type  = BRIK_NORMAL;
        if(col.value().eq(Point3f(0.3,0.3,0.3))){
            type = BRIK_SILVER;
        }
        if(col.value().eq(Point3f(0.4,0.4,0.0))){
            type = BRIK_GOLDEN;
        }
        return Brick(*game, col.value(), pos.value(), type);
    }
    return {};
}



