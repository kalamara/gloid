#include "GLoid.h"
#include "Go.h"

Go::Go(){
    type = STEP_GO;
}

Go::~Go(){

}

Go & Go::draw(){
    return *this;
}

Go & Go::update(){
    return *this;
}

int Go::next(){
    return STEP_GO;
}

