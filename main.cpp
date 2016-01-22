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
SDL_GLContext gContext = NULL;

SDL_Joystick* gJoyStick[16] = {NULL};
SDL_Haptic* gHaptic[16] = {NULL};

SDL_HapticEffect gEffect[1];

int gWinWidth = 640;
int gWinHeight = 480;

SDL_version linked;

int defMajorOGL = 0;
int defMinorOGL = 0;

int msBuffers = 0;
int msSamples = 0;

int joycount = 0;

bool gGameOver = false;

// Forward declare functions
int do_work();

int update_state();
int redraw();

int createHapticEffect();

void dumpSDLversions();
int dumpSDL2Joystick();
int dumpSDL2Haptic();
void dumpOpenGLversions();
void dumpVideoDrivers();

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
void dumpOpenGLmultisample()
{
    int repBuffers = 1;
    int repSamples = 32;
    msBuffers = 1;
    msSamples = 32;
    SDL_Window* window = NULL;
    SDL_GLContext context = NULL;
    int Major = 0;
    int Minor = 0;

    while(msSamples > 1)
    {
        int err = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, msBuffers);
        err = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msSamples);

        printf("Trying  multisample buffers = %d     multisample samples = %d\n", msBuffers, msSamples);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, defMajorOGL);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, defMinorOGL);

        //err = SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        window = SDL_CreateWindow("SDL2 Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,gWinWidth,gWinHeight,SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
        if(NULL != window)
        {
            gRenderer = SDL_CreateRenderer(window,-1,0);
            if(NULL != gRenderer)
            {
                context = SDL_GL_CreateContext(window);
                if(context)
                {
                    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &Major);
                    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &Minor);
                    printf("Using OpenGL version: %d.%d \n", Major,Minor);
            
                    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &repBuffers);
                    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &repSamples);
                    printf("OpenGL reports multisample buffers: %d     multisample samples: %d\n\n", repBuffers, repSamples);

                    SDL_GL_DeleteContext(context);
                    context = NULL;
                }
                else
                {
                    printf("SDL_GL_CreateContext() FAILED \n");
                }
                SDL_DestroyRenderer(gRenderer);
                gRenderer = NULL;
            }
            else
            {
                printf("SDL_CreateRenderer() FAILED \n");
            }

            SDL_DestroyWindow(window);
            window = NULL;
        }
        else
        {
            printf("SDL_CreateWindow() FAILED \n");
        }
        msSamples /= 2;
    }
}
//============================================================================
void dumpOpenGLversions()
{

    SDL_GLContext context = NULL;
    int Major = 5;
    int Minor = 5;
    SDL_Window* window = NULL;
    window = SDL_CreateWindow("SDL2 Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,gWinWidth,gWinHeight,SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);

    if(NULL != window)
    {
        context = SDL_GL_CreateContext(window);
        if(context)
        {
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &defMajorOGL);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &defMinorOGL);
            printf("Default OpenGL version: %d.%d\n", defMajorOGL,defMinorOGL);
            SDL_GL_DeleteContext(context);
            context = NULL;
        }

        SDL_DestroyWindow(window);
        window = NULL;
    }

    while(Major >= 1)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Minor);

        window = SDL_CreateWindow("SDL2 Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,gWinWidth,gWinHeight,SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
        if(NULL != window)
        {
            context = SDL_GL_CreateContext(window);

            if(context)
            {
                SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &Major);
                SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &Minor);
                printf("OpenGL version: %d.%d available\n", Major,Minor);
                SDL_GL_DeleteContext(context);
                context = NULL;
            }
            SDL_DestroyWindow(window);
            window = NULL;
        }
        Minor--;
        if(Minor < 0)
        {
            Minor = 5;
            Major--;
        }
    }
}
//============================================================================
void dumpVideoDrivers()
{
    int nDrivers = SDL_GetNumVideoDrivers();

    for(int i = 0;i < nDrivers;i++)
    {
        printf("Video Driver %d: name: %s\n",i, SDL_GetVideoDriver(i));
    }
     printf("\n");

}
//============================================================================
int setupSDL2Haptic()
{
   int return_value = -1;
   int hapticcount = 0;
   if (PLAYGROUND_OK == SDL_InitSubSystem(SDL_INIT_HAPTIC))
   {
       hapticcount = SDL_NumHaptics();
       printf("\nSDL_NumHaptics() shows: %d haptic devices\n", hapticcount);
       for(int i = 0;i < joycount;i++)
       {
           gHaptic[i] = SDL_HapticOpenFromJoystick(gJoyStick[i]);
       
           if(NULL == gHaptic[i])
           {
               printf("\nSDL_HapticOpenFromJoystick(gJoyStick[i]) failed: %s\n", SDL_GetError());
               gHaptic[i] = SDL_HapticOpen(i);
               if(NULL == gHaptic[i])
               {
                   printf("SDL_HapticOpen(%d) failed: %s\n",i, SDL_GetError());
               }
               else
               {
                   printf("SDL_HapticOpen(%d) Opened: %s\n",i, SDL_HapticName(i));
               }
           }
           else
           {
               printf("\nSDL_HapticOpenFromJoystick(gJoyStick[i]) Opened: %s\n", SDL_HapticName(i));
           }
       
           if(NULL != gHaptic[i])
           {
               return_value = PLAYGROUND_OK;
               if(SDL_TRUE == SDL_HapticRumbleSupported(gHaptic[i]))
               {
                   printf("SDL_HapticRumbleSupported(gHaptic[i]) returned SDL_TRUE \n");
                   if(PLAYGROUND_OK == SDL_HapticRumbleInit(gHaptic[i]))
                   {
                       printf("SDL_HapticRumbleInit(gHaptic[i]) returned SDL_TRUE \n");

                       //SDL_HapticRumbleStop(gHaptic[i]);
                       SDL_HapticRumblePlay(gHaptic[i],1.0f,3000);
                       SDL_Delay(3000);
                   }
                   else
                   {
                       printf("SDL_HapticRumbleInit(gHaptic[i]) failed: %s\n", SDL_GetError());
                   }
               }
               else
               {
                   printf("SDL_HapticRumbleSupported(gHaptic[i]) returned SDL_FALSE \n");
               }
           }
       }
       //        createHapticEffect();
       //        //if ((SDL_HapticQuery(gHaptic[i]) & SDL_HAPTIC_CONSTANT) != 0)
       //        //{
       //        //   gEffect.type = SDL_HAPTIC_CONSTANT;
       //        //   gEffect.constant.direction.type = SDL_HAPTIC_POLAR;
       //        //   gEffect.constant.direction.dir[0] = 9000;
       //        //   gEffect.constant.length = 1000;
       //        //   gEffect.constant.level = 25000;
       //        //   gEffect.constant.attack_length = 0;
       //        //   gEffect.constant.fade_length = 1000;

       //        //   int effect = SDL_HapticNewEffect(gHaptic[i],&gEffect);
       //        //   if(effect >=0)
       //        //   {
       //        //      if(PLAYGROUND_OK == SDL_HapticRunEffect(gHaptic[i],effect,1))
       //        //      {
       //        //         SDL_Delay(2000);
       //        //      }
       //        //      else
       //        //      {
       //        //         printf("SDL_HapticRunEffect(gHaptic[i]) failed: %s\n", SDL_GetError());
       //        //      }
       //        //   }
       //        //   else
       //        //   {
       //        //      printf("SDL_HapticNewEffect(gHaptic[i]) failed: %s\n", SDL_GetError());
       //        //   }
       //        //}
       //        //else
       //        //{
       //        //   printf("SDL_HapticQuery(gHaptic[i]) for SDL_HAPTIC_CONSTANT returned FALSE \n");
       //        //}
       //    }
       //}
   }
   else
   {
      printf("\nSDL_InitSubSystem(SDL_INIT_HAPTIC) failed: %s\n", SDL_GetError());
   }

   return return_value;
}
//============================================================================
int setupSDL2Joystick()
{
   int return_value = -1;

   //SDL_SetHint("SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", "1");
   if (PLAYGROUND_OK == SDL_InitSubSystem(SDL_INIT_JOYSTICK|SDL_INIT_HAPTIC|SDL_INIT_GAMECONTROLLER))
   {
      joycount = SDL_NumJoysticks();
      printf("\nSDL_NumJoysticks() shows: %d joysticks\n", joycount);
      for(int i = 0;i < joycount;i++)
      {
         gJoyStick[i] = SDL_JoystickOpen(i);
         if(NULL != gJoyStick[i])
         {
            printf("\nSDL_JoystickOpen() Opened: %s\n", SDL_JoystickName(gJoyStick[i]));
            return_value = PLAYGROUND_OK;
            int ret = SDL_JoystickIsHaptic(gJoyStick[i]);
            if(ret == SDL_TRUE)
            {
                printf("SDL_JoystickIsHaptic() returned true\n");
            }
            if(ret == SDL_FALSE)
            {
                printf("SDL_JoystickIsHaptic() returned false\n");
            }
            if(ret < 0)
            {
                printf("SDL_JoystickIsHaptic() failed: %s\n", SDL_GetError());
            }
            if(SDL_IsGameController(i))
            {
                printf("SDL_IsGameController() returned true\n");
            }
            else
            {
                printf("SDL_IsGameController() returned false\n");
            }
         }
         else
         {
            printf("\nSDL_JoystickOpen() failed: %s\n", SDL_GetError());
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
      bool bValue = false;
      dumpVideoDrivers();
      //dumpOpenGLversions();
      //dumpOpenGLmultisample();
      SDL_GL_ResetAttributes();
      //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, defMajorOGL);
      //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, defMinorOGL);

      gWin = SDL_CreateWindow("SDL2 Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,gWinWidth,gWinHeight,SDL_WINDOW_OPENGL);//SDL_WINDOW_SHOWN
      if(NULL != gWin)
      {
         bValue = SDL_GL_ExtensionSupported("GL_ARB_multitexture");
         //dumpOpenGLversions();
         gRenderer = SDL_CreateRenderer(gWin,-1,0);
         if(NULL != gRenderer)
         {
             //bValue = SDL_GL_ExtensionSupported("GL_ARB_multitexture");

             int Major = -1;
             int Minor = -1;
             int Red = -1;
             int Green = -1;
             int Blue = -1;
             int Depth = -1;
             int Alpha = -1;
             // TODO - add more setup here
             //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, defMajorOGL);
             //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, defMinorOGL);
             gContext = SDL_GL_CreateContext(gWin);
             SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &Major);
             SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &Minor);
             printf("OpenGL version: %d.%d\n", Major,Minor);

             bValue = SDL_GL_ExtensionSupported("GL_ARB_multitexture");
             bValue = SDL_GL_ExtensionSupported("GL_ARB_multisample");
             bValue = SDL_GL_ExtensionSupported("GL_ARB_texture_compression");
             bValue = SDL_GL_ExtensionSupported("GL_ARB_texture_rectangle");
             bValue = SDL_GL_ExtensionSupported("GL_ARB_texture_non_power_of_two");
             bValue = SDL_GL_ExtensionSupported("GL_ARB_texture_cube_map");
             bValue = SDL_GL_ExtensionSupported("GL_ARB_texture_env_combine");
             bValue = SDL_GL_ExtensionSupported("GL_ARB_texture_env_dot3");
             bValue = SDL_GL_ExtensionSupported("ARB_imaging");
             bValue = SDL_GL_ExtensionSupported("GL_EXT_texture_filter_anisotropic");

             SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &Red);
             SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &Green);
             SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &Blue);
             SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &Depth);
             SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &Alpha);

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

   for(int i = 0;i < 16;i++)
   {
       SDL_HapticClose(gHaptic[i]);
   }
   SDL_QuitSubSystem(SDL_INIT_HAPTIC);

   for(int i = 0;i < 16;i++)
   {
       SDL_JoystickClose(gJoyStick[i]);
   }
   SDL_QuitSubSystem(SDL_INIT_JOYSTICK);

   // Shutdown the renderer
   if(NULL != gRenderer)
   {
      SDL_DestroyRenderer(gRenderer);
      gRenderer = NULL;
   }
   if(NULL != gContext)
   {
       SDL_GL_DeleteContext(gContext);
       gContext = NULL;
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
   //SDL_version linked;

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
      // Due to bug in 2.0.3, cannot Quit SDL_INIT_HAPTIC, then later re-init.
      // This is fixed in the upcoming 2.0.4
      if((linked.major >= 2) && (linked.patch >= 4))
      {
          SDL_QuitSubSystem(SDL_INIT_HAPTIC);
      }

      return_value = hapticcount;
   }
   return return_value;
}
//============================================================================
int createHapticEffect()
{
   int return_value = -1;
   //if(NULL != gHaptic)
   //{
   //   if ((SDL_HapticQuery(gHaptic) & SDL_HAPTIC_CONSTANT) != 0)
   //      {
   //         //gEffect[0] = SDL_HapticEffect();
   //         gEffect[0].type = SDL_HAPTIC_CONSTANT;
   //         gEffect[0].constant.direction.type = SDL_HAPTIC_CARTESIAN;
   //         gEffect[0].constant.direction.dir[0] = 1;
   //         gEffect[0].constant.direction.dir[1] = 0;
   //         gEffect[0].constant.direction.dir[2] = 0;
   //         gEffect[0].constant.length = 2500;
   //         gEffect[0].constant.level = -0x7fff;
   //         gEffect[0].constant.attack_length = 00;
   //         gEffect[0].constant.fade_length = 00;

   //         int effect = SDL_HapticNewEffect(gHaptic,gEffect);
   //         if(effect >=0)
   //         {
   //            if(PLAYGROUND_OK == SDL_HapticRunEffect(gHaptic,effect,1))
   //            {
   //               SDL_Delay(2000);
   //            }
   //            else
   //            {
   //               printf("SDL_HapticRunEffect(gHaptic) failed: %s\n", SDL_GetError());
   //            }
   //         }
   //         else
   //         {
   //            printf("SDL_HapticNewEffect(gHaptic) failed: %s\n", SDL_GetError());
   //         }
   //      }
   //      else
   //      {
   //         printf("SDL_HapticQuery(gHaptic) for SDL_HAPTIC_CONSTANT returned FALSE \n");
   //      }
   //}
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

