// gloid_world.cpp - class functions and other drawing functions

#include "gloid.h"

extern int Level_type;
extern int Lives;
extern int Bonus_mode;
extern SDL_Surface Texture[N_BMP+1];
extern GLuint TextureID[N_BMP+1];
double ms10 = 10.0;
extern double MinmsPF;
extern unsigned int Screen_W;
extern unsigned int Screen_H;

// Global variables
balls Ball[BALLNUM];
vauses Vaus[2];
pills Pill[INTX+1][INTY+1][INTZ+1];
bricks Brick[INTX+1][INTY+1][INTZ+1];
aliens Alien[ALIENNUM];
shots Shot[SHOTNUM];

// Display animated game objects

void particles::display(float side)
{
   glPushMatrix();
   glTranslatef(place.x, place.y, place.z);
   glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
   glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
   glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);

   glColor4f(rgb.x, rgb.y, rgb.z, life_fraction * life_fraction);

   glBegin(GL_TRIANGLE_STRIP);
   glVertex3f(0.0f, side, 0.0f);
   glVertex3f(0.0f,-side, 0.0f);
   glVertex3f(side, 0.0f, 0.0f);
   glEnd();

   glPopMatrix();
}

void particles::animate()
{
   place.x += speed.x * ms10/1000;
   place.y += speed.y * ms10/1000;
   place.z -= speed.z * ms10/1000;

   //speed.y -= GRAVITY * ms10/1000;

   rotation.x += 0.01*rotspeed.x * ms10/1000;
   rotation.y += 0.01*rotspeed.y * ms10/1000;
   rotation.z -= 0.01*rotspeed.z * ms10/1000;

   life_fraction -= ms10 / life_total;

   if(life_fraction < 0.0f)
      active = FALSE;
}

void particles::explode(point3f where, point3f* pal, int palette_size, float side)
{
   int color;
   float base, start;

   place.x = where.x;
   place.y = where.y;
   place.z = where.z;

   life_total = rand() % 1500;
   life_fraction = 1.0f;

   speed = rand3f(1000.0f);

   speed.x = 0.01f*speed.x;
   speed.y = 0.01f*speed.y;
   speed.z = 0.01f*speed.z;

   rotspeed = rand3f(36000.0f);

   // Pick a random color from the palette
   color = rand()%palette_size;
   base = pal[color].x;

   if(pal[color].y > base)
      base = pal[color].y;

   if(pal[color].z > base)
      base = pal[color].z;

   // If the base color is black, generate a random grey; otherwise,
   // apply a random shift to the palette color.
   if(base <= 0.0f)
   {
      rgb.x = rand() / RAND_MAX;
      rgb.y = rgb.x;
      rgb.z = rgb.x;
   }
   else
      start = rand() * (1.0f - 1.0f/base) / RAND_MAX + 1.0f / base;

   rgb.x = start * pal[color].x;
   rgb.y = start * pal[color].y;
   rgb.z = start * pal[color].z;
}

// Launch ball
void balls::launch()
{
   if(!launched)
   {
      launched = TRUE;

      speed.x = launchspeed.x;
      speed.y = launchspeed.y;
      speed.z = launchspeed.z;

      PlaySDLSound(WAV_LAUNCH);
   }
}

void balls::reinit(point3f sp)
{
   if(!active)
      active = TRUE;

   if(launched)
   {
      launched = FALSE;
      memset(&speed, 0, sizeof(point3f));
      memset(&nextbounce, 0, sizeof(point3f));
      launchspeed = sp;
   }
   nextspeed = launchspeed;
}

// Draw the ball
void balls::display()
{
   GLUquadricObj* base = NULL;

   if(active)
   {
      base = gluNewQuadric();

      glPushMatrix();
       glTranslatef(place.x, place.y, place.z);
       glPushMatrix();
        glColor3f(0.2f, 0.2f, 0.2f);
        gluSphere(base, rad, 12, 12); ;
       glPopMatrix();
      glPopMatrix();

      gluDeleteQuadric(base);
   }
}

