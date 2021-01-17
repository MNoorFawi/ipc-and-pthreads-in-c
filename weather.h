#include <errno.h>
#include <stdlib.h>
// number of cities in data
#define NUM_CITIES 15
// length of strings
#define STRLEN 256
// how many bytes the shared memory object is
#define SIZE 2048
// name of shared object
#define SHM_OBJ "weather_data"
// named pipe (FIFO)
#define CITY_FIFO "cityfifo"
// construct first command with the city
#define CMD1 "curl -s 'http://api.openweathermap.org/data/2.5/weather?q=%s"
// append api key and extract only needed info and convert to csv
#define CMD2 "&appid=<your-api-key>' | jq -r '{name: .name, temperature: .main.temp, desc: .weather[].description} | [.name, .temperature, .desc] | @csv'"
// filter only cities which have clear sky and some clouds & the degree is below or equal to 25 celsius and add a header then pretty print them
#define CMD3 "< ./weather_data.txt grep -E 'clear sky|clouds' | awk -F, '{if($2-273.15 <= 25) {print $1\",\"$2-273.15\",\"$3}}' | sed '1i city,temperature,description' | csvlook"

// error handling
void error(char * msg) {
  printf("%s: %s\n", msg, strerror(errno));
  exit(1);
}