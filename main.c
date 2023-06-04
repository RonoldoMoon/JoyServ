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

  printf("usage:\n%s /dev/input/jsSOMETHING\a\n\n");
}

void Finish()  {

  close(fd);
}

int main(int argc, char *argv[])  {

  js_event_t      e;
  int             js_count, quit, i;
  int             axis_buff[6]; //quick specific hack.//FIX
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


  while(read (fd, &e, sizeof(e)))  {
  
    switch(e.type)  {
    
      case JS_EVENT_AXIS: {
	
        printf("%i, %i\n", e.number, e.value);
	break;
      }
      
      default: { break; }
    }
  }
 
  return 0;
}