// Estimate point and direction of next expected collision
// FIXME: This is more complicated than it has to be
int balls::bounce(point3f curspeed)
{
   point3f test, lim, testnext, candid[3];
   float min, next;
   point3i axis, relative;
   int dir[3], ret, i;
   unsigned int foundX, foundY, foundZ;

   // Zero speed, no bounce
   if(res3f(curspeed.x, curspeed.y, curspeed.z) == 0.0f)
      return 0;

   memset(dir, 0, 3 * sizeof(int));

   // Find direction
   if(curspeed.x > 0)
      dir[0] = 1;

   if(curspeed.x < 0)
      dir[0] = -1;

   if(curspeed.y > 0)
      dir[1] = 1;

   if(curspeed.y < 0)
      dir[1] = -1;

   if(curspeed.z > 0)
      dir[2] = 1;

   if(curspeed.z < 0)
      dir[2] = -1;

   // Find limits
   lim.x = dir[0] * SCENE_MAX;
   lim.y = dir[1] * SCENE_MAX;
   lim.z = dir[2] > 0 ? SCENE_MIN - SCENE_MAX : 0.0f;

   if(curspeed.x != 0.0f)
   {
      candid[0].x = lim.x;    
      candid[0].y = place.y + (curspeed.y * (lim.x-place.x))/curspeed.x;
      candid[0].z = place.z - (curspeed.z * (lim.x-place.x))/curspeed.x;
   }

   if(curspeed.y != 0.0f)
   {
      candid[1].y = lim.y;    
      candid[1].x = place.x + (curspeed.x * (lim.y-place.y))/curspeed.y;
      candid[1].z = place.z - (curspeed.z * (lim.y-place.y))/curspeed.y;
   }

   if(curspeed.z != 0.0f)
   {
      candid[2].z = lim.z;    
      candid[2].x = place.x - (curspeed.x * (lim.z-place.z))/curspeed.z;
      candid[2].y = place.y - (curspeed.y * (lim.z-place.z))/curspeed.z;
   }

   foundX = FALSE;
   foundY = FALSE;
   foundZ = FALSE;

   relative = brickcoords(place);

   axis = relative;
   test = place;

   // FIXME: The code for each axis is substantially the same, so it should
   //        be factored out.

   // Positive movement
   if(curspeed.z > 0)
   {
      // Find which brick it will hit
      for(i = 3; i < INTZ - 2; i++)
      {
         if(Brick[axis.X][axis.Y][i].active)
         {
            testnext.x = 0.0f;
            testnext.y = 0.0f;

            if(i < INTZ - 2)
               testnext.z = Brick[axis.X][axis.Y][i+1].place.z + 1.25f;
            else
               testnext.z = lim.z;

            testnext.x = test.x - (curspeed.x * (testnext.z-test.z))/curspeed.z;
            testnext.y = test.y - (curspeed.y * (testnext.z-test.z))/curspeed.z;

            axis = brickcoords(testnext);

            if(Brick[axis.X][axis.Y][i+1].active)
            {
               candid[2] = testnext;
               break;
            }
         }
      }
   }
   else
   {
      if(curspeed.z < 0)
      {
         for(i = axis.Z - 2; i >= 2; i--)
         {
            if(Brick[axis.X][axis.Y][i].active)
            {
               if(i > 0)
                  testnext.z = Brick[axis.X][axis.Y][i-1].place.z -1.25f;
               else
                  testnext.z = 0;

               testnext.x = test.x - (curspeed.x * (testnext.z-test.z))/curspeed.z;
               testnext.y = test.y - (curspeed.y * (testnext.z-test.z))/curspeed.z;

               axis = brickcoords(testnext);

               if(Brick[axis.X][axis.Y][i-1].active)
               {
                  candid[2] = testnext;
                  break;
               }
            }
         }
      }
   }

   // X axis
   test = place;
   axis = relative;

   // Positive movement
   if(curspeed.x > 0)
   {
      for(i = axis.X; i < INTX; i++)
      {
         if(Brick[i][axis.Y][axis.Z].active)
         {
            if(i < INTX - 1)
               testnext.x = Brick[i+1][axis.Y][axis.Z].place.x + 2.5f;
            else
               testnext.x = lim.x;

            testnext.z = test.z - (curspeed.z * (testnext.x-test.x))/curspeed.x;
            testnext.y = test.y + (curspeed.y * (testnext.x-test.x))/curspeed.x;

            axis = brickcoords(testnext);

            if(Brick[i+1][axis.Y][axis.Z].active)
            {
               candid[0] = testnext;
               break;
            }
         }
      }
   }
   else
   {
      if(curspeed.x < 0)
      {
         for(i = axis.X; i >= 0; i--)
         {
            if(Brick[i][axis.Y][axis.Z].active)
            {
               if(i > 0)
                  testnext.x = Brick[i-1][axis.Y][axis.Z].place.x - 2.5f;
               else
                  testnext.x = lim.x;

               testnext.z = test.z - (curspeed.z * (testnext.x-test.x))/curspeed.x;
               testnext.y = test.y + (curspeed.y * (testnext.x-test.x))/curspeed.x;

               axis = brickcoords(testnext);

               if(Brick[i-1][axis.Y][axis.Z].active)
               {
                  candid[0] = testnext;
                  break;
               }
            }
         }
      }
   }

   // Y axis
   test = place;
   axis = relative;

   // Positive movement
   if(curspeed.y > 0)
   {
      for(i = axis.Y; i < INTY; i++)
      {
         if(Brick[axis.X][i][axis.Z].active)
         {
            if(i < INTY - 1)
               testnext.y = Brick[axis.X][i+1][axis.Z].place.y + 2.5f;
            else
               testnext.y = lim.y;

            testnext.z = test.z - (curspeed.z * (testnext.y-test.y))/curspeed.y;
            testnext.x = test.x + (curspeed.x * (testnext.y-test.y))/curspeed.y;

            axis = brickcoords(testnext);

            if(Brick[axis.X][i+1][axis.Z].active)
            {
               candid[1] = testnext;
               break;
            }
         }
      }
   }
   else
   {
      if(curspeed.y < 0)
      {
         for(i = axis.Y; i >= 0; i--)
         {
            if(Brick[axis.Y][i][axis.Z].active)
            {
               if(i > 0)
                  testnext.y = Brick[axis.Y][i-1][axis.Z].place.y - 2.5f;
               else
                  testnext.y = lim.y;

               testnext.z = test.z - (curspeed.z * (testnext.y-test.y))/curspeed.y;
               testnext.x = test.x + (curspeed.x * (testnext.y-test.y))/curspeed.y;

               axis = brickcoords(testnext);

               if(Brick[axis.X][i-1][axis.Z].active)
               {
                  candid[1] = testnext;
                  break;
               }
            }
         }
      }
   }

   // FIXME: clean up description of algorithm, put at top of block
   //        also, is the long chain of else really necessary?

   // Z = place.z + t*speed.z
   // t = Z/speed.z - place.z
   // x = place.x + speed.x*Z/speed.z
   // y = place.y + speed.y*Z/speed.z, 
   //
   // find first brick that collides on x, y with a.x, a.y
   // if not found, z = max
   //   
   // solve per x, y, keep candidate z
   // rinse repeat for x, y, break loop when candidate further than other candidates.
   // test res3f(place - A) for all 3 candidates, keep smallest at nextbounce
   // first, what happens if 2 speeds are 0?

   // Bounce X
   if(res3f(0.0f, curspeed.y, curspeed.z) == 0.0f)
   {
      ret = 0;
   }
   else
   // Bounce Y
   if(res3f(curspeed.x, 0.0f, curspeed.z) == 0.0f)
   {
      ret = 1;
   }
   else
   // Bounce Z
   if(res3f(curspeed.x, curspeed.y, 0.0f) == 0.0f)
   {
      ret = 2;
   }
   else
   if(curspeed.x==0.0f)
   {
      //find smallest from candid[1], candid[2]
      min = res3f(candid[2].x-place.x,candid[2].y-place.y,candid[2].z-place.z);
      next = res3f(candid[1].x-place.x,candid[1].y-place.y,candid[1].z-place.z);

      if(min<next)
      {
         //return z
         ret = 2;
      }
      else
      {
         //return y
         ret = 1;
      }
   }
   else
   if(curspeed.y == 0.0f)
   {
      //find smallest from candid[0], candid[2]
      min = res3f(candid[0].x-place.x,candid[0].y-place.y,candid[0].z-place.z);
      next = res3f(candid[2].x-place.x,candid[2].y-place.y,candid[2].z-place.z);

      if(min < next)
      {
         //return x
         ret = 0;
      }
      else
      {
         //return z
         ret = 2;
      }
   }
   else
   if(curspeed.z == 0.0f)
   {
      //find smallest from candid[0], candid[1]
      min = res3f(candid[0].x-place.x,candid[0].y-place.y,candid[0].z-place.z);
      next = res3f(candid[1].x-place.x,candid[1].y-place.y,candid[1].z-place.z);

      if(min < next)
      {
         //return x
         ret = 0;
      }
      else
      {
         //return y
         ret = 1;
      }
   }
   else
   {
      //compare all 3
      min = res3f(candid[0].x-place.x,candid[0].y-place.y,candid[0].z-place.z);
      next = res3f(candid[1].x-place.x,candid[1].y-place.y,candid[1].z-place.z);
      ret = 0;

      if (next < min)
      {//return 
         min = next;
         ret  = 1;
      }
      next = res3f(candid[2].x-place.x,candid[2].y-place.y,candid[2].z-place.z);

      if (next < min)
      {//return z;
         min = next;
         ret  = 2;
      }
   }

   // change direction of axis of collision for nextspeed
   memcpy(&nextbounce, &(candid[ret]), sizeof(point3f));
   memcpy(&nextspeed, &curspeed, sizeof(point3f));

   // Return X
   if(ret == 0)
      nextspeed.x = -nextspeed.x;

   // Return Y
   if(ret == 1)
      nextspeed.y = -nextspeed.y;

   // Return Z
   if(ret == 2)
      nextspeed.z = -nextspeed.z;

   return dir[ret] * (ret+1);
}

