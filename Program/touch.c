#include <stdint.h>
#include <wiringPi.h>

//160 4800

unsigned char	CHX=144; 	//通道X+的选择控制字	
unsigned char	CHY=208;	//通道Y+的选择控制字

void TP_DCLK(a){
  if (a)
    digitalWrite(SPI_CLK,HIGH);
  else
    digitalWrite(SPI_CLK,LOW);
}

void TP_CS(a){
  if (a)
    digitalWrite(SPI_CS,HIGH);
  else
    digitalWrite(SPI_CS,LOW);
}

void TP_DIN(a){
  if (a)
    digitalWrite(SPI_MOSI,HIGH);
  else
    digitalWrite(SPI_MOSI,LOW);
}

int TP_DOUT(){
  return digitalRead(SPI_MISO);
}

static void WR_CMD (unsigned char cmd) {
    unsigned char buf;
    unsigned char i;
    TP_CS(1);
    TP_DIN(0);
    TP_DCLK(0);
    TP_CS(0);
    for(i=0;i<8;i++) 
    {
        buf=(cmd>>(7-i))&0x1;
        TP_DIN(buf);
        //Delayus(5);
        TP_DCLK(1);
        //Delayus(5);
        TP_DCLK(0);
    }
}

static unsigned short RD_AD(void) {
    unsigned short buf=0,temp;
    unsigned char i;
    TP_DIN(0);
    TP_DCLK(1);
    for(i=0;i<12;i++) 
    {
        TP_DCLK(0);          
        temp= TP_DOUT() ? 1:0;
        buf|=(temp<<(11-i));
        //Delayus(5);
        TP_DCLK(1);
    }
    TP_CS(1);
    buf&=0x0fff;
    return(buf);
}

int GUI_TOUCH_X_MeasureX(void) { 
   int i[7],temp;
  uint8_t cou=0,k=0;
   
  while(cou<7)  //循环读数7次
  { 
    WR_CMD(CHX);      
    i[cou]=RD_AD();
    cou++;  
  }
        //将数据升序排列
  for(k=0;k<6;k++)
  {   
    for(cou=0;cou<6-k;cou++)
    {
      if(i[cou]>i[cou+1])
      {
        temp=i[cou+1];
        i[cou+1]=i[cou];
        i[cou]=temp;
      }  
    }
  }
  if(i[4]-i[2]>80)return 0;                 
    else return i[3];    
}

int GUI_TOUCH_X_MeasureY(void) { 
  int i[7],temp;
  uint8_t cou=0,k=0;
  while(cou<7)  //循环读数7次
  { 
    WR_CMD(CHY);      
    i[cou]=RD_AD();
    cou++;  
  }
        //将数据升序排列
  for(k=0;k<6;k++)
  {   
    for(cou=0;cou<6-k;cou++)
    {
      if(i[cou]>i[cou+1])
      {
        temp=i[cou+1];
        i[cou+1]=i[cou];
        i[cou]=temp;
      }  
    }
  }                 
    if(i[3]-i[0]>80||i[6]-i[3]>80)return 0;                 
    else return i[3];   
}

int touch_getX() {
    return GUI_TOUCH_X_MeasureX();
}

int touch_getY() {
    return GUI_TOUCH_X_MeasureY();
}

void touch_init(){
  pinMode(SPI_CLK,OUTPUT);
  pinMode(SPI_CS,OUTPUT);
  pinMode(SPI_MOSI,OUTPUT);
  pinMode(SPI_MISO,INPUT);
}
