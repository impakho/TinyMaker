#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "init.h"

FILE* fphzk16 = NULL;
FILE* fpasc16 = NULL;

void PrintHZK16(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor,unsigned int bColor,unsigned int flag){
  unsigned char i,j,k,m;
  long offset;
  uint8_t pBuffer[32];
  while (*s){
    uint8_t data[16*16*3];
    uint8_t temp;
    offset=(94*(unsigned int)(s[0]-0xA0-1)+(s[1]-0xA0-1))*32;
    fseek(fphzk16, offset, SEEK_SET);
    fread(pBuffer, 1, 32, fphzk16);
    for (i=0;i<16;i++){
      m=pBuffer[2*i];
      for (j=0;j<8;j++){
        if ((m&0x80)==0x80){
          if (flag){
            for (temp=0;temp<3;temp++) data[(i*16+j)*3+temp]=ColorLib[fColor][temp%3];
          }else{
            if ((x+j)<=(SCREEN_W-1)&&(y+i)<=(SCREEN_H-1)) DrawPixel(x+j,y+i,fColor);
          }
        }else {
          if (flag){
            for (temp=0;temp<3;temp++) data[(i*16+j)*3+temp]=ColorLib[bColor][temp%3];
          }
        }
        m=m<<1;
      }
      m=pBuffer[2*i+1];
      for (j=0;j<8;j++){
        if ((m&0x80)==0x80){
          if (flag){
            for (temp=0;temp<3;temp++) data[(i*16+j+8)*3+temp]=ColorLib[fColor][temp%3];
          }else{
            if ((x+j+8)<=(SCREEN_W-1)&&(y+i)<=(SCREEN_H-1)) DrawPixel(x+j+8,y+i,fColor);
          }
        }else {
          if (flag){
            for (temp=0;temp<3;temp++) data[(i*16+j+8)*3+temp]=ColorLib[bColor][temp%3];
          }
        }
        m=m<<1;
      }
    }
    if (flag){
      GRAM_Block_Set(x,x+15,y,y+15);
      WriteNData(data,16*16*3);
    }
    x+=16;
    s+=2;
  }
}

void PrintASC16(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor,unsigned int bColor,unsigned int flag){
  unsigned char i,j,k,m;
  uint8_t pBuffer[16];
  long offset;

  while (*s){
    if (s[0]==0x0A) break;
    uint8_t data[8*16*3];
    uint8_t temp;
    offset=s[0]*16;
    fseek(fpasc16, offset, SEEK_SET);
    fread(pBuffer, 1, 16, fpasc16);
    for (i=0;i<16;i++){
      m=pBuffer[i];
      for (j=0;j<8;j++){
        if ((m&0x80)==0x80){
          if (flag){
            for (temp=0;temp<3;temp++) data[(i*8+j)*3+temp]=ColorLib[fColor][temp%3];
          }else{
            if ((x+j)<=(SCREEN_W-1)&&(y+i)<=(SCREEN_H-1)) DrawPixel(x+j,y+i,fColor);
          }
        }else {
          if (flag){
            for (temp=0;temp<3;temp++) data[(i*8+j)*3+temp]=ColorLib[bColor][temp%3];
          }
        }
        m=m<<1;
      }
    }
    if (flag){
      GRAM_Block_Set(x,x+7,y,y+15);
      WriteNData(data,8*16*3);
    }
    x+=8;
    s+=1;
  }
}

int text_init(){
  fphzk16 = fopen("hzk16", "rb");
  if (fphzk16 == NULL) return 1;

  fpasc16 = fopen("asc16", "rb");
  if (fpasc16 == NULL) return 2;

  return 0;
}

void text_close(){
  fclose(fphzk16);
  fphzk16 = NULL;

  fclose(fpasc16);
  fpasc16 = NULL;
}
