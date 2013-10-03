#include "gloid.h"

extern int game_step;
extern GLuint TextureID[N_BMP+1];
extern SDL_Surface Texture[N_BMP+1];		

const char* Bmp_files[N_BMP] = 
{
   "arka1_hires",
   "arka2_hires",
   "arka3_hires",
   "arka4_hires",
   "gloid",
   "vaus",
   "target",
   "crosshairs",
   "warp"
};

const char* Wav_files[N_WAV] = 
{
   "alien",
   "bounce0",
   "bounce1",
   "enlarge",
   "go",
   "intro",
   "kanonaki",
   "launch",
   "lose",
   "shot",
   "warp"
};
     
// Load BMP files as OpenGL textures
BOOL LoadTextures()
{
   int i;
   char filename[MAXLINE]; 
   SDL_Surface* p;

   // Generate texture names
   glGenTextures(N_BMP+1, TextureID);

   for(i = 0; i < N_BMP;i++)
   {
      snprintf(filename, MAXLINE, "%s/textures/%s.bmp", WORKPATH, Bmp_files[i]);

      p = SDL_LoadBMP(filename);

      if(p == NULL)
         return FALSE;

      if((p->w == 0) || (p->h == 0))
         return FALSE;

      SDL_SetColorKey(p, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGBA(p->format, 0, 0, 0, 0));

      Texture[i] = *p;

#ifdef DEBUG
      Log("loaded %d X %d image %s\n", Texture[i].w,Texture[i].h, filename);

      if((Texture[i].w & (Texture[i].w - 1)) != 0) 
         Log("warning: %s's width is not a power of 2\n",filename);

      if((Texture[i].h & (Texture[i].h - 1)) != 0) 
         Log("warning: %s's height is not a power of 2\n",filename);
#endif
   }
   return TRUE;
}

// Parse ASCII export from 3DSMax and generate level
int generateLevel(int level)
{
   point3f xyz, rgb;
   char* p;
   int type = 0; 
   int i, j, k;
   int x, y, z;
   int brick_count = 0;
   int lookingfor = PARSE_BRIK;
   char smallbuffer[80];
   char buffer[MAXLINE*80];

   extern int Level_type;
   extern bricks Brick[7][7][14];
	
   char path[MAX_PATH];
   FILE* f;

   memset(path, 0, MAX_PATH);
   snprintf(path, MAX_PATH, "%s/levels/level%d.ase", WORKPATH, level + 1);

   if(!(f = fopen(path, "r")))
      Log("File %s not found!\n", path); 
#ifdef DEBUG
   else
      Log("Loading %s\n", path); 
#endif
   int lvl = level % 4; // 4 for now?

   for(i = 0; i < INTX; i++)
   {
      for(j = 0; j < INTY; j++)
      {
         for(k = 0; k < INTZ; k++)
         {
            Brick[i][j][k].init(0,0,0,0,i,j,k);
            Brick[i][j][k].active = FALSE;
         }
      }
   }

   memset(smallbuffer, '\0', 80);
	
   while(fgets(smallbuffer, 80, f))
   {
      type = 0;
      p = strstr(smallbuffer, "*");

      if(p)
      {
         switch (lookingfor)
         {
            case PARSE_BRIK:
               p[11]=0;

               if(!strcmp("*GEOMOBJECT",p))
               {
                  strcat(buffer, p);	
                  lookingfor = PARSE_POS;
               }
               break;

            case PARSE_POS:
               p[7]=0;

               if(!strcmp("*TM_POS", p))
               {
                  if(sscanf(p+8,"%f%f%f", &(xyz.x), &(xyz.y), &(xyz.z)) < 0)
                     return -2;

                  strcat(buffer, p);	
                  lookingfor = PARSE_COL;
               }
               break;

            case PARSE_COL:
               p[16]=0;

               if(!strcmp("*WIREFRAME_COLOR", p))
               {
                  if(sscanf(p+17,"%f%f%f", &(rgb.x), &(rgb.y), &(rgb.z)) < 0)
                     return -2;

                  strcat(buffer, p);	

                  if(rgb.x == 0.3f && rgb.y == 0.3f && rgb.z == 0.3f)
                     type = 1;

                  if(rgb.x == 0.4f && rgb.y == 0.4f && rgb.z == 0.0f)
                     type = 2;

                  x = (int)xyz.x / 10;
                  y = (int)xyz.y / 10;
                  z = (int)xyz.z / 10;

                  Brick[x][y][z].init(rgb.x, rgb.y, rgb.z, type, x, y, z);

                  if(type < 2)
                     brick_count++;

                  lookingfor = PARSE_BRIK;
               }
               break;	

            default:
               break;	
         }
      }
   }
   Level_type = lvl;

   fclose(f);
#ifdef DEBUG
   Log("%d bricks generated\n",brick_count);
#endif
   return brick_count;			
}

// Use SDL to load WAV files
int loadSounds()
{
   int i;
   char filename[MAXLINE];
   Uint8 *data;
   Uint32 dlen;

   extern SDL_AudioSpec Audio;
   extern SDL_AudioCVT Wave[N_WAV];

   for(i = 0; i < N_WAV; i++)
   {
      // Load the sound file and convert it to 16-bit stereo at 22kHz
      snprintf(filename, MAXLINE, "%s/sounds/%s.wav", WORKPATH, Wav_files[i]);

      if(SDL_LoadWAV(filename, &Audio, &data, &dlen) == NULL) 
      {
         Log("Couldn't load %s: %s\n", filename, SDL_GetError());
         return FALSE;
      }
#ifdef DEBUG
      Log("Loaded %s\n", filename);
#endif
      SDL_BuildAudioCVT(&(Wave[i]), Audio.format, Audio.channels, Audio.freq, AUDIO_S16, 2, 22050);

      Wave[i].buf = (Uint8*)malloc(dlen * Wave[i].len_mult);

      memcpy(Wave[i].buf, data, dlen);
      Wave[i].len = dlen;

      SDL_ConvertAudio(&Wave[i]);
      SDL_FreeWAV(data);
   }
   return TRUE; 	
}
