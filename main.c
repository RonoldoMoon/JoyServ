#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>

#define TERMINATE 12

//gcc -lSDL main.c

int main()  {

  SDL_Event     e;
  SDL_Joystick* js;
  int           js_count, quit, i;
  int           axis_buff[6]; //quick specific hack.
  
  if (SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO) != 0)  {
  
    printf("Error: %s\n", SDL_GetError());
    exit(1);
  }
  
  atexit(SDL_Quit);
  
  js_count = SDL_NumJoysticks();
  
  printf("\nFound %i joysticks\n\n", js_count);
  
  for ( i = 0; i < js_count; i++ )  {
  
    printf("JOYSTICK #%i\n", i);
    js = SDL_JoystickOpen(i);
    
    if ( js == NULL )
      printf("Unable to open joystick #%i..\n", i);
      
    else  {
    
      printf("\e[0;36m%s\e[0m\n", SDL_JoystickName(i));
      printf("Axes: %i\tButtons: %i\tTrackballs: %i\n",
             SDL_JoystickNumAxes(js),
	     SDL_JoystickNumButtons(js),
	     SDL_JoystickNumBalls(js));
	     
      SDL_JoystickClose(js);
    }
  }
    
  printf("\nWhich joystick would you like to open: ");
  scanf("%d", &i);
  
  if (SDL_SetVideoMode(256, 256, 16, 0) == NULL)  {
  
    puts("video error");
    exit(1);
  }
    
  printf("opening joystick #%i\n", i);
  printf("\e[0;36m%s\e[0m...\n", SDL_JoystickName(i));
  js = SDL_JoystickOpen(i);
  
  if ( js == NULL )  {
  
      printf("Unable to open joystick #%i..\n", i);
      exit(1);
  }
  
  while(SDL_WaitEvent(&e))  {
  
    switch(e.type)  {
    
      case SDL_JOYAXISMOTION: {
      
        if ( axis_buff[(e.jaxis.axis-1)] != e.jaxis.value )  {
	
          printf("%i, %i\n", e.jaxis.axis, e.jaxis.value);
	  axis_buff[(e.jaxis.axis-1)] = e.jaxis.value;
	}
	
	break;
      }
      
      case SDL_JOYBUTTONUP:   { break; }
      
      case SDL_JOYBUTTONDOWN: { break; }
      
      case TERMINATE:         { exit(0); }
      
      default: { break; }
    }
  }
  
  return 0;
}
