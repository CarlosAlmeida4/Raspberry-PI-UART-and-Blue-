#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define erro 69
#define shutdown 7


/*void foward(int fd){
	unsigned char c = 'W';
	serialPutchar(fd,c);
	sleep(10);
	printf("char launched : %s \n", c);
}
void left(int fd){
	unsigned char c = 'A';
	serialPutchar(fd,c);
	sleep(10);
	printf("char launched : %s \n", c);
	
}
void right(int fd){
	unsigned char c = 'D';
	serialPutchar(fd,c);
	sleep(10);
	printf("char launched : %s \n", c);

}
void reverse(int fd){
	unsigned char c = 'S';
	serialPutchar(fd,c);
	sleep(10);
	printf("char launched : %s \n", c);
}
void Diagonal_right(int fd){
	unsigned char c = 'E';
	serialPutchar(fd,c);
	sleep(10);
	printf("char launched : %s \n", c);
	
}
void Diagonal_left(int fd){
	unsigned char c = 'Q';
	serialPutchar(fd,c);
	sleep(10);
	printf("char launched : %s \n", c);
	sleep(1);
}
void Stop(int fd){
	unsigned char c = 'Z';
	serialPutchar(fd,c);
	sleep(10);
	printf("char launched : %s \n", c);
	sleep(1);
}*/

bool Input ( int fd){
    
    bool first = true;
    static int mouseFd = -1;
    static int keyboardFd = -1;
    struct input_event ev[64];
    int rd;

	unsigned char c='Z';
	
    bool ret = false;


    if(first){
	DIR *dirp;
	struct dirent *dp;
	regex_t kbd,mouse;

	char fullPath[1024];
	static char *dirName = "/dev/input/by-id";
	int result;

	if(regcomp(&kbd,"event-kbd",0)!=0){
	    printf("regcomp\n");
	    return true;
	}

	if ((dirp = opendir(dirName)) == NULL){
	    printf("opendir failed\n");
	    return true;
	}

	// Find any files that match the regex for keyboard or mouse

	do {
	    errno = 0;
	    if ((dp = readdir(dirp)) != NULL) 
	    {
			printf("readdir (%s)\n",dp->d_name);
			if(regexec (&kbd, dp->d_name, 0, NULL, 0) == 0)
			{
				printf("match for the kbd = %s\n",dp->d_name);
				sprintf(fullPath,"%s/%s",dirName,dp->d_name);
				keyboardFd = open(fullPath,O_RDONLY | O_NONBLOCK);
				printf("%s Fd = %d\n",fullPath,keyboardFd);
				printf("Getting exclusive access: ");
				result = ioctl(keyboardFd, EVIOCGRAB, 1);
				printf("%s\n", (result == 0) ? "SUCCESS" : "FAILURE");
			}
	    }
	} while (dp != NULL);

	closedir(dirp);


	regfree(&kbd);
	regfree(&mouse);

	
	first = false;
	if(keyboardFd == -1) return true;

    }

    
    // Read events from keyboard

    rd = read(keyboardFd,ev,sizeof(ev));
    if(rd > 0){
	int count,n;
	struct input_event *evp;

	count = rd / sizeof(struct input_event);
	n = 0;
	while(count--){
		
	    evp = &ev[n++];
	    
	    if(evp->type == 1){
		
		if(evp->value == 1) {
			
			//modo automatico on
		    if(evp->code == KEY_LEFTCTRL){
				printf("Left Control key pressed\n");
				//AUTO(1);
		    }
		    //modo automatico off
			if(evp->code == KEY_LEFTSHIFT){
				printf("Left Shift key pressed\n");
				//AUTO(0);
		    }
			//Front
			if(evp->code == KEY_W){
				printf("Move Front\n");
				c = 'W';
				serialPutchar(fd,c);
				//sleep(1);
				//foward(fd);
		    }
			//reverse
			if(evp->code == KEY_S){
				printf("Move Back\n");
				c = 'S';
				serialPutchar(fd,c);
				//reverse(fd);
		    }
   			//left
			if(evp->code == KEY_A){
				printf("Move Left\n");
				c = 'A';
				serialPutchar(fd,c);
				//left(fd);
		    }
			//right
			if(evp->code == KEY_D){
				printf("Move Right\n");
				c = 'D';
				serialPutchar(fd,c);
				//right(fd);
		    }
			//Diagonal esquerda
			if(evp->code==KEY_Q){
				printf("Move diagonal esquerda \n");
				c = 'Q';
				serialPutchar(fd,c);
				//Diagonal_left(fd);
			}
			//Diagonal Direita
			if(evp->code==KEY_E){
				printf("Move diagonal direita\n");
				c = 'E';
				serialPutchar(fd,c);
				//Diagonal_right(fd);
			}
			//Paradex
			if(evp->code==KEY_Z){
				printf("Paradex\n");
				c = 'Z';
				serialPutchar(fd,c);
				//Stop(fd);
			}
		}


		if((evp->code == KEY_X) && (evp->value == 1)){
		    ret = true;;


	    }
	}

    }
	}

    return(ret);

}

int main(int argc,char **argv){
    
    int loops;
	int fd;
	//wiringPiSetup();
	
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0){
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return erro;
	}

	if (wiringPiSetup () == -1){
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return erro;
	}
	
	
    while(Input(fd) == false ){
	//printf(" %d \n", loops);
	loops += 1;
    }
  
	serialClose(fd);
    printf(" %d loops \n",loops);

    return(1);
}
