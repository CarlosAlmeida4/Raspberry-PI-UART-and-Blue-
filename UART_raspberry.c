#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main (){
  
	int fd ;
	int count ;
	unsigned int nextTime ;
	unsigned char c ='A';
	
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0){
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

	if (wiringPiSetup () == -1){
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

	while(1){
	serialPutchar(fd,c);
	printf("char launched : %s \n", c);
	
	sleep(1);
	}
	serialClose(fd);
		
	return 0;
}