int balls::animate(unsigned int* bricknum)
{
   float M, U, R, U2;
   point3f randspeed, bouncespeed, center;
   int found, i = 0;
   int score = 0;
   int axisX, axisY, axisZ;
   point3i rel;
   float AR;

   if(active)
   {
      // Hasn't been launched yet
      if(!launched)
      {
         U = 2 * res3f(launchspeed.x, launchspeed.y, launchspeed.z);

         setplace(Vaus[0].prevx + Vaus[0].size.x * launchspeed.x / U,
                  Vaus[0].prevy + Vaus[0].size.y * launchspeed.y / U,
                 -Vaus[0].size.z / 2 - rad);

         nextaxis = bounce(launchspeed);
      }
      else
      {
         U = res3f(nextspeed.x, nextspeed.y, nextspeed.z);

         rel = brickcoords(place);

         axisZ = rel.Z + 1;
         axisX = rel.X;
         axisY = rel.Y;

         // Bounce from brick
         if(Brick[axisX][axisY][axisZ].active)
         {
            score += Brick[axisX][axisY][axisZ].hit();
            PlaySDLSound(WAV_BOUNCE1);

            switch(nextaxis)
            {
               case 1:
                  speed.x = -speed.x;
                  place.x -= rad;
                  break;

               case -1:
                  speed.x = -speed.x;
                  place.x += rad;
                  break;

               case 2:
                  speed.y = -speed.y;
                  place.y -= rad;
                  break;

               case -2:
                  speed.y = -speed.y;
                  place.y += rad;
                  break;

               case 3:
                  speed.z = -speed.z;
                  place.z -= rad;
                  break;

               case -3:
                  speed.z = -speed.z;
                  place.z += rad;
                  break;
            }
            nextaxis = bounce(speed);
         }

         // If it hits an alien, destroy it and ricochet randomly
         for(i = 0; i < ALIENNUM; i++)
         {
            AR = res3f(Alien[i].place.x - place.x,
                       Alien[i].place.y - place.y,
                       Alien[i].place.z - place.z);

            if(Alien[i].active && (AR <= rad + Alien[i].rad))
            {
               score += SCOREALIEN;
               do//if random speed is too slow on any axis, try again
                 randspeed = rand3f(U);
               while(abs(randspeed.z) < MINSPEED || abs(randspeed.x) < MINSPEED ||abs(randspeed.y) < MINSPEED);
               nextaxis = bounce(randspeed);
               speed.x = randspeed.x;
               speed.y = randspeed.y;
               speed.z = randspeed.z;
               
               Alien[i].die();
            }
         }

         // If it hits the Vaus
         if(place.z >= Vaus[0].size.z/2 - rad)
         {
            R = rad + Vaus[0].base_rad;
            M = res3f(place.x - Vaus[0].place.x, place.y - Vaus[0].place.y, 0.0f);

            // Bounce the ball off the Vaus
            if(M < R)
            {
               // Play bounce sound
               PlaySDLSound(WAV_BOUNCE0);

               // reverse Z travel
               speed.z = -speed.z;

               // If the fire button is down, bounce spherically; otherwise, bounce flat
               if(Vaus[0].fire)
               {
                  // The magnitude of the velocity
                  U = sqrt(6) * 10.0f;

                  // Find the center of Vaus
                  center.x = Vaus[0].place.x;
                  center.y = Vaus[0].place.y;
                  center.z = Vaus[0].base_rad;

                  // Calculate the vector from the center of Vaus to the ball
                  bouncespeed = chase(center, place, U);

                  // Add the initial velocity to the bounce velocity
                  speed.x += bouncespeed.x;
                  speed.y += bouncespeed.y;
                  speed.z += bouncespeed.z;

                  // Calculate the new resultant 
                  U2 = res3f(speed.x, speed.y, speed.z);

                  // Normalize with the old one
                  speed.x = U / U2 * speed.x;
                  speed.y = U / U2 * speed.y;
                  speed.z = U / U2 * speed.z;                  
               }

               if(Bonus_mode == G)
                  reinit(speed);

               place.z = -Vaus[0].size.z/2-rad;
               nextaxis = bounce(speed);
            }
         }

         // If the ball flies off the screen, it is lost
         if(place.z > 1.0f)
         {
            active = FALSE;
            found = FALSE;

            for(i = 0; i < BALLNUM; i++)
            {
               if(Ball[i].active)
               {
                  found = TRUE;
                  break;
               }
            }

            // If there are no more active balls, lose a life
            if(!found)
            {
               Vaus[0].active = FALSE;

               for(i = 0; i < VAUS_PARTICLES; i++)
               {
                  Vaus[0].explosion[i].active = TRUE;
                  Vaus[0].explosion[i].explode(Vaus[0].place,
                     Vaus[0].pal, VAUS_COLORS, Vaus[0].particle_rad);
               }
            }
         }

         // Set the ball speed
         if(Bonus_mode == S)
         {
            place.x += speed.x * ms10/1414;
            place.y += speed.y * ms10/1414;
            place.z -= speed.z * ms10/1414;
         }
         else
         {
            place.x += speed.x * ms10/1000;
            place.y += speed.y * ms10/1000;
            place.z -= speed.z * ms10/1000;
         }

         if(place.z < SCENE_MIN - SCENE_MAX)
         {
            place.z +=rad;
            speed.z = -speed.z;
            nextaxis = bounce(speed);
         }

         if(place.x > SCENE_MAX)
         {
            place.x -= rad;
            speed.x = -speed.x;
            nextaxis = bounce(speed);
         }

         if(place.x < SCENE_MIN)
         {
            place.x += rad;
            speed.x = -speed.x;
            nextaxis = bounce(speed);
         }
         if(place.y > SCENE_MAX)
         {
            place.y -= rad;
            speed.y = -speed.y;
            nextaxis = bounce(speed);
         }

         if(place.y < SCENE_MIN)
         {
            place.y += rad;
            speed.y = -speed.y;
            nextaxis = bounce(speed);
         }
      }
   }
   return score;
}

