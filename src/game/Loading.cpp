#include "GLoid.h"
#include "Loading.h"

Loading::Loading(){
    type = STEP_LOADING;
}
Loading::~Loading(){}
Loading * Loading::next(){
    return this;
}
Loading * Loading::update(){
    return this;
}
Loading * Loading::draw(){
    return this;
}




