========================================================================
			     GLoid is OpenGL arkanoid

Antonis K. 2010											
========================================================================

The  purpose of  this project  is to  migrate the  gameplay and  feeling
of  the classic  arcade  game  "Arkanoid"   into  three 
dimensions,  using OpenGL.  I wanted  to be  as respectful  as possible
to the  original, so all sound recording, animation and graphic design  
has been implemented as closely as possible to the original. 
All levels were designed in a way that the concept  of the  equivalent 
original  levels of  Arkanoid remains  more or  less the  same. 
Although the size of levels looks a lot smaller, it's not. 
(each level <=  7x7x7 bricks instead of 13x13 on  the original)
*************************************************************************
GAMEPLAY

If you have played Arkanoid, you should feel at home.

The objective is simple: break all the bricks, go to next level, repeat, 
gather score points, enter the high score hall of fame, rejoice.

The ball bounces on the spaceship, the bricks and the walls according to physics, 
and randomly when it hits an alien.

Color bricks take one hit to break, silver bricks take 2 and golden don't break at all.

If you lose the ball, you die.

Every brick gives 70pts., every alien you kill gives 150 pts., 
every powerup pill you take gives 1000pts.

You gain a new life every 100000 points.

When the ball is on the spaceship, 
you can see a moving crosshair to help you aim.

When it is coming at your direction, 
you can see a moving target on the spot it will bounce.

If you press fire when you catch the ball, it bounces retrospectively 
to the distance from the center of the spaceship to the center of the target.

Otherwise it bounces just like it does on the wall.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Controls:
-> mouse moves ship.
-> left click fires.
-> arrow keys move camera.
-> R reshapes camera.
-> ESC quits. 
----------------------------------------------------------------------------------------------------------------------
Bonus pills:
-> grey P: Player life 
-> pink B: Boost to next level
-> blue E: Enlarge
-> red L: Lazer cannon
-> green G: Glue
-> magenta D: Divide balls
-> orange S: Slow

 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

This will _NOT_ work without 3d accelaration, or with non-proprietary 
(windows plug and play for example) vga drivers!

You _must_ log in as administrator to install them!

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

Finally, thanks to everyone from "NeHe.org", "gamedev.net" and 
"sourceforge.net" for fragments
of code I have been fishing here and there...