// Display laser shots
void shots::display()
{
   GLUquadricObj * base = gluNewQuadric();

   glPushMatrix();
    glTranslatef(place.x,place.y,place.z);
    glPushMatrix();
     glScalef(1.0f, 1.0f, 4.0f);
     glColor3f(1.0f, 1.0f, 1.0f);
     gluSphere(base, 0.3f, 12, 12);
    glPopMatrix();
   glPopMatrix();
   gluDeleteQuadric(base);
}

// Animate laser shots
int shots::animate(unsigned int* brickno)
{
   int i;
   int score = 0;
   point3i rel;
   float AR;

   if(active)
   {
      // Check for collision with bricks
      rel = brickcoords(place);
      score = Brick[rel.X][rel.Y][rel.Z].hit();

      if((place.z <= SCENE_MIN - SCENE_MAX + size.z/2) || (score > 0))
         active = FALSE;

      // Check for collision with alien
      for(i = 0; i < ALIENNUM; i++)
      {
         AR = res3f(Alien[i].place.x - place.x,
                    Alien[i].place.y - place.y,
                    Alien[i].place.z - place.z);

         if(Alien[i].active && (AR <= (size.z / 2) + 1.0f))
         {
            active = FALSE;
            Alien[i].die();

            score += SCOREALIEN;
         }
      }
      place.z -= speedz * ms10/1000;
   }
   return score;
}

// Initialize brick
int bricks::init(float red, float green, float blue, int t, int X, int Y, int Z)
{
   active = TRUE;
   type = t;

   switch(type)
   {
      case 2:
         hit_counter = 0;
         r = 0.37f;
         g = 0.41f;
         b = 0.00f;
         break;

      case 1:
         hit_counter = 2;
         r = 0.31f;
         g = 0.31f;
         b = 0.31f;
         break;

      default:
         hit_counter = 1;
         r = red;
         g = green;
         b = blue;
         break;
   }

   hit_effect = 0;
   setsize(3.3f, 3.3f, 1.6f);
   setplace(5*X + SCENE_MIN + 2.5f, 5*Y + SCENE_MIN + 2.5f, - 2.5*Z - 1.25f);
   return 1;
}

