#ifndef _LOADING_H
#define _LOADING_H

class Loading: public Step{
public:
    Loading();
    ~Loading();
    Loading * next();
    Loading * update();
    Loading * draw();
};

#endif //_LOADING_H

