#ifndef _GO_H
#define _GO_H

class Go:
        public Step{
public:

    Go();
    ~Go();
    Go & draw();
    Go & update();
    int next();
};

#endif //_GO_H
