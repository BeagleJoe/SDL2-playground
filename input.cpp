/*
==============================================================================

    file                 : input.cpp
    created              : May 24 2015
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
#include "input.h"

//============================================================================
int handle_events()
{
   int return_value = 0;
   SDL_Event event;
   static int unicode = 0;
   static SDL_Keymod modifier = KMOD_NONE;

   while(SDL_PollEvent(&event))
   {
      // Process events we care about, and ignore the others.
         switch(event.type)
         {
            case SDL_TEXTINPUT:
               unicode = (int)(event.text.text[0]);
               modifier = SDL_GetModState();
               printf("SDL_TEXTINPUT: %c %X\r\n",(char)unicode,modifier);
               break;

            case SDL_KEYDOWN:
               printf("SDL_KEYDOWN: %c\r\n",(char)event.key.keysym.sym);
               switch(event.key.keysym.sym)
               {
                  case SDLK_ESCAPE:
                     gGameOver = true;
                     break;
               }
               break;

            case SDL_KEYUP:
               printf("SDL_KEYUP: %c unicode = %c\r\n",(char)event.key.keysym.sym,(char)unicode);
               break;

            case SDL_QUIT:
               gGameOver = true;
               break;

            default:
               break;
         }
   }

   return return_value;
}