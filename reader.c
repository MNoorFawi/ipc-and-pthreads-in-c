#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "weather.h"

int main(int argc, char * argv[]) {

  if (argc != 2) {
    printf("Error: You need at least 1 arguemnt which is cities file ...\n");
    return 1;
  }

  // temporary variables
  char city[STRLEN / 4], cities[NUM_CITIES][STRLEN / 4];
  
  int fd, i = 0;
  // reading the cities from file and storing in cities array
  FILE * input = fopen(argv[1], "r");
  while (fgets(city, sizeof(city), input)) {
	// removing the new line character that may come from fgets
    city[strcspn(city, "\n")] = 0;
    sprintf(cities[i], "%s", city);
	// tracking how many cities we have
    ++i;
  }
  fclose(input); // closing the file
  // opening the pipe to send the cities to worker process
  // mkfifo(fifo_name, permission) 
  mkfifo(CITY_FIFO, 0666);
  // FIFO with write only 
  fd = open(CITY_FIFO, O_WRONLY);
  // writing them in backwards
  while (i--> 0) {
    write(fd, cities[i], strlen(cities[i]) + 1);
  }
  // closing FIFO
  close(fd);
  
  return 0;
}