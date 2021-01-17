#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <string.h>
#include "weather.h"

int main() {
  // shared memory file descriptor
  int shm_fd;
  // pointer to shared memory object
  char * shm_ptr;
  // opening shared memory object with read only
  shm_fd = shm_open(SHM_OBJ, O_RDONLY, 0666);
  // memory mapping shared memory object
  shm_ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
  // unlink the shared memory object
  shm_unlink(SHM_OBJ);

  // writing shared memory object into a file
  FILE * out = fopen("./weather_data.txt", "w");
  fprintf(out, "%s", shm_ptr);
  fclose(out);
  
  // performing the third command to show filtered data
  char cmd[256];
  strcpy(cmd, CMD3);
  puts("\nCities with clear sky or clouds and degree <= 25:\n");
  system(cmd);
  
  return 0;
}