// Brick was hit by ball or shot
int bricks::hit()
{
   extern unsigned int Brick_count;
   int score = 0;
   int X, Y, Z;
   double tic;

   X = (int)((place.x - SCENE_MIN - 2.5f)/5);
   Y = (int)((place.y - SCENE_MIN - 2.5f)/5);
   Z = (int)((place.z + 1.25f)/2.5f);

   tic = SDL_GetTicks();

   if(active)
   {
      if(hit_counter != 1)
         hit_effect = tic;

      // Not a gold brick
      if(hit_counter > 0)
      {
         hit_counter--;
         if(hit_counter == 0)
         {
            score = SCOREBRICK;
            active = FALSE;
            Brick_count--;

            // Randomly determine bonus pill
            Pill[X][Y][Z].init();

            if(Pill[X][Y][Z].type > -1)
               Pill[X][Y][Z].active = TRUE;

            Pill[X][Y][Z].setplace(place.x,place.y,place.z);
         }
      }
   }
   return score;
}

// Display bricks
void bricks::display(Uint32 now)
{
   if(active)
   {
      glEnable(GL_BLEND);
      glPushMatrix();
      glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
      glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
      glScalef(size.x, size.y, size.z);

      // Normal brick
      if(type == 0 || now - hit_effect < 200)
      {
         glDisable(GL_BLEND);
         glColor3f(r, g, b);
      }
      else
      {
         hit_effect = 0;
         glColor4f(r, g, b, 0.7f);
      }
      mySolidRhombik(1.0f);
      glPopMatrix();
      glDisable(GL_BLEND);
   }
}

void aliens::display()
{
   type = Level_type;

   if(active)
   {
      GLUquadricObj * base = gluNewQuadric();
      glPushMatrix();
      glTranslatef(place.x, place.y, place.z);

      switch(type)
      {
         // Alien 1
         case 0:
            glPushMatrix();
             glColor3f(0.0f, 0.0f, 1.0f);
             glTranslatef(0.0f, 0.0f, 0.0f);
             glRotatef(roty, 0.0f, 1.0f, 0.0f); 			
             glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
             glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
             glPushMatrix();
              glTranslatef(0.0f, 0.0f, 0.0f);
              gluQuadricDrawStyle(base, GLU_FILL);
              gluCylinder(base, 1.0f, 0.0f, 2.0f, 12, 12);
              gluQuadricOrientation(base, GLU_INSIDE);
              gluDisk(base, 0, 1.0f, 12, 12);
              gluQuadricOrientation(base, GLU_OUTSIDE);
             glPopMatrix();
             glPushMatrix();
              gluDisk(base, 0, 1.0f, 12, 12);
              glTranslatef(-1.0f, 0.0f, -1.0f);
              gluQuadricDrawStyle(base, GLU_FILL);
              gluCylinder(base, 1.5f, 1.0f, 1.0f, 12, 12);
              gluQuadricOrientation(base, GLU_INSIDE);
              gluDisk(base, 0, 1.5f, 12, 12);
              gluQuadricOrientation(base, GLU_OUTSIDE);
             glPopMatrix();
            glPopMatrix();
            break;
         // Alien 2
         case 1:
            
            glPushMatrix();
             glRotatef(roty, 0.0f, 1.0f, 0.0f); 
             glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
             glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
             glTranslatef(0.0f, 0.0f, 0.0f);
	    glPushMatrix();
             glColor3f( 0.0f, 1.0f, 0.0f); //green
             mySolidTetrahedron(6.0f);
 	    glPopMatrix();
	     
             glPushMatrix();
              glTranslatef ( 0.0f, 0.0f, 0.0f );
              glColor3f( 1.0f, 0.0f, 0.0f); //red
              gluSphere(base, 2.0f, 12, 12); 
             glPopMatrix();
            glPopMatrix();
            break;

         // Alien 3
         case 2:
            glPushMatrix();
             glRotatef(roty, 0.0f, 1.0f, 0.0f); 
             glPushMatrix();
              glTranslatef(0.0f, 1.5f, 0.0f);
              glColor3f(1.0f, 0.0f, 0.0f);
              gluSphere(base, 1.0f, 12, 12); 
             glPopMatrix();
             glPushMatrix();
              glColor3f(0.0f, 1.0f, 0.0f);
              glTranslatef(0.866f, 0.0f, 0.0f); 
              gluSphere(base, 1.0f, 12, 12); 
             glPopMatrix();
             glPushMatrix();
              glColor3f(0.0f, 0.0f, 1.0f);
              glTranslatef(-0.866f, 0.0f, 0.0f);
              gluSphere(base, 1.0f, 12, 12); 
             glPopMatrix();
            glPopMatrix();
            break;

         // Alien 4
         case 3:
            if((SDL_GetTicks() % 4000) < 1000)
            {
               glPushMatrix();
                glTranslatef(0.0f, 0.0f, 0.0f);
                glColor3f(1.0f, 0.0f, 0.0f);//red
                gluSphere(base, 2.0f, 12, 12); 	
               glPopMatrix();
            }
            else
            {
               glPushMatrix();
                glTranslatef(0.0f, 0.0f, 0.0f);
                glRotatef(45, 0.0f, 1.0f, 0.0f);
                glRotatef(60, 1.0f, 0.0f, 0.0f);
                glRotatef(roty, 0.0f, 1.0f, 0.0f); 
                glColor3f(1.0f, 0.0f, 0.0f);
                mySolidCube(4.0f);
               glPopMatrix();
            }
            break;

         default:
            break;
      }
      glPopMatrix();
      gluDeleteQuadric(base);
   }
}

