#ifndef _WIN_H
#define _WIN_H

class Win:
        public Step{
public:

    Win();
    ~Win();
    Win & draw();
    Win & update();
    int next();

};
#endif //_WIN_H
