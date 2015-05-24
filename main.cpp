/*
==============================================================================

    file                 : main.cpp
    created              : May 23 2015
    copyright            : (C) 2015 Joe Thompson
    email                : beaglejoe@users.sourceforge.net
    version              : $Id:$

==============================================================================

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

==============================================================================
*/
#include "main.h"
#include <SDL.h>

/* Boolean */
#define GL_TRUE                           1
#define GL_FALSE                          0

SDL_Window* gWin = NULL;
SDL_Renderer* gRenderer = NULL;

int gWinWidth = 640;
int gWinHeight = 480;

// Forward declare functions
int do_work();
int render();


int main(int argc, char **argv)
{
   int return_value = 0;

   // Initialize SDL video subsystem (and exit if not supported).
   if (SDL_InitSubSystem(SDL_INIT_VIDEO) >= 0)
   {
      do_work();

      // Shutdown SDL video sub-system.
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      // Shudown SDL.
      SDL_Quit();
   }
   else
   {
      printf("Couldn't initialize SDL video sub-system (%s)\n", SDL_GetError());
      return_value = -1;
   }

   return return_value;
}

int do_work()
{
   int return_value = 0;

   //printf("Hello World\n");
   gWin = SDL_CreateWindow("SDL2 Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,gWinWidth,gWinHeight,SDL_WINDOW_RESIZABLE);//SDL_WINDOW_SHOWN
   if(gWin != 0)
   {
      gRenderer = SDL_CreateRenderer(gWin,-1,0);
      if(NULL != gRenderer)
      {
         return_value = render();
      }
      else
      {
         printf("SDL_CreateRenderer() failed: (%s)\n", SDL_GetError());
         return_value = -1;
      }
   }
   else
   {
      printf("SDL_CreateWindow() failed: (%s)\n", SDL_GetError());
      return_value = -1;
   }

   return return_value;
}

int render()
{
   int return_value = 0;

   SDL_SetRenderDrawColor(gRenderer,0,0,0,255);
   
   SDL_RenderClear(gRenderer);

   SDL_RenderPresent(gRenderer);

   SDL_Delay(5000);

   return return_value;
}