void aliens::animate()
{
   point3i rel;
   point3f alienspeed;
   float M, R;
   int i = 0;

   memset(&alienspeed, 0, sizeof(point3f));

   if(active)	
   {
      // If the alien flies off the screen, it dies
      if(place.z > 0.0f)
      {
         die();
         R = rad+Vaus[0].base_rad;
         M = res3f(place.x - Vaus[0].place.x,place.y - Vaus[0].place.y,0.0f);//distance resultant

         if(M < R)
         {
            Vaus[0].active = FALSE;

            for(i = 0; i < VAUS_PARTICLES; i++)
            {
               Vaus[0].explosion[i].active = TRUE;
               Vaus[0].explosion[i].explode(Vaus[0].place, Vaus[0].pal, VAUS_COLORS, Vaus[0].particle_rad);
            }
         }
      }
      else
      {
         rel = brickcoords(place);

         for(i = 0; i< BALLNUM; i++)
            if(Ball[i].active)
               break;

         if(Ball[i].active)
            alienspeed = chase(place,Ball[i].place, 
                               res3f(Ball[i].speed.x, Ball[i].speed.y, Ball[i].speed.z));

         speed.x = alienspeed.x;
         speed.y = alienspeed.y;
         speed.z = alienspeed.z;

         if(Brick[rel.X][rel.Y][rel.Z].active)
         {
            speed.x = -speed.x;
            speed.y = -speed.y;
            speed.z = -speed.z;
            place.z -= size.z;
        }

         if(place.x >= SCENE_MAX - size.x/2)
         {// a safer but more verbose way to confine aliens within the game scene.
            if(speed.x > 0.0f)
            {
                       speed.x = -speed.x;
                       place.x += speed.x * size.x/abs(speed.x);
            }
         }
         if(place.x <= SCENE_MIN + size.x/2)
         {
            if(speed.x < 0.0f)
            {
                       speed.x = -speed.x;
                       place.x += speed.x * size.x/abs(speed.x);
            }
         }
        
         if(place.y >= SCENE_MAX - size.y/2)
         {
            if(speed.y > 0.0f)
            {
                       speed.y = -speed.y;
                       place.y += speed.y * size.y/abs(speed.y);
            }
         }
         if(place.y <= SCENE_MIN + size.y/2)
         {
            if(speed.y < 0.0f)
            {
                       speed.y = -speed.y;
                       place.y += speed.y * size.y/abs(speed.y);
            }
         }

         if(place.z <= SCENE_MIN - SCENE_MAX + size.z/2)
         {
            if(speed.z < 0.0f)
            {
               speed.z = - speed.z;
               place.z += speed.z * size.z/abs(speed.z);
            }
         }	
         place.x += speed.x * ms10/1000; 
         place.y += speed.y * ms10/1000;
         place.z += 5.0f * ms10/1000;
         roty += rotspeed * ms10/1000;
      }
   }
}

// Aliens blow up when they die
void aliens::die()
{
   int j;
   active = FALSE;

   for(j = 0; j < ALIEN_PARTICLES; j++)
   {
      explosion[j].active = TRUE;			
      explosion[j].explode(place, pal, 3, particle_rad);
      PlaySDLSound(WAV_ALIEN);
   }
}

// Make Vaus larger
void vauses::enlarge()
{
   if(large == FALSE)
   {
      large = TRUE;
      PlaySDLSound(WAV_ENLARGE);
   }
}

void vauses::reset()
{
   active = TRUE;
   large = FALSE;
   armed = FALSE;
   warping = FALSE;
   place.x = 0.0f;
   place.y = 0.0f;
}

void vauses::display()
{
   int rot, dir;
   point3f paparia;
   GLUquadricObj* base = NULL;

   base = gluNewQuadric();
   gluQuadricDrawStyle(base, GLU_FILL);
   glPushMatrix();
   glTranslatef(place.x,place.y,0.0f);
   glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

   if(large)
      base_rad = 4.5f;
   else
      base_rad = 3.0f;

   paparia.x = base_rad - 0.5f;
   paparia.y = 0.625f;
   paparia.z = 0.0f;

   glPushMatrix();
    glEnable(GL_BLEND);
    glColor4f(0.3f, 0.3f, 0.3f, 0.3f);//black
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 0.25f);
    gluSphere(base, base_rad, 48, 12);
    glDisable(GL_BLEND);
   glPopMatrix();

   for(dir = 0; dir < 2; dir++)
   {
      for(rot = 0; rot < 2; rot++)
      {
         glPushMatrix();
         glColor3f(1.0f, 0.0f, 0.0f);

         if(rot)
         {
            glTranslatef(paparia.x * (1 - 2 * dir), paparia.y, 0.0f);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
         }
         else
         {
            glTranslatef(-paparia.y, paparia.x * (1 - 2 * dir), 0.0f);
            glRotatef(90.0f, 0.0f, 0.1f, 0.0f);
         }

         gluCylinder(base, 0.625f, 0.625f, 1.25f, 12, 12);
         glPopMatrix();

         glPushMatrix();
         glTranslatef((1 - dir) * base_rad * (1 - 2 * rot), dir * base_rad * (1 - 2 * rot), 0.0f);

         // If the fire button is down, light up the laser canons
         if(fire)
            glColor3f(0.5f, 0.5f, 1.0f);
         else
            glColor3f(0.0f, 0.0f, 1.0f);

         gluSphere(base, 0.3f, 12, 12);
         glPopMatrix();
      }
   }
   glPopMatrix();
   gluDeleteQuadric(base);
}

