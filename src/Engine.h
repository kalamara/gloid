#ifndef ENGINE_H
#define ENGINE_H
// this is the BASE GAME ENGINE CORE.
// Constants

#define PI 3.141525f
#define ROOT2 1.414213f
#define ROOT3 1.732051f

#define HALF_CIRCLE 180.0f

#define GRAVITY	9.81f

#define SCENE_MAX   17.5f // Up, right, in
#define	SCENE_MIN  -17.5f // Down, left, out
#define SCENE_AIR   11.7f // Camera distance

/*
#ifndef WIN32
//TODO: can we use C++17 <filesystem> here?
 #define WORKPATH "/usr/local/share/gloid"
#else
 #define WORKPATH "."
#endif
*/
#define WORKPATH "."
// Name of the log file
#define LOG_FILE  "log.txt"
//struct version minSdlVer = version(1,2,9);
#define MINSDLVER 0x010209
/*SDL_MAJOR_VERSION * 0x10000
        + SDL_MINOR_VERSION * 0x100
        + SDL_PATCHLEVEL;*/

// If we're under MSVC, tell the linker to look for these libraries.
#ifdef WIN32
 #pragma comment(lib, "OpenGL32.lib")
 #pragma comment(lib, "GLu32.lib")
 #pragma comment(lib, "SDLmain.lib")
 #pragma comment(lib, "SDL.lib")
#endif

// Includes
#ifdef WIN32
 #include <windows.h>
#else
#include <stdarg.h>
#include <ctype.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>

#ifndef __MACOSX__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif


#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <memory>
#include <optional>

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

// Sound buffer: SDL only works with double-buffering, so NUM_BUFFERS = 2
typedef struct sbuffer{
   unsigned char *data = nullptr;
   unsigned int dlen = 0;
   sbuffer(){}
   sbuffer(unsigned char * d,
           unsigned int l){
       data = d;
       dlen = l;
   }
} * sbuffer_t;

// Application runtime data
typedef struct appstatus{
    bool looping = true;         // still looping or heve caught QUIT message?
    bool visible = true;         // Is the application visible or iconified?
    bool mouse_focus = true;     // Is the mouse cursor inside the window?
    bool keyboard_focus = true;  // Does our application have keyboard focus?
} * appstatus_t;

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
using keypair = std::pair<const unsigned char, bool>;
using screenopt = std::optional<struct screen>;
using audioopt = std::optional<SDL_AudioSpec>;
using fontopt = std::optional<TTF_Font*>;

template<class G> class Engine{ //base class: Engine, derived class: Game

    static const int vFlags = SDL_OPENGLBLIT | SDL_HWSURFACE;// | SDL_FULLSCREEN;
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

    //logging
    std::ofstream logStream;
    //sdl
    struct sdlVer;
    //mouse
    struct mousecntl mouse;
    //keyboard (only the keys we are handling)
    std::map<unsigned char, bool> keys = {

        {SDLK_a,        false},
        {SDLK_b,        false},
        {SDLK_c,        false},
        {SDLK_d,        false},
        {SDLK_e,        false},
        {SDLK_f,        false},
        {SDLK_g,        false},
        {SDLK_h,        false},
        {SDLK_i,        false},
        {SDLK_j,        false},
        {SDLK_k,        false},
        {SDLK_l,        false},
        {SDLK_m,        false},
        {SDLK_n,        false},
        {SDLK_o,        false},
        {SDLK_p,        false},
        {SDLK_q,        false},
        {SDLK_r,        false},
        {SDLK_s,        false},
        {SDLK_t,        false},
        {SDLK_u,        false},
        {SDLK_v,        false},
        {SDLK_w,        false},
        {SDLK_x,        false},
        {SDLK_y,        false},
        {SDLK_z,        false},
        {SDLK_RETURN,   false},
        {SDLK_DELETE,   false},
        {SDLK_BACKSPACE,false},
        {SDLK_ESCAPE,   false},
        {SDLK_RIGHT,    false},
        {SDLK_LEFT,     false},
        {SDLK_UP,       false},
        {SDLK_DOWN,     false},
        {SDLK_KP_PLUS,  false},
        {SDLK_KP_MINUS, false},
    };
    //text
    unsigned int fontSize = 22;
    //time
    unsigned int tic;
    //video
    const SDL_VideoInfo* desktop;
    //audio
    std::vector<struct sbuffer> soundBuffers;
    std::map<std::string, SDL_AudioCVT> sounds;
    std::map<std::string, std::pair<GLuint, SDL_Surface>> textures;
    //3d scene
    class Point3f camera;      // Camera coordinates
    float phi = ZERO;
    float theta = ZERO;
    //any of those are empty if initialization has failed
    screenopt sdlScreen = {};
    fontopt sdlFont = {};
    audioopt sdlAudio = {};

    screenopt testVmode(unsigned x, unsigned int y);
    void reshape(int width, int height);

public:
    struct appstatus app;

    Engine();
    ~Engine();

    bool looping(){
        return app.looping;
    }
    bool visible(){
        return app.visible;
    }
    bool mouseFocusing(){
        return app.mouse_focus;
    }
    bool kbdFocusing(){
        return app.keyboard_focus;
    }
    bool keyPressed(unsigned char k){
        return keys[k];
    }

    screenopt getScreen() ;
    mousecntl_t getMouse();

    audioopt getSdlAudio() {
        return sdlAudio;
    }
    unsigned int getFontSize() {
        return fontSize;
    }
    fontopt getFont(){
            return sdlFont;
    }
    //initialization builders
    G& withSdlGlVideo(struct version & v);
    G& withSdlTtf(std::string fontPath);
    G& withSdlAudio(int freq, unsigned char channels, unsigned int samples);
    G& withOpenGl();
    G& handleEvent(SDL_Event & e);

    virtual G& loop()=0;
    //virtual G* start(class Step * at)=0;
    virtual class Step& nextStep()=0;

    //variadic log
    template<typename T, typename... Args> void error(T value, Args... args){
        text2d::log(&logStream,  toc(), ":ERROR:", value,  args...);
    }
    template<typename T,typename... Args> void info(T value, Args... args){
        text2d::log(&logStream,  toc(), ":INFO:",  value, args...);
    }
    template<typename T,typename... Args> void warning(T value, Args... args){
        text2d::log(&logStream,  toc(), ":WARNING:",  value, args...);
    }
    //public SDL interface
    //audio
    static void mixer(void *udata, Uint8 *stream, int len);
    G* loadBmp(const std::string & name, GLuint id);
    G* loadSound(const std::string & name);
    G* addSound(unsigned char * data,
                unsigned int dlen,
                const std::string & key);
    bool playSound(const std::string & sound);
    unsigned int pendingSounds(){

        return soundBuffers.size();
    }
    //time
    static int toc();

    //printing
    SDL_Surface * print2d(text2d & text);

    //draw a 2D SDL surface
    void draw2d(SDL_Surface *surf,
            int x,
            int y);
//helper math functions
//convention: 0 is power of two (two to the minus inf)
    static unsigned int nextpoweroftwo(unsigned int x){
       double y = pow(2, ceil(log(x) / log(2)));

       return (unsigned int)y;
    }

    static bool ispoweroftwo(unsigned int x){

        return (x & (x - 1)) == 0;
    }

};
#endif // ENGINE_H
