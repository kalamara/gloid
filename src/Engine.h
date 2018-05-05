#ifndef ENGINE_H
#define ENGINE_H
typedef struct mousecntl * mousecntl_t;
typedef struct screen * screen_t;

template<class G> class Engine{
        public:
        screen_t scr;
        mousecntl_t mouse;
        unsigned int fontSize 	= 48;
    //static SDL interface
        //audio
        static void playSound(int sound);
        //time
        Uint32 tic;
        static int now();

        static void printText(bool option,
                              text2d* text,
                              SDL_Color fg,
                              SDL_Color bg,
                              int x,
                              int y,
                              const char* buf, ...);

};
#endif // ENGINE_H
