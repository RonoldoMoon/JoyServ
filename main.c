#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
//#include <joystick.h>
#include <stdint.h>
#include <unistd.h>

/*
  Some stuff stole from /usr/include/linux/joystick.h
  For some reason mt env couldn't find the header when I tries to include it.
  When I pointed GCC ti it -I it threw a bunch of errors.
*/
 
#define CYAN "\e[0;36m"
#define NORMAL "\e[0m"
#define TERMINATE 12
#define JSIOCGNAME(len)		_IOC(_IOC_READ, 'j', 0x13, len)
#define JS_EVENT_AXIS		0x02	/* joystick moved */

//gcc main.c

typedef struct js_event_s {
        uint32_t time;     /* event timestamp in milliseconds */
        uint16_t value;    /* value */
        uint8_t  type;      /* event type */
        uint8_t  number;    /* axis/button number */
} js_event_t;


int fd;

void usage(char* p)  {

  printf("usage:\n%s /dev/input/jsSOMETHING\a\n\n", p);
}

void Finish()  {

  close(fd);
}

int main(int argc, char *argv[])  {

  js_event_t      e;
  int             js_count, quit, i;
  int             LeftAxis, RightAxis;  //later up and down.
  int             LeftValue = 0;
  int             RightValue = 0;
  char            JoyNameBuff[128];
  
  if (argc < 2)  {
  
    usage(argv[0]);
    exit(1);
  }
  
  atexit(Finish);
    
  
  if( (fd = open(argv[1], O_RDONLY)) == -1 )  {
  
      perror("open() - joystick device");
      exit(1);
  }


  if (ioctl(fd, JSIOCGNAME(sizeof(JoyNameBuff)), JoyNameBuff) < 0)
  
    strncpy(JoyNameBuff, "Unknown", sizeof(JoyNameBuff));

  printf(CYAN);
  printf("%s\n", JoyNameBuff);
  printf(NORMAL);
  
  printf("\nPlay with all the different axis for a 100 cycles\n");


  for ( i = 0; i < 100; i++, read(fd, &e, sizeof(e)) )  {
  
    switch(e.type)  {
    
      case JS_EVENT_AXIS: {
	
        printf("%i, %i\n", e.number, e.value);
	break;
      }
      
      default: { break; }
    }
  }
  
  printf("\nWhich axis do you want for the left: ");
  scanf("%d", &LeftAxis);
  printf("\nWhich axis do you want for the right: ");
  scanf("%d", &RightAxis);
  
  printf("\nSuper!...\n\n");
  
  for ( i = 0; i < 3; i++, read(fd, &e, sizeof(e))); //Read some garbage bytes.
  
  while ( read(fd, &e, sizeof(e)) )  {
  
    switch(e.type)  {
    
      case JS_EVENT_AXIS: {
	
	if ( e.number == RightAxis )  {
	
          //printf("%i, %i\n", e.number, e.value);
	  RightValue = e.value;
	  printf("%i, %i\n", LeftValue, RightValue);  //OUTPUT
	}
	
	else if ( e.number == LeftAxis )  {
	
	  LeftValue = e.value;
	  printf("%i, %i\n", LeftValue, RightValue);  //OUTPUT
	}
	
	break;
      }
      
      default: { break; }
    }
  } 
  
  
  return 0;
}
