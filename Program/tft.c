#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include "spi.h"
#include "wire.h"
#include "color.h"

void WriteComm(uint8_t chr){
  digitalWrite(RS,LOW);
  SPI_Write(&chr,1);
}

void WriteData(uint8_t chr){
  digitalWrite(RS,HIGH);
  SPI_Write(&chr,1);
}

void WriteDData(uint16_t data){
  uint8_t tmp[2];
  tmp[0]=data>>8;
  tmp[1]=data&0x00ff;
 
  digitalWrite(RS,HIGH);
  SPI_Write(tmp,2);
}

void WriteNData(uint8_t* data,int N){
 digitalWrite(RS,HIGH);
 SPI_Write(data,N);
}

void delayms(int s){
 int k;
 for(k=0;k<s;k++)
      usleep(1000);
}

void LCD_HD_reset(){
  digitalWrite(RST,LOW);
  delayms(30);
  digitalWrite(RST,HIGH);
  delayms(10);
}

void GRAM_Block_Set(uint16_t Xstart,uint16_t Xend,uint16_t Ystart,uint16_t Yend){
  WriteComm(0x2a);   
  WriteData(Xstart>>8);
  WriteData(Xstart&0xff);
  WriteData(Xend>>8);
  WriteData(Xend&0xff);

  WriteComm(0x2b);   
  WriteData(Ystart>>8);
  WriteData(Ystart&0xff);
  WriteData(Yend>>8);
  WriteData(Yend&0xff);

  WriteComm(0x2c);
}


void DrawBox(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong,uint16_t Color){
  uint32_t temp;
  int i;

  GRAM_Block_Set(xStart,xStart+xLong-1,yStart,yStart+yLong-1);

  uint8_t colorbar[2048]={0};
  for (i=0;i<xLong*3;i++) colorbar[i]=ColorLib[Color][i%3];

  for (temp=0; temp<yLong; temp++) WriteNData(colorbar,xLong*3);
}

void DrawPixel(uint16_t x, uint16_t y, uint16_t Color){
  int i;
  GRAM_Block_Set(x,x,y,y);

  uint8_t colorbar[3]={0};
  for (i=0;i<3;i++) colorbar[i]=ColorLib[Color][i];

  for (i=0;i<3;i++) WriteNData(colorbar,3);
}

void LCD_Init_Reg(){
 delayms(20);
 LCD_HD_reset();

 WriteComm(0x11); // sleep out
 delayms(10); // must wait 5ms for sleep out

 WriteComm(0xc0);
 WriteComm(0xB7);   //t ver address 
 WriteData(0x07);


 WriteComm(0xF0);   //Enter ENG 
 WriteData(0x36);
 WriteData(0xA5);
 WriteData(0xD3);


 WriteComm(0xE5);   //Open gamma function 
 WriteData(0x80);

 WriteComm(0xF0);   //  Exit ENG  
 WriteData(0x36);
 WriteData(0xA5);
 WriteData(0x53);

 WriteComm(0xE0);   //Gamma setting 
 WriteData(0x00);
 WriteData(0x35);
 WriteData(0x33);
 WriteData(0x00);
 WriteData(0x00);
 WriteData(0x00);
 WriteData(0x00);
 WriteData(0x35);
 WriteData(0x33);
 WriteData(0x00);
 WriteData(0x00);
 WriteData(0x00);

 WriteComm(0x3a);   //interface pixel format  
 WriteData(0x55);      //16bits pixel 

/* --------- brightness control ----------*/

 WriteComm(0x55);WriteData(0x02); //-- adaptive brightness control Still Mode
 WriteComm(0x53);WriteData(0x2c); //--- Display control 
 WriteComm(0x5E);WriteData(0x00); //--- Min Brightness 
 WriteComm(0x51); WriteData(0x00); //-- display brightness
 printf("\nBrightness set finished!\n");


 WriteComm(0x29);   //display ON
 delayms(10);

 WriteComm(0x36); //memory data access control
 WriteData(0x08); // BGR order ????? 
  //how to turn on off  back light

 //LCD_ColorBox(0,0,320,480,0x0000);

}

int lcd_init(){
  if (SPI_Open()==0){
    printf("SPI Init OK!\n");
  }else{
    printf("SPI Error!\n");
    return 1;
  }
  pinMode(RST,OUTPUT);
  pinMode(RS,OUTPUT);



  LCD_HD_reset();
  LCD_Init_Reg();

/*------------------- Pixel Format ---- Brightness ------- BGR order ----------------*/
  WriteComm(0x3a); WriteData(0x66);  //set pixel format 18bits pixel
  WriteComm(0x36); WriteData(0x20);   // 0 RGB ???--set color order as GBR,same as BMP file 
  //set entry mode,flip mode here if necessary
  //brightness control if necessary
  return 0;
}
