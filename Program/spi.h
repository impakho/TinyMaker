#include <stdint.h>
#include <wiringPi.h>

char dest[10240];

int SPI_Open(){
  if (wiringPiSPISetup(0,50000000)==-1) return -1;
  return 0;
}

void SPI_Write(uint8_t* buff,int len){
  memcpy(dest,buff,len);
  wiringPiSPIDataRW(0,dest,len);
}
