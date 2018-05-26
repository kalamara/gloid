#include <string>
#include <ios>
//#include <experimental/filesystem>
#include <algorithm>    // std::for_each
#include "GLoid.h"
#include "Point.h"

#include "Engine.h"
#include "World.h"
#include "Game.h"

//APP_NAME + " V" + APP_VERSION +
template<> Engine<Game>::Engine(){
//init log
    logStream.open(LOG_FILE, std::ios::out | std::ios::trunc);
        info(std::string(APP_NAME),
        version(APP_VERSION).toString(),
        " -- Log Init...");

// Initialize the SDL library
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0){
        error("Unable to open SDL: ",
              SDL_GetError());
        exit(1);
    }else{
       info("SDL ",
            version(SDL_MAJOR_VERSION,
                    SDL_MINOR_VERSION,
                    SDL_PATCHLEVEL).toString(),
            ": Initialized! ");
    }
    // Initialize timer
    tic = SDL_GetTicks();
    // Initialize rand()
    srand(tic);

////initialize OpenGL

//    // Black background
//    glClearColor(ZERO, ZERO, ZERO, ZERO);

//    // Set up depth buffer
//    glClearDepth(ONE);

//    // Set the type of depth testing
//    glDepthFunc(GL_LEQUAL);

//    //       if(Option_smooth)
//    //        glShadeModel(GL_SMOOTH);
//    //   else
//    glShadeModel(GL_FLAT);

//    // Set perspective calculations to most accurate
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//    // Set point smoothing to nicest
//    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

//    // Set the blending function
//    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

//    // Enable multisampling
//#ifndef WIN32
//    if(multisampleBuf > 0)
//        glEnable(GL_MULTISAMPLE_ARB);
//#endif
//    // Enable alpha blending
//    glEnable(GL_BLEND);

//    // Enable texture mapping
//    glEnable(GL_TEXTURE_2D);

//    // Set up the lighting parameters
//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

//    // Set up the OpenGL view
//    glViewport(0, 0, (GLsizei)(scr.W),(GLsizei)(scr.H));

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(90.0f, (GLfloat)(scr.W)/(GLfloat)(scr.H), 0.1f, 500.0f);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(ZERO, ZERO, 10.0f ,
//              ZERO, ZERO, -SCENE_MAX,
//              ZERO, ONE, ZERO);

//    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

template<> Engine<Game>::~Engine(){
    logStream.flush();
    logStream.close();
    if(sdlScreen){
        delete sdlScreen;
    }
    if(sdlAudio){
        delete sdlAudio;
    }
}

template<> screen_t Engine<Game>::getScreen() const {
    return sdlScreen;
}

template<> mousecntl_t Engine<Game>::getMouse()  {
    return &mouse;
}

template<> void Engine<Game>::printText(bool option,
                     text2d* text,
                     SDL_Color fg,
                     SDL_Color bg,
                     int x,
                     int y,
                     const char* buf, ...){
    // extern TTF_Font *DejaVuSans;
     va_list Arg;

     va_start(Arg, buf);
     vsprintf(text->msg,buf, Arg);
     va_end(Arg);

     if(option){
        text->T = TTF_RenderText_Shaded(font, text->msg, fg, bg);
     }else{
        text->T = TTF_RenderText_Blended(font, text->msg, fg);
     }
     text->src.w = text->T->w;
     text->src.h = text->T->h;
     text->src.x = x;
     text->src.y = y;
}

template<> screen_t Engine<Game>::testVmode(unsigned x, unsigned int y){
    unsigned char bpp = 32;
    SDL_Surface *s = NULL;
    for(; bpp > 8;  bpp -= 8){
        s = SDL_SetVideoMode(x, y, bpp, vFlags);
        if(s != NULL){
              break;
         }
    }
    if(s){
        return new screen(x, y, bpp, s);
    }else{
        return NULL;
    }
}

template<> void Engine<Game>::mixer(void *udata, Uint8 *stream, int len){
    std::vector<struct sbuffer> *buf = (std::vector<struct sbuffer> *)udata;
    if(buf){
        std::transform(buf->begin(),
                       buf->end(),
                       buf->begin(),
                       [stream, len](struct sbuffer s) -> struct sbuffer{
            if(s.data){
                // How much is left to play?
                int amount = (s.dlen > len) ? len : s.dlen;
                unsigned char * traf = (amount < s.dlen) ?
                                            &(s.data[amount]) :
                                            (unsigned char *)"";
                SDL_MixAudio(stream,
                             s.data,
                             amount, SDL_MIX_MAXVOLUME);

                return sbuffer(traf, s.dlen - amount);
            }
            return s; //sapio
        });/*
        std::remove_if(buf->begin(), buf->end(), [](sbuffer_t s){
            return s->data == NULL || s->dlen <= s->dpos;
        });*/
    }
}

template<> Game* Engine<Game>::withSdlGlVideo(version &v){
    if(v.value() < 0x010209){
        warning("libSDL ",
            v.toString(),
            " doesn't support SDL_GetVideoInfo(), testing video modes...");
        pairs::const_iterator it = videoModes.begin();
        while(it++ != videoModes.end() && sdlScreen == NULL){
            sdlScreen = testVmode(it->first, it->second);
        }
    }else{
        desktop = SDL_GetVideoInfo();
        sdlScreen = testVmode(desktop->current_w,desktop->current_h);
    }
    if(sdlScreen == NULL){
             error("Can't open an SDL Screen:",
                  SDL_GetError());
    }else{
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, sdlScreen->BPP);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, multisampleBuf);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisampleSamples);
        // Hide system cursor
        SDL_ShowCursor(0);
        fontSize = sdlScreen->H/16;
    }
    return static_cast<Game*>(this);
}
/*(std::string(WORKPATH)+ "/DejaVuSans.ttf").c_str()*/
template<> Game* Engine<Game>::withSdlTtf(std::string fontPath){
    if(TTF_Init() < 0){
        error("Unable to initialize SDL_ttf: ",
              TTF_GetError());
    }else{
        font = TTF_OpenFont(fontPath.c_str(),
                                  fontSize);
        if(font == NULL){
            error("Unable to initialize SDL_ttf: ",
                  TTF_GetError());
        }else{
            TTF_SetFontStyle(font, TTF_STYLE_BOLD);
        }
    }
    return static_cast<Game*>(this);
}

template<> Game* Engine<Game>::withSdlAudio(int freq,
                                            unsigned char channels,
                                            unsigned int samples){
    sdlAudio = new SDL_AudioSpec();

    sdlAudio->freq = freq;
    sdlAudio->format = AUDIO_S16;
    sdlAudio->channels = channels;
    sdlAudio->samples = samples;
    sdlAudio->callback = Engine<Game>::mixer;
    sdlAudio->userdata = NULL;

// Open the audio device
    if(SDL_OpenAudio(sdlAudio, NULL) < 0){
        error("Unable to open audio:",SDL_GetError());
        delete sdlAudio;
        sdlAudio = NULL;
    } else {// Start playing whatever is in the buffer
         SDL_PauseAudio(0);
    }
    return static_cast<Game*>(this);
}

template<> int Engine<Game>::now(){
    return SDL_GetTicks();
}
