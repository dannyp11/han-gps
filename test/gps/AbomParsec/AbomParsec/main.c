#include "debug.h"
#include "gpsParser.h"

const char test1[] = "$GPGGA,064951.000,2307.1256,N,12016.4438,E,1,8,0.95,39.9,M,17.8,M,,*65\r\n";

int main() {
  int i;
  for (i = 0; i < sizeof(test1); ++i) {
    gpsStepParser(test1[i]);
  }
  printf("Latitude Degree: %d, Latitude Minute x 1000: %d\r\n", getGPSLatitudeDeg(), getGPSLatitudeMin());
  printf("Longitude Degree: %d, Longitude Minute x 1000: %d\r\n", getGPSLongitudeDeg(), getGPSLongitudeMin());
  getchar();
  return 0;
}