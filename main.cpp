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
#include "input.h"

/* Boolean */
#define GL_TRUE                           1
#define GL_FALSE                          0

#define PLAYGROUND_OK                     0

// Global variables
SDL_Window* gWin = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Joystick* gJoyStick = NULL;
SDL_Haptic* gHaptic = NULL;

SDL_HapticEffect gEffect[1];

int gWinWidth = 640;
int gWinHeight = 480;

bool gGameOver = false;

// Forward declare functions
int do_work();

int update_state();
int redraw();

int createHapticEffect();

void dumpSDLversions();
int dumpSDL2Joystick();
int dumpSDL2Haptic();

int setupSDL2();
int setupSDL2Joystick();
int setupSDL2Haptic();
int shutdownSDL2();

//============================================================================
int main(int argc, char **argv)
{
   int return_value = 0;

   dumpSDLversions();
   dumpSDL2Joystick();
   dumpSDL2Haptic();


   if (PLAYGROUND_OK == setupSDL2())
   {
      setupSDL2Joystick();
      setupSDL2Haptic();

      do_work();

      shutdownSDL2();
   }
   else
   {
      printf("Couldn't initialize SDL video sub-system (%s)\n", SDL_GetError());
      return_value = -1;
   }

   return return_value;
}
//============================================================================
int do_work()
{
   int return_value = 0;

   while(false == gGameOver)
   {
      handle_events();
      update_state();
      redraw();
   }

   return return_value;
}
//============================================================================
int setupSDL2Haptic()
{
   int return_value = -1;
   int hapticcount = 0;
   if (PLAYGROUND_OK == SDL_InitSubSystem(SDL_INIT_HAPTIC))
   {
      hapticcount = SDL_NumHaptics();
      printf("SDL_NumHaptics() shows: %d haptic devices\n", hapticcount);

      //gHaptic = SDL_HapticOpenFromJoystick(gJoyStick);
      if(NULL == gHaptic)
      {
         printf("SDL_HapticOpenFromJoystick(gJoyStick) failed: %s\n", SDL_GetError());
         gHaptic = SDL_HapticOpen(0);
         if(NULL == gHaptic)
         {
            printf("SDL_HapticOpen(0) failed: %s\n", SDL_GetError());
         }
         else
         {
            printf("SDL_HapticOpen(0) Opened: %s\n", SDL_HapticName(0));
         }
      }
      else
      {
         printf("SDL_HapticOpenFromJoystick(gJoyStick) Opened: %s\n", SDL_HapticName(0));
      }
      if(NULL != gHaptic)
      {
         return_value = PLAYGROUND_OK;
         if(SDL_TRUE == SDL_HapticRumbleSupported(gHaptic))
         {
            if(PLAYGROUND_OK == SDL_HapticRumbleInit(gHaptic))
            {
               SDL_HapticRumbleStop(gHaptic);
               SDL_HapticRumblePlay(gHaptic,0.5f,1000);
               SDL_Delay(1000);
            }
            else
            {
               printf("SDL_HapticRumbleInit(gHaptic) failed: %s\n", SDL_GetError());
            }
         }
         else
         {
            printf("SDL_HapticRumbleSupported(gHaptic) returned FALSE \n");
         }
         createHapticEffect();
         //if ((SDL_HapticQuery(gHaptic) & SDL_HAPTIC_CONSTANT) != 0)
         //{
         //   gEffect.type = SDL_HAPTIC_CONSTANT;
         //   gEffect.constant.direction.type = SDL_HAPTIC_POLAR;
         //   gEffect.constant.direction.dir[0] = 9000;
         //   gEffect.constant.length = 1000;
         //   gEffect.constant.level = 25000;
         //   gEffect.constant.attack_length = 0;
         //   gEffect.constant.fade_length = 1000;

         //   int effect = SDL_HapticNewEffect(gHaptic,&gEffect);
         //   if(effect >=0)
         //   {
         //      if(PLAYGROUND_OK == SDL_HapticRunEffect(gHaptic,effect,1))
         //      {
         //         SDL_Delay(2000);
         //      }
         //      else
         //      {
         //         printf("SDL_HapticRunEffect(gHaptic) failed: %s\n", SDL_GetError());
         //      }
         //   }
         //   else
         //   {
         //      printf("SDL_HapticNewEffect(gHaptic) failed: %s\n", SDL_GetError());
         //   }
         //}
         //else
         //{
         //   printf("SDL_HapticQuery(gHaptic) for SDL_HAPTIC_CONSTANT returned FALSE \n");
         //}
      }
   }
   else
   {
      printf("SDL_InitSubSystem(SDL_INIT_HAPTIC) failed: %s\n", SDL_GetError());
   }

   return return_value;
}
//============================================================================
int setupSDL2Joystick()
{
   int return_value = -1;

   int joycount = 0;
   //SDL_SetHint("SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", "1");
   if (PLAYGROUND_OK == SDL_InitSubSystem(SDL_INIT_JOYSTICK))
   {
      joycount = SDL_NumJoysticks();
      printf("SDL_NumJoysticks() shows: %d joysticks\n", joycount);
      if(joycount > 0)
      {
         gJoyStick = SDL_JoystickOpen(0);
         if(NULL != gJoyStick)
         {
            printf("SDL_JoystickOpen() Opened: %s\n", SDL_JoystickName(gJoyStick));
            return_value = PLAYGROUND_OK;
         }
         else
         {
            printf("SDL_JoystickOpen() failed: %s\n", SDL_GetError());
         }
      }
   }
   else
   {
      printf("SDL_InitSubSystem(SDL_INIT_JOYSTICK) failed: %s\n", SDL_GetError());
   }

   return return_value;
}
//============================================================================
int setupSDL2()
{
   int return_value = 0;

   if (SDL_InitSubSystem(SDL_INIT_VIDEO) >= 0)
   {
      gWin = SDL_CreateWindow("SDL2 Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,gWinWidth,gWinHeight,SDL_WINDOW_RESIZABLE);//SDL_WINDOW_SHOWN
      if(NULL != gWin)
      {
         gRenderer = SDL_CreateRenderer(gWin,-1,0);
         if(NULL != gRenderer)
         {
            // TODO - add more setup here
            return_value = 0;
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
   }
   else
   {
      printf("Couldn't initialize SDL video sub-system (%s)\n", SDL_GetError());
      return_value = -1;
   }
   return return_value;
}
//============================================================================
int shutdownSDL2()
{
   int return_value = 0;

   SDL_HapticClose(gHaptic);
   SDL_QuitSubSystem(SDL_INIT_HAPTIC);

   SDL_JoystickClose(gJoyStick);
   SDL_QuitSubSystem(SDL_INIT_JOYSTICK);

   // Shutdown the renderer
   if(NULL != gRenderer)
   {
      SDL_DestroyRenderer(gRenderer);
      gRenderer = NULL;
   }

   // Shutdown the window
   if(NULL != gWin)
   {
      SDL_DestroyWindow(gWin);
      gWin = NULL;
   }
   // Shutdown SDL video sub-system.
   SDL_QuitSubSystem(SDL_INIT_VIDEO);
   
   // Shudown SDL.
   SDL_Quit();

   return return_value;
}
//============================================================================
void dumpSDLversions()
{
   SDL_version compiled;
   SDL_version linked;

   SDL_VERSION(&compiled);
   SDL_GetVersion(&linked);
   printf("Compiled against SDL version %d.%d.%d \n",
      compiled.major, compiled.minor, compiled.patch);
   printf("Linking against SDL version %d.%d.%d.\n",
      linked.major, linked.minor, linked.patch);
}
//============================================================================
int dumpSDL2Joystick()
{
   int return_value = -1;
   if (PLAYGROUND_OK == SDL_InitSubSystem(SDL_INIT_JOYSTICK))
   {
      int joycount = SDL_NumJoysticks();
      printf("SDL_NumJoysticks() shows: %d joysticks\n", joycount);
      for(int joy = 0; joy < joycount;joy++)
      {
         printf("%2.2d SDL_JoystickNameForIndex() shows: %s \n",joy,SDL_JoystickNameForIndex(joy));
      }
      SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
      return_value = joycount;
   }
   return return_value;
}
//============================================================================
int dumpSDL2Haptic()
{
   int return_value = -1;
   if (PLAYGROUND_OK == SDL_InitSubSystem(SDL_INIT_HAPTIC))
   {
      int hapticcount = SDL_NumHaptics();
      printf("SDL_NumHaptics() shows: %d haptic devices\n", hapticcount);
      for(int hap = 0; hap < hapticcount;hap++)
      {
         printf("%2.2d SDL_HapticName() shows: %s \n",hap,SDL_HapticName(hap));
      }
      SDL_QuitSubSystem(SDL_INIT_HAPTIC);
      return_value = hapticcount;
   }
   return return_value;
}
//============================================================================
int createHapticEffect()
{
   int return_value = -1;
   if(NULL != gHaptic)
   {
      if ((SDL_HapticQuery(gHaptic) & SDL_HAPTIC_CONSTANT) != 0)
         {
            //gEffect[0] = SDL_HapticEffect();
            gEffect[0].type = SDL_HAPTIC_CONSTANT;
            gEffect[0].constant.direction.type = SDL_HAPTIC_CARTESIAN;
            gEffect[0].constant.direction.dir[0] = 1;
            gEffect[0].constant.direction.dir[1] = 0;
            gEffect[0].constant.direction.dir[2] = 0;
            gEffect[0].constant.length = 2500;
            gEffect[0].constant.level = -0x7fff;
            gEffect[0].constant.attack_length = 00;
            gEffect[0].constant.fade_length = 00;

            int effect = SDL_HapticNewEffect(gHaptic,gEffect);
            if(effect >=0)
            {
               if(PLAYGROUND_OK == SDL_HapticRunEffect(gHaptic,effect,1))
               {
                  SDL_Delay(2000);
               }
               else
               {
                  printf("SDL_HapticRunEffect(gHaptic) failed: %s\n", SDL_GetError());
               }
            }
            else
            {
               printf("SDL_HapticNewEffect(gHaptic) failed: %s\n", SDL_GetError());
            }
         }
         else
         {
            printf("SDL_HapticQuery(gHaptic) for SDL_HAPTIC_CONSTANT returned FALSE \n");
         }
   }
   return return_value;
}
//============================================================================
int redraw()
{
   int return_value = 0;

   SDL_SetRenderDrawColor(gRenderer,0,0,0,255);
   
   SDL_RenderClear(gRenderer);

   SDL_RenderPresent(gRenderer);

   return return_value;
}
//============================================================================
int update_state()
{
   int return_value = 0;
   return return_value;
}

