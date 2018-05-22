#ifndef ENGINE_H
#define ENGINE_H

#include <list>
#include <utility>
#include <fstream>
#include <iostream>

#define WORKPATH "."
// Name of the log file
#define LOG_FILE  "log.txt"
//struct version minSdlVer = version(1,2,9);
#define MINSDLVER 0x010209
/*SDL_MAJOR_VERSION * 0x10000
        + SDL_MINOR_VERSION * 0x100
        + SDL_PATCHLEVEL;*/
// this is the BASE GAME ENGINE CORE.
// Screen struct
typedef struct screen{
    int W=800;
    int H=600;
    // The default bits per pixel
#ifdef WIN32
    unsigned int BPP = 32;
#else
#ifdef __MACOSX__
    unsigned int BPP = 32;
#else
    unsigned int BPP = 24;
#endif//__MACOSX__
#endif//WIN32
    SDL_Surface * S;
    screen(){}
//    screen(struct screen & other){
//        W = other.W;
//        H = other.H;
//        BPP = other.BPP;
//    }
    screen(int w, int h, int bpp, SDL_Surface * s){
        W = w;
        H = h;
        BPP = bpp;
        S = s;
    }
} * screen_t;

// Mouse struct
typedef struct mousecntl{
    int X = 0;
    int Y = 0;
    bool leftclick = false;
    mousecntl(){}
    mousecntl(int x, int y, int l){
        X = x;
        Y = y;
        leftclick = l;
    }
} * mousecntl_t;

// Application runtime data
typedef struct appstatus{
    bool looping = true;         // still looping or heve caught QUIT message?
    bool visible = true;         // Is the application visible or iconified?
    bool mouse_focus = true;     // Is the mouse cursor inside the window?
    bool keyboard_focus = true;  // Does our application have keyboard focus?
} * appstatus_t;

typedef struct text2d{
    SDL_Surface *T;
    SDL_Rect src;//obsolete
    char msg[MAXLINE];
    unsigned int timestamp;  // If timestamp is zero, the message is always on.
    unsigned int lifetime;   // If (ticks - timestamp) > lifetime, the popup dies.
} * text2d_t;

typedef struct version {
    unsigned char major = 0;
    unsigned char minor = 0;
    unsigned char patch = 0;
    version(){}
    version(unsigned int v){
        major = (v & 0xff0000) >> 16; //v / 0x10000;
        minor = (v & 0xff00) >> 8;    //(v % 0x10000)/0x100 ;
        patch = (v & 0xff);            //v % 0x100;
    }
    version(unsigned char maj,
            unsigned char min,
            unsigned char pat){
        major = maj;
        minor = min;
        patch = pat;
    }
    unsigned int value(){
        return (major)*0x10000
             + (minor)*0x100
             + (patch);
    }
    std::string toString(){
        return std::string("V")
            + std::to_string(int(major))
            + "."
            + std::to_string(int(minor))
            + "."
            + std::to_string(int(patch));
    }
} * version_t;


using pairs = std::list<std::pair<unsigned int, unsigned int>>;

template<class G> class Engine{ //base class: Engine, derived class: Game

    static const int vFlags = SDL_OPENGLBLIT | SDL_HWSURFACE | SDL_FULLSCREEN;
    static const int multisampleBuf = 1;
    static const int multisampleSamples = 4;
    static const int colorSize = 5;
    const pairs videoModes = {
        {2048, 1152},
        {1920, 1200},
        {1600, 1200},
        {1280, 1024},
        {1280, 768},
        {1280, 720},
        {1024, 768},
        {1024, 600},
        {800, 600},
        {720, 576},
        {720, 480},
        {640, 480}
    };
    static constexpr float ambient[] = {0.319, 0.319, 0.319, 1.0};
    static constexpr float diffuse[] = {0.725, 0.725, 0.725, 0.0};
    static constexpr float mat_shininess[] = {10.0};
    static constexpr float mat_specular[] = {0.8, 0.8, 0.8, 1.0};
    static constexpr float mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
    static constexpr float mat_diffuse[] = {0.4, 0.4, 0.4, 1.0};
    static constexpr float mat_emission[] = {0.2f, 0.2f, 0.2f, 0.0f};

    struct appstatus app;

    //logging
    std::ofstream logStream;
    //sdl
    struct sdlVer;
    //mouse
    struct mousecntl mouse;
    //text
    unsigned int fontSize = 22;
    //time
    unsigned int tic;
    //video
    const SDL_VideoInfo* desktop;

    //any of those are NULL if initialization has failed
    struct screen * sdlScreen = NULL;
    TTF_Font *font = NULL;
    SDL_AudioSpec *sdlAudio = NULL;

    screen_t testVmode(unsigned x, unsigned int y);

        /*TODO: add time*/
    template<typename T, typename... Args> void error(T value, Args... args){
        log(&logStream,  "ERROR:", value,  args...);
    }
    template<typename T,typename... Args> void info(T value, Args... args){
        log(&logStream,  "INFO:",  value, args...);
    }
    template<typename T,typename... Args> void warning(T value, Args... args){
        log(&logStream,  "WARNING:",  value, args...);
    }
    static void mixer(void *udata, Uint8 *stream, int len);
public:
    Engine();
    ~Engine();

    screen_t getScreen() const ;
    mousecntl_t getMouse();

    SDL_AudioSpec * getSdlAudio() const {
        return sdlAudio;
    }
    unsigned int getFontSize() const {
        return fontSize;
    }
    TTF_Font *getFont() const{
            return font;
    }
    //initialization builders
/*{
  TODO: make this polymorphic (draw and animate can be virtual
  to interface with derived Game class
   return dynamic_cast<G*>(this);
    }*/
    G* withSdlGlVideo(struct version & v);
    G* withSdlTtf(std::string fontPath);
    G* withSdlAudio(int freq, unsigned char channels, unsigned int samples);
    //variadic log
    template <typename T> static void log(std::ostream * to, T s){
        if (to) {
#ifdef DEBUG
            *to << s << std::endl;
#endif
            std::cout << s << std::endl;
        }
    }
    template<typename T, typename... Args> static void log(std::ostream * to,
                                                           T value,
                                                           Args... args){
        if (to) {
            std::cout << value;		// use first non-format argument
#ifdef DEBUG
            *to << value;
#endif
            return log(to, args ...); 	// ``peel off'' first argument
         }
         *to << value << std::endl;
    }
    //public SDL interface
    //audio
    static void playSound(int sound);
    //time
    int now();
    //printing
    void printText(bool option,
                          text2d* text,
                          SDL_Color fg,
                          SDL_Color bg,
                          int x,
                          int y,
                          const char* buf, ...);



};
#endif // ENGINE_H
