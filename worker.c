#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include "weather.h"

void * request(void * inp) {
  // the final command to be called
  char cmd[STRLEN];
  // void * into char *
  char * city = inp;
  char * res = (char * ) malloc(STRLEN * sizeof(char));
  
  // construct the command
  sprintf(cmd, CMD1, city);
  strcat(cmd, CMD2);
  
  // save the command output into a variable
  FILE * fp;
  fp = popen(cmd, "r");
  // error handling
  if (fp == NULL) {
    printf("Failed to run command\n");
    exit(1);
  }
  // read command output including spaces and commas
  // until new line
  fscanf(fp, "%[^\n]s", res);
  pclose(fp);
  
  // return the pointer to res as void *
  return (void * ) res;
}

int main() {
  
  // temporary variables
  char cities[NUM_CITIES][STRLEN / 4];
  char city[STRLEN / 4];
  int fd, i = 0, ln;
  // to collect results from threads
  void * result;
  
  // open the pipe with read only
  mkfifo(CITY_FIFO, 0666); 
  fd = open(CITY_FIFO, O_RDONLY);
  // read data from the pipe
  while (read(fd, city, sizeof(city))) {
    if (i == 0)
      ln = 0;
    else
      ln = 1;
    /* remove some unknown character at the end of
	cities names except for the first city
	it is coming from using fgets and the write to or/and
	read from named pip.
	I need to debug it more; but it works with this solution */
    strncpy(cities[i], city, strlen(city) - ln);
    i++;

  }
  // close pipe
  close(fd);
  
  // create threads (backwards) to go in parallel hitting the api
  pthread_t threads[NUM_CITIES];
  while (i--> 0) {
	// create the threads and apply the request function
    if (pthread_create( & threads[i], NULL, request, (void * ) cities[i]) == -1) // passing arg pointer as void*
      error("Can't create thread\n");

  }
  
  // shared memory object stuff to share results
  // shared memory file descriptor
  int shm_fd;
  // pointer to shared memory object
  char * shm_ptr, * msg;
  // creating shared memory object
  shm_fd = shm_open(SHM_OBJ, O_CREAT | O_RDWR, 0666);
  // adjusting size of shared memory object
  ftruncate(shm_fd, SIZE);
  // memory mapping the shared memory object
  shm_ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
  // error handling
  if (shm_ptr == MAP_FAILED)
    error("Can't map\n");

  // collecting results from threads and write into shared mem obj
  while (++i < NUM_CITIES) {
    if (pthread_join(threads[i], & result) == -1)
      error("Can't join thread\n");
    // void * to char *
    msg = result;
	// print the message into the shared mem obj ptr
    sprintf(shm_ptr, "%s\n", msg);
	/* extend the ptr length with city result length and
	and the new line character */ 
    shm_ptr += strlen(msg) + 1;
  }
  return 0;
}