// Make Vaus follow the mouse
void vauses::animate(int mousex, int mousey) 
{
   speedx = (prevx-place.x)/MinmsPF;
   speedy = (prevy-place.y)/MinmsPF;

   prevx = place.x;
   prevy = place.y;

   setplace((mousex * (SCENE_MAX - SCENE_MIN - size.x))/Screen_W + SCENE_MIN + size.x/2,
            (-mousey * (SCENE_MAX - SCENE_MIN - size.y))/Screen_H + SCENE_MAX - size.y/2,
            0.0f);
            
   //apply motion smoothness filter to vaus motion
   //place.x -= speedx * ms10/1000; 
   //place.y -= speedy * ms10/1000;

   switch(Bonus_mode)
   {
      // If the portal is open and we touch it, warp to the next level
      case B:
         if((place.x >= SCENE_MAX - (size.x/2) -1) && (place.y <= SCENE_MIN +(size.y/2)+5.0f))
         {
            if(!warping)
            {	
               warping = true;
               PlaySDLSound(WAV_WARP);
            }
         }
         break;

      case E:
         enlarge();
         break;

      case L:
         armed = TRUE;
         break;

      default:
         break;
   }
}

void pills::init()
{
   SDL_Color b,f;
   extern const char * PillLabels;
   extern SDL_Color Palette[N_RGB_COLORS];
   extern unsigned int Font_Size;
   extern unsigned int Screen_BPP;
   type = roulette();

   if(type >= 0)
   {
      col.x = Palette[type].b/255;
      col.y = Palette[type].g/255;
      col.z = Palette[type].r/255;

      b = Palette[type];
      f = Palette[RGB_BLACK];

      label = PillLabels[type];

      surf = SDL_CreateRGBSurface(0, Font_Size, Font_Size, Screen_BPP, 
                0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

      memset(&text,0,sizeof(text2d));

      text.T = SDL_CreateRGBSurface(0, Font_Size, Font_Size, Screen_BPP, 
                  0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

      SDL_SetColorKey(text.T, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                      SDL_MapRGBA(text.T->format, 0, 0, 0, 0));
   
      printText(TRUE,&text, f, b, 0, 0, "%c", label);

      SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format,b.r, b.g, b.b, 128));
      SDL_BlitSurface(text.T, 0, surf, 0);
   }	
}

void pills::display()
{
   extern unsigned int Font_Size;

   if(active)
   {
      GLUquadricObj * base = gluNewQuadric();

      glPushMatrix();
       glEnable(GL_TEXTURE_2D);
       glColor3f(col.x, col.y, col.z);
       glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
       glTranslatef(place.x,place.y, place.z);
       gluQuadricTexture(base, GL_TRUE);	
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Font_Size,
                    Font_Size, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                    surf->pixels);
       glPushMatrix();
        glPushMatrix();
         glRotatef(rotx, -1.0f, 0.0f, 0.0f);
         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
         glMatrixMode(GL_TEXTURE);
         glPushMatrix();
          glTranslatef(len/2, 0.0f, 0.0f);
          glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
          glRotatef(180.0f, 1.0f, 0.0f, 0.0f);   
          gluCylinder(base, rad, rad, len, 12, 12);
         glPopMatrix();
         glMatrixMode(GL_MODELVIEW);	
        glPopMatrix();
        glColor3f(col.x, col.y, col.z);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
         glTranslatef(0, 0.0f, 0.0f);
         gluSphere(base, rad, 12, 12); 
        glPopMatrix();
        glPushMatrix();
         glTranslatef(len, 0.0f, 0.0f);
         gluSphere(base, rad, 12, 12); 
        glPopMatrix();
       glPopMatrix();
      glPopMatrix();
   }
}

int pills::animate()
{
   int score = 0;

   if(active)
   {	
      if((place.z >= Vaus[0].size.z/2 - rad)
         && collision(place.x - size.x/2, place.x + size.x/2,
                      place.y + rad, place.y - rad,
                      Vaus[0].place.x - Vaus[0].base_rad, 
                      Vaus[0].place.x + Vaus[0].base_rad,
                      Vaus[0].place.y + Vaus[0].base_rad, 
                      Vaus[0].place.y - Vaus[0].base_rad)) {
         score += SCOREBONUS;
         active = FALSE;
         Vaus[0].reset();
         Bonus_mode = type;

         if(type == P)
         {
            Lives++;
            PlaySDLSound(WAV_KANONAKI);
         }

         if(type == D)
            divideballs();
      }
      else
         if(place.z > 1.0f)
            active = FALSE;
   }
   place.z -= speedz * ms10/1000;
   rotx += rotspeed * ms10/1000;

   return score;
}

// Display background
void display_background()
{
   // float step = 10.0f;

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TextureID[Level_type]);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, 3, Texture[Level_type].w,
      Texture[Level_type].h, 0, GL_RGB, GL_UNSIGNED_BYTE,
      Texture[Level_type].pixels);

   // Back face
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(SCENE_MIN, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(0.0f, 4.0f);
   glVertex3f(SCENE_MIN, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 4.0f);
   glVertex3f(SCENE_MAX, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 0.0f);
   glVertex3f(SCENE_MAX, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glEnd();

   // Top face
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(SCENE_MIN, SCENE_MAX, -SCENE_AIR);
   glTexCoord2f(0.0f, 4.0f);
   glVertex3f(SCENE_MIN, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 4.0f);
   glVertex3f(SCENE_MAX, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 0.0f);
   glVertex3f(SCENE_MAX, SCENE_MAX, -SCENE_AIR);
   glEnd();

   // Bottom face
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(SCENE_MIN, SCENE_MIN, -SCENE_AIR);
   glTexCoord2f(0.0f, 4.0f);
   glVertex3f(SCENE_MIN, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 4.0f);
   glVertex3f(SCENE_MAX, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 0.0f);
   glVertex3f(SCENE_MAX, SCENE_MIN, -SCENE_AIR);
   glEnd();

   // Right face
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(SCENE_MAX, SCENE_MIN, -SCENE_AIR);
   glTexCoord2f(0.0f, 4.0f);
   glVertex3f(SCENE_MAX, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 4.0f);
   glVertex3f(SCENE_MAX, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 0.0f);
   glVertex3f(SCENE_MAX, SCENE_MAX, -SCENE_AIR);
   glEnd();

   // Left face
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(SCENE_MIN, SCENE_MIN, -SCENE_AIR);
   glTexCoord2f(0.0f, 4.0f);
   glVertex3f(SCENE_MIN, SCENE_MIN, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 4.0f);
   glVertex3f(SCENE_MIN, SCENE_MAX, SCENE_MIN - SCENE_MAX - SCENE_AIR);
   glTexCoord2f(4.0f, 0.0f);
   glVertex3f(SCENE_MIN, SCENE_MAX, -SCENE_AIR);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

// Draw GLoid logo
void ready_display(SDL_Surface * window)
{
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TextureID[BMP_LOGO]);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Texture[BMP_LOGO].w, Texture[BMP_LOGO].h,
                0, GL_RGB, GL_UNSIGNED_BYTE, Texture[BMP_LOGO].pixels);

   glBegin(GL_QUADS);
   glTexCoord2f(1.0f, 0.0f);
   glVertex3f(SCENE_MAX/3, SCENE_MAX/4, -SCENE_AIR);
   glTexCoord2f(1.0f, 1.0f);
   glVertex3f(SCENE_MAX/3, SCENE_MIN/4, -SCENE_AIR);
   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(SCENE_MIN/3, SCENE_MIN/4, -SCENE_AIR);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(SCENE_MIN/3, SCENE_MAX/4, -SCENE_AIR);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

// Display number of lives left
void display_lives(SDL_Surface * window)
{
   int i = 0;
   float len = SCENE_MAX/15;
   float hgt = SCENE_MAX/20;

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TextureID[BMP_VAUS]);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Texture[BMP_VAUS].w, Texture[BMP_VAUS].h,
                0, GL_RGB, GL_UNSIGNED_BYTE, Texture[BMP_VAUS].pixels);

   glPushMatrix();

   for(i = 0; i < 10 && Lives > i; i++)
   {
      glBegin(GL_QUADS);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(SCENE_MAX - (i+1) * len, -SCENE_MAX, -SCENE_AIR);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(SCENE_MAX - (i+1) * len, -SCENE_MAX - hgt, -SCENE_AIR);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(SCENE_MAX - i * len, -SCENE_MAX - hgt, -SCENE_AIR);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(SCENE_MAX - i * len, -SCENE_MAX, -SCENE_AIR);
      glEnd();
   }
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

// Display warp effect
void display_warp()
{
   static unsigned int flag;
   float one, zero;

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TextureID[BMP_WARP]);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Texture[BMP_WARP].w, Texture[BMP_WARP].h,
                0, GL_RGB, GL_UNSIGNED_BYTE, Texture[BMP_WARP].pixels);

   glPushMatrix();

   // Flip texture in alternating frames
   if(flag>0)
   {
      one = 1.0f;
      zero = 0.0f;
      flag = 0;
   }
   else
   {
      flag = 1;
      one = 0.0f;
      zero = 1.0f;
   }

   glBegin(GL_QUADS);
   glTexCoord2f(one, zero);
   glVertex3f(SCENE_MAX, SCENE_MIN, -(SCENE_AIR + 5.0f));

   glTexCoord2f(one, one);
   glVertex3f(SCENE_MAX, SCENE_MIN + 5.0f, -(SCENE_AIR +5.0f));

   glTexCoord2f(zero, one);
   glVertex3f(SCENE_MAX, SCENE_MIN + 5.0f, -SCENE_AIR);

   glTexCoord2f(zero, zero);
   glVertex3f(SCENE_MAX, SCENE_MIN, -SCENE_AIR);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

// Display crosshairs
void display_cross(int axis, point3f where, int type)
{
   float sizex, sizey;

   sizex = 1.0f;
   sizey = 1.0f;

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TextureID[type]);
   
   glPushMatrix();
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTranslatef(where.x,where.y,where.z);

   glTexImage2D(GL_TEXTURE_2D, 0, 3, Texture[type].w, Texture[type].h,
                0, GL_RGB, GL_UNSIGNED_BYTE, Texture[type].pixels);

   switch(axis)
   {
      case 1:
         glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
         break;

      case -1:
         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
         break;

      case 2:
         glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
         break;

      case -2:
         glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
         break;

      case -3:
         glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
         break;

      case 3:
         break;

      default:
         return;
   }

   glBegin(GL_QUADS);
   glTexCoord2f(1.0f, 0.0f);
   glVertex3f(sizex, -sizey, 0.0f);

   glTexCoord2f(1.0f, 1.0f);
   glVertex3f(sizex, sizey, 0.0f);

   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(-sizex, sizey, 0.0f);

   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(-sizex, -sizex, 0.0f);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
