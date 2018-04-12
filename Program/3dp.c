#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include "tft.c"
#include "image.c"
#include "text.c"
#include "touch.c"
#include "init.h"
#include "io.c"
#include "serial.c"
#include <pthread.h>

//480   320
//180   160
//3780  3840
//7.5   11.5

int FLAG=0;
int PAGE=1;
int PAGET=1;
int PAD=1;

int sliderUnit=2;
float sliderCount=1;
char wifiname[33];
char wifipass[65];
char xspeed[8];
char yspeed[8];
char zspeed[8];
char p0temp[8];
char e0len[8];
char e0speed[8];
char printname[33];
char printpath[65];
char wifiip[20];
int wifitype=0;
int serialNum=5;
int serialCount=5;
int serialtype=0;
int tempNum=8;
int tempCount=8;
char gcode[32];
int printing=0;
pthread_t thid_print;

int P2TX(int P){return (int) P*TOUCH_X_UNIT+TOUCH_X_MIN;}
int P2TY(int P){return (int) P*TOUCH_Y_UNIT+TOUCH_Y_MIN;}
int InBox(int PX_MIN,int PY_MIN,int PX_LEN,int PY_LEN,int TX,int TY){
  if (P2TX(PX_MIN)<TX&&TX<P2TX(PX_MIN+PX_LEN))
    if (P2TY(PY_MIN+TOPBAR_H)<TY&&TY<P2TY(PY_MIN+TOPBAR_H+PY_LEN)){
      FLAG=1;
      return 1;
    }
  return 0;
}
int InBoxT(int PX_MIN,int PY_MIN,int PX_LEN,int PY_LEN,int TX,int TY){
  if (P2TX(PX_MIN)<TX&&TX<P2TX(PX_MIN+PX_LEN))
    if (P2TY(PY_MIN)<TY&&TY<P2TY(PY_MIN+PY_LEN)){
      FLAG=1;
      return 1;
    }
  return 0;
}
void PADPrint(){
  switch(PAD){
    case PAD_WIFINAME:PrintASC16(17,TOPBAR_H+31,wifiname,BLACK,WHITE,1);break;
    case PAD_WIFIPASS:PrintASC16(17,TOPBAR_H+31,wifipass,BLACK,WHITE,1);break;
    case PAD_XSPEED:PrintASC16(17,TOPBAR_H+31,xspeed,BLACK,WHITE,1);break;
    case PAD_YSPEED:PrintASC16(17,TOPBAR_H+31,yspeed,BLACK,WHITE,1);break;
    case PAD_ZSPEED:PrintASC16(17,TOPBAR_H+31,zspeed,BLACK,WHITE,1);break;
    case PAD_P0TEMP:PrintASC16(17,TOPBAR_H+31,p0temp,BLACK,WHITE,1);break;
    case PAD_E0LEN:PrintASC16(17,TOPBAR_H+31,e0len,BLACK,WHITE,1);break;
    case PAD_E0SPEED:PrintASC16(17,TOPBAR_H+31,e0speed,BLACK,WHITE,1);break;
    case PAD_PRINTNAME:PrintASC16(17,TOPBAR_H+31,printname,BLACK,WHITE,1);break;
    default:break;
  }
}
void PADPrintL(){
  char L[2];
  switch(PAD){
    case PAD_WIFINAME:sprintf(L,"%c",wifiname[strlen(wifiname)-1]);PrintASC16(17+8*(strlen(wifiname)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    case PAD_WIFIPASS:sprintf(L,"%c",wifipass[strlen(wifipass)-1]);PrintASC16(17+8*(strlen(wifipass)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    case PAD_XSPEED:sprintf(L,"%c",xspeed[strlen(xspeed)-1]);PrintASC16(17+8*(strlen(xspeed)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    case PAD_YSPEED:sprintf(L,"%c",yspeed[strlen(yspeed)-1]);PrintASC16(17+8*(strlen(yspeed)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    case PAD_ZSPEED:sprintf(L,"%c",zspeed[strlen(zspeed)-1]);PrintASC16(17+8*(strlen(zspeed)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    case PAD_P0TEMP:sprintf(L,"%c",p0temp[strlen(p0temp)-1]);PrintASC16(17+8*(strlen(p0temp)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    case PAD_E0LEN:sprintf(L,"%c",e0len[strlen(e0len)-1]);PrintASC16(17+8*(strlen(e0len)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    case PAD_E0SPEED:sprintf(L,"%c",e0speed[strlen(e0speed)-1]);PrintASC16(17+8*(strlen(e0speed)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    case PAD_PRINTNAME:sprintf(L,"%c",printname[strlen(printname)-1]);PrintASC16(17+8*(strlen(printname)-1),TOPBAR_H+31,L,BLACK,WHITE,1);break;
    default:break;
  }
}
void PADPrintE(){
  switch(PAD){
    case PAD_WIFINAME:DrawBox(17+8*strlen(wifiname),TOPBAR_H+31,8,16,WHITE);break;
    case PAD_WIFIPASS:DrawBox(17+8*strlen(wifipass),TOPBAR_H+31,8,16,WHITE);break;
    case PAD_XSPEED:DrawBox(17+8*strlen(xspeed),TOPBAR_H+31,8,16,WHITE);break;
    case PAD_YSPEED:DrawBox(17+8*strlen(yspeed),TOPBAR_H+31,8,16,WHITE);break;
    case PAD_ZSPEED:DrawBox(17+8*strlen(zspeed),TOPBAR_H+31,8,16,WHITE);break;
    case PAD_P0TEMP:DrawBox(17+8*strlen(p0temp),TOPBAR_H+31,8,16,WHITE);break;
    case PAD_E0LEN:DrawBox(17+8*strlen(e0len),TOPBAR_H+31,8,16,WHITE);break;
    case PAD_E0SPEED:DrawBox(17+8*strlen(e0speed),TOPBAR_H+31,8,16,WHITE);break;
    case PAD_PRINTNAME:DrawBox(17+8*strlen(printname),TOPBAR_H+31,8,16,WHITE);break;
    default:break;
  }
}
void Slide(int P){
  switch(P){
    case PAGE_MAIN:image_show(0,TOPBAR_H,1,"img/main.bmp");break;
    case PAGE_PRINT:image_show(0,TOPBAR_H,1,"img/print.bmp");PrintASC16(130,TOPBAR_H+122,printname,BLACK,WHITE,1);break;
    case PAGE_MOVE:image_show(0,TOPBAR_H,1,"img/move.bmp");
      switch(sliderUnit){
        case 1:image_show(2,TOPBAR_H+40,1,"img/slider1.bmp");break;
        case 2:image_show(2,TOPBAR_H+40,1,"img/slider2.bmp");break;
        case 3:image_show(2,TOPBAR_H+40,1,"img/slider3.bmp");break;
        default:break;
      }
      PrintASC16(8,TOPBAR_H+22,"0.1",BLACK,WHITE,1);PrintASC16(104,TOPBAR_H+22,"1",BLACK,WHITE,1);
      PrintASC16(180,TOPBAR_H+22,"10",BLACK,WHITE,1);PrintASC16(404,TOPBAR_H+104,xspeed,BLACK,WHITE,1);
      PrintASC16(404,TOPBAR_H+169,yspeed,BLACK,WHITE,1);PrintASC16(404,TOPBAR_H+237,zspeed,BLACK,WHITE,1);break;
    case PAGE_HEAT:image_show(0,TOPBAR_H,1,"img/heat.bmp");PrintASC16(242,TOPBAR_H+204,p0temp,BLACK,WHITE,1);break;
    case PAGE_EXT:image_show(0,TOPBAR_H,1,"img/ext.bmp");PrintASC16(157,TOPBAR_H+204,e0len,BLACK,WHITE,1);
      PrintASC16(335,TOPBAR_H+204,e0speed,BLACK,WHITE,1);break;
    case PAGE_WIFI:image_show(0,TOPBAR_H,1,"img/wifi_conn.bmp");PrintASC16(152,TOPBAR_H+59,wifiname,BLACK,WHITE,1);
      PrintASC16(152,TOPBAR_H+133,wifipass,BLACK,WHITE,1);break;
    case PAGE_NUMPAD:image_show(0,TOPBAR_H,1,"img/numpad.bmp");PADPrint();break;
    case PAGE_TOUCHPAD:image_show(0,TOPBAR_H,1,"img/touchpad.bmp");PADPrint();break;
    case PAGE_SHIFTPAD:image_show(0,TOPBAR_H,1,"img/shiftpad.bmp");PADPrint();break;
    default:break;
  }
}
int SavePAD(int P){
  switch(P){
    case PAD_WIFINAME:save_wifiname(wifiname);break;
    case PAD_WIFIPASS:save_wifipass(wifipass);break;
    case PAD_XSPEED:save_xspeed(xspeed);break;
    case PAD_YSPEED:save_yspeed(yspeed);break;
    case PAD_ZSPEED:save_zspeed(zspeed);break;
    case PAD_P0TEMP:save_p0temp(p0temp);break;
    case PAD_E0LEN:save_e0len(e0len);break;
    case PAD_E0SPEED:save_e0speed(e0speed);break;
    case PAD_PRINTNAME:save_printname(printname);break;
    default:break;
  }
}
int GetPAGE(int P){
  switch(P){
    case PAD_WIFINAME:return PAGE_WIFI;break;
    case PAD_WIFIPASS:return PAGE_WIFI;break;
    case PAD_XSPEED:return PAGE_MOVE;break;
    case PAD_YSPEED:return PAGE_MOVE;break;
    case PAD_ZSPEED:return PAGE_MOVE;break;
    case PAD_P0TEMP:return PAGE_HEAT;break;
    case PAD_E0LEN:return PAGE_EXT;break;
    case PAD_E0SPEED:return PAGE_EXT;break;
    case PAD_PRINTNAME:return PAGE_PRINT;break;
    default:break;
  }
  return PAGE_MAIN;
}
void PADInput(char *S){
  switch(PAD){
    case PAD_WIFINAME:if (strlen(wifiname)<(sizeof(wifiname)-1)){sprintf(wifiname,"%s%s",wifiname,S);PADPrintL();}break;
    case PAD_WIFIPASS:if (strlen(wifipass)<(sizeof(wifipass)-1)){sprintf(wifipass,"%s%s",wifipass,S);PADPrintL();}break;
    case PAD_XSPEED:if (strlen(xspeed)<(sizeof(xspeed)-1)){sprintf(xspeed,"%s%s",xspeed,S);PADPrintL();}break;
    case PAD_YSPEED:if (strlen(yspeed)<(sizeof(yspeed)-1)){sprintf(yspeed,"%s%s",yspeed,S);PADPrintL();}break;
    case PAD_ZSPEED:if (strlen(zspeed)<(sizeof(zspeed)-1)){sprintf(zspeed,"%s%s",zspeed,S);PADPrintL();}break;
    case PAD_P0TEMP:if (strlen(p0temp)<(sizeof(p0temp)-1)){sprintf(p0temp,"%s%s",p0temp,S);PADPrintL();}break;
    case PAD_E0LEN:if (strlen(e0len)<(sizeof(e0len)-1)){sprintf(e0len,"%s%s",e0len,S);PADPrintL();}break;
    case PAD_E0SPEED:if (strlen(e0speed)<(sizeof(e0speed)-1)){sprintf(e0speed,"%s%s",e0speed,S);PADPrintL();}break;
    case PAD_PRINTNAME:if (strlen(printname)<(sizeof(printname)-1)){sprintf(printname,"%s%s",printname,S);PADPrintL();}break;
    default:break;
  }
}
void PADDelete(){
  switch(PAD){
    case PAD_WIFINAME:if (strlen(wifiname)>0){wifiname[strlen(wifiname)-1]='\0';PADPrintE();}break;
    case PAD_WIFIPASS:if (strlen(wifipass)>0){wifipass[strlen(wifipass)-1]='\0';PADPrintE();}break;
    case PAD_XSPEED:if (strlen(xspeed)>0){xspeed[strlen(xspeed)-1]='\0';PADPrintE();}break;
    case PAD_YSPEED:if (strlen(yspeed)>0){yspeed[strlen(yspeed)-1]='\0';PADPrintE();}break;
    case PAD_ZSPEED:if (strlen(zspeed)>0){zspeed[strlen(zspeed)-1]='\0';PADPrintE();}break;
    case PAD_P0TEMP:if (strlen(p0temp)>0){p0temp[strlen(p0temp)-1]='\0';PADPrintE();}break;
    case PAD_E0LEN:if (strlen(e0len)>0){e0len[strlen(e0len)-1]='\0';PADPrintE();}break;
    case PAD_E0SPEED:if (strlen(e0speed)>0){e0speed[strlen(e0speed)-1]='\0';PADPrintE();}break;
    case PAD_PRINTNAME:if (strlen(printname)>0){printname[strlen(printname)-1]='\0';PADPrintE();}break;
    default:break;
  }
}

void tempCut(char *src, char *dst){
  src+=3;
  int i=0;
  while (*src){
    if (src[0]==0x40) break;
    if (src[0]!=0x20){
      dst[i]=src[0];
      i++;
    }
    src+=1;
  }
  dst[i]='\0';
}

void lightGcode(char *src, char *dst){
  int i=0;
  while (*src){
    if (src[0]==0x3B||src[0]==0x0A) break;
    if (dst[i-1]==0x20&&src[0]==0x20){
      i--;
      break;
    }
    dst[i]=src[0];
    i++;
    src++;
  }
  dst[i]='\n';
  dst[i+1]='\0';
}

void print_thread(){
  delayms(1000);
  char read_line[4096];
  char gcode_line[4096];
  FILE *fp;
  if ((fp=fopen(printpath,"r"))==NULL){
    printing=0;
    return;
  }
  while ((fgets(read_line,sizeof(read_line),fp))!=NULL){
    lightGcode(read_line,gcode_line);
    if (strlen(gcode_line)>2){
      char tempget[32];
      char temppost[32];
      gcode_send("M105\n");
      gcode_readline(tempget);
      tempCut(tempget,temppost);
      DrawBox(341,13,120,16,TOPBAR);
      PrintASC16(341,13,temppost,BLACK,TOPBAR,1);
      gcode_send(gcode_line);
      printf(gcode_line);
      gcode_wait("ok");
    }
  }
  fclose(fp);
  printing=0;
  printf("printing finish.\n");
}

void StartPrint(){
  if (printing==0){
    FILE *fp;
    sprintf(printpath,"gcode/%s",printname);
    if ((fp=fopen(printpath,"r"))!=NULL){
      fclose(fp);
      if (pthread_create(&thid_print,NULL,(void *) print_thread,NULL)==0){
        printing=1;
        printf("start printing...\n");
      }
    }
  }else{
    printf("still printing...\n");
  }
}

void StopPrint(){
  if (printing==1){
    while (pthread_cancel(thid_print)!=0);
    printing=0;
    printf("printing stopped.\n");
  }else{
    printf("no printing...\n");
  }
}

void loop(){
  delayms(100);
  getIP(wifiip);
  if (strlen(wifiip)<4){
    if (wifitype!=1){
      wifitype=1;
      DrawBox(12,12,169,16,TOPBAR);
      PrintHZK16(12,12,"未连接",BLACK,TOPBAR,1);
      PrintASC16(61,13,"WIFI",BLACK,TOPBAR,1);
    }
  }else{
    if (wifitype!=2){
      wifitype=2;
      DrawBox(12,12,169,16,TOPBAR);
      PrintHZK16(12,12,"已连接",BLACK,TOPBAR,1);
      PrintASC16(61,13,wifiip,BLACK,TOPBAR,1);
    }
  }
  if (printing==0){
    if (serialCount>=serialNum){
      serialCount=0;
      if (!printer_status()){
        if (serialtype!=1){
          serialtype=1;
          PrintHZK16(213,12,"未连接打印机",BLACK,TOPBAR,1);
        }
      }else{
        if (serialtype!=2){
          serialtype=2;
          PrintHZK16(213,12,"已连接打印机",BLACK,TOPBAR,1);
        }
      }
    }else{
      serialCount++;
    }
  }
  if (serialtype==2&&printing==0){
    if (tempCount>=tempNum){
      tempCount=0;
      char tempget[32];
      char temppost[32];
      gcode_send("M105\n");
      gcode_readline(tempget);
      tempCut(tempget,temppost);
      DrawBox(341,13,120,16,TOPBAR);
      PrintASC16(341,13,temppost,BLACK,TOPBAR,1);
    }else{
      tempCount++;
    }
  }
  int TX=touch_getX();
  int TY=touch_getY();
  if (TX>4050&&TY<50&&FLAG==1) FLAG=0;
  if (FLAG==1) return;
  if (InBoxT(0,0,193,40,TX,TY)&&PAGE!=PAGE_NUMPAD
    &&PAGE!=PAGE_TOUCHPAD&&PAGE!=PAGE_SHIFTPAD){
      PAGET=PAGE;
      PAGE=PAGE_WIFI;
      Slide(PAGE);
  }
  if (InBoxT(201,0,321,40,TX,TY)&&serialtype==1){
    serial_close();
    serial_open();
  }
  switch(PAGE){
    case PAGE_MAIN:
      if (InBox(20,20,210,110,TX,TY)){}
      if (InBox(250,20,210,110,TX,TY)){PAGE=PAGE_PRINT;Slide(PAGE);}
      if (InBox(20,150,135,110,TX,TY)){PAGE=PAGE_MOVE;Slide(PAGE);}
      if (InBox(172,150,135,110,TX,TY)){PAGE=PAGE_HEAT;Slide(PAGE);}
      if (InBox(325,150,135,110,TX,TY)){PAGE=PAGE_EXT;Slide(PAGE);}
      break;
    case PAGE_PRINT:
      if (InBox(387,19,75,54,TX,TY)){PAGE=PAGE_MAIN;Slide(PAGE);}
      if (InBox(121,104,338,51,TX,TY)){PAGE=PAGE_TOUCHPAD;PAD=PAD_PRINTNAME;Slide(PAGE);}
      if (InBox(76,196,150,60,TX,TY)){StartPrint();}
      if (InBox(256,196,150,60,TX,TY)){StopPrint();}
      break;
    case PAGE_MOVE:
      if (InBox(8,13,40,59,TX,TY)){if (sliderUnit!=1){sliderUnit=1;sliderCount=0.1;save_slider(sliderUnit);image_show(2,TOPBAR_H+40,1,"img/slider1.bmp");}}
      if (InBox(88,13,40,59,TX,TY)){if (sliderUnit!=2){sliderUnit=2;sliderCount=1;save_slider(sliderUnit);image_show(2,TOPBAR_H+40,1,"img/slider2.bmp");}}
      if (InBox(168,13,40,59,TX,TY)){if (sliderUnit!=3){sliderUnit=3;sliderCount=10;save_slider(sliderUnit);image_show(2,TOPBAR_H+40,1,"img/slider3.bmp");}}
      if (InBox(218,14,90,54,TX,TY)){gcode_send("G28\n");}
      if (InBox(315,14,75,54,TX,TY)){gcode_send("M84\n");}
      if (InBox(397,14,75,54,TX,TY)){PAGE=PAGE_MAIN;Slide(PAGE);}
      if (InBox(8,81,94,56,TX,TY)){gcode_send("G91\n");sprintf(gcode,"G0 Z%.1f F%s\n",sliderCount,zspeed);gcode_send(gcode);gcode_send("G90\n");}
      if (InBox(111,81,94,56,TX,TY)){gcode_send("G91\n");sprintf(gcode,"G0 X-%.1f F%s\n",sliderCount,xspeed);gcode_send(gcode);gcode_send("G90\n");}
      if (InBox(214,81,94,56,TX,TY)){gcode_send("G28 X\n");}
      if (InBox(394,86,78,51,TX,TY)){PAGE=PAGE_NUMPAD;PAD=PAD_XSPEED;Slide(PAGE);}
      if (InBox(8,148,94,56,TX,TY)){gcode_send("G91\n");sprintf(gcode,"G0 Y%.1f F%s\n",sliderCount,yspeed);gcode_send(gcode);gcode_send("G90\n");}
      if (InBox(111,148,94,56,TX,TY)){gcode_send("G91\n");sprintf(gcode,"G0 Y-%.1f F%s\n",sliderCount,yspeed);gcode_send(gcode);gcode_send("G90\n");}
      if (InBox(214,148,94,56,TX,TY)){gcode_send("G28 Y\n");}
      if (InBox(394,151,78,51,TX,TY)){PAGE=PAGE_NUMPAD;PAD=PAD_YSPEED;Slide(PAGE);}
      if (InBox(8,215,94,56,TX,TY)){gcode_send("G91\n");sprintf(gcode,"G0 Z-%.1f F%s\n",sliderCount,zspeed);gcode_send(gcode);gcode_send("G90\n");}
      if (InBox(111,215,94,56,TX,TY)){gcode_send("G91\n");sprintf(gcode,"G0 X%.1f F%s\n",sliderCount,xspeed);gcode_send(gcode);gcode_send("G90\n");}
      if (InBox(214,215,94,56,TX,TY)){gcode_send("G28 Z\n");}
      if (InBox(394,219,78,51,TX,TY)){PAGE=PAGE_NUMPAD;PAD=PAD_ZSPEED;Slide(PAGE);}
      break;
    case PAGE_HEAT:
      if (InBox(387,19,75,54,TX,TY)){PAGE=PAGE_MAIN;Slide(PAGE);}
      if (InBox(85,99,150,60,TX,TY)){sprintf(gcode,"M104 S%s\n",p0temp);gcode_send(gcode);}
      if (InBox(253,99,150,60,TX,TY)){gcode_send("M104 S0\n");}
      if (InBox(232,186,78,51,TX,TY)){PAGE=PAGE_NUMPAD;PAD=PAD_P0TEMP;Slide(PAGE);}
      break;
    case PAGE_EXT:
      if (InBox(387,19,75,54,TX,TY)){PAGE=PAGE_MAIN;Slide(PAGE);}
      if (InBox(85,99,150,60,TX,TY)){gcode_send("G91\n");sprintf(gcode,"G0 E%s F%s\n",e0len,e0speed);gcode_send(gcode);gcode_send("G90\n");}
      if (InBox(253,99,150,60,TX,TY)){gcode_send("G91\n");sprintf(gcode,"G0 E-%s F%s\n",e0len,e0speed);gcode_send(gcode);gcode_send("G90\n");}
      if (InBox(147,186,78,51,TX,TY)){PAGE=PAGE_NUMPAD;PAD=PAD_E0LEN;Slide(PAGE);}
      if (InBox(325,186,78,51,TX,TY)){PAGE=PAGE_NUMPAD;PAD=PAD_E0SPEED;Slide(PAGE);}
      break;
    case PAGE_WIFI:
      if (InBox(141,41,257,51,TX,TY)){PAGE=PAGE_TOUCHPAD;PAD=PAD_WIFINAME;Slide(PAGE);}
      if (InBox(141,115,257,51,TX,TY)){PAGE=PAGE_TOUCHPAD;PAD=PAD_WIFIPASS;Slide(PAGE);}
      if (InBox(133,193,88,56,TX,TY)){connectWiFi(wifiname,wifipass);PAGE=PAGET;Slide(PAGE);}
      if (InBox(253,193,88,56,TX,TY)){PAGE=PAGET;Slide(PAGE);}
      break;
    case PAGE_NUMPAD:
      if (InBox(387,13,80,51,TX,TY)){SavePAD(PAD);PAGE=GetPAGE(PAD);Slide(PAGE);}
      if (InBox(7,86,110,55,TX,TY)){PADInput("1");}
      if (InBox(126,86,110,55,TX,TY)){PADInput("2");}
      if (InBox(244,86,110,55,TX,TY)){PADInput("3");}
      if (InBox(363,86,110,55,TX,TY)){PADDelete();}
      if (InBox(7,152,110,55,TX,TY)){PADInput("4");}
      if (InBox(126,152,110,55,TX,TY)){PADInput("5");}
      if (InBox(244,152,110,55,TX,TY)){PADInput("6");}
      if (InBox(363,152,110,55,TX,TY)){PADInput(".");}
      if (InBox(7,217,110,55,TX,TY)){PADInput("7");}
      if (InBox(126,217,110,55,TX,TY)){PADInput("8");}
      if (InBox(244,217,110,55,TX,TY)){PADInput("9");}
      if (InBox(363,217,110,55,TX,TY)){PADInput("0");}
      break;
    case PAGE_TOUCHPAD:
      if (InBox(387,13,80,51,TX,TY)){SavePAD(PAD);PAGE=GetPAGE(PAD);Slide(PAGE);}
      if (InBox(6,81,40,41,TX,TY)){PADInput("0");}
      if (InBox(54,81,40,41,TX,TY)){PADInput("1");}
      if (InBox(101,81,40,41,TX,TY)){PADInput("2");}
      if (InBox(149,81,40,41,TX,TY)){PADInput("3");}
      if (InBox(197,81,40,41,TX,TY)){PADInput("4");}
      if (InBox(244,81,40,41,TX,TY)){PADInput("5");}
      if (InBox(292,81,40,41,TX,TY)){PADInput("6");}
      if (InBox(339,81,40,41,TX,TY)){PADInput("7");}
      if (InBox(387,81,40,41,TX,TY)){PADInput("8");}
      if (InBox(434,81,40,41,TX,TY)){PADInput("9");}
      if (InBox(6,132,40,41,TX,TY)){PADInput("q");}
      if (InBox(54,132,40,41,TX,TY)){PADInput("w");}
      if (InBox(101,132,40,41,TX,TY)){PADInput("e");}
      if (InBox(149,132,40,41,TX,TY)){PADInput("r");}
      if (InBox(197,132,40,41,TX,TY)){PADInput("t");}
      if (InBox(244,132,40,41,TX,TY)){PADInput("y");}
      if (InBox(292,132,40,41,TX,TY)){PADInput("u");}
      if (InBox(339,132,40,41,TX,TY)){PADInput("i");}
      if (InBox(387,132,40,41,TX,TY)){PADInput("o");}
      if (InBox(434,132,40,41,TX,TY)){PADInput("p");}
      if (InBox(30,183,40,41,TX,TY)){PADInput("a");}
      if (InBox(78,183,40,41,TX,TY)){PADInput("s");}
      if (InBox(125,183,40,41,TX,TY)){PADInput("d");}
      if (InBox(173,183,40,41,TX,TY)){PADInput("f");}
      if (InBox(220,183,40,41,TX,TY)){PADInput("g");}
      if (InBox(267,183,40,41,TX,TY)){PADInput("h");}
      if (InBox(315,183,40,41,TX,TY)){PADInput("j");}
      if (InBox(362,183,40,41,TX,TY)){PADInput("k");}
      if (InBox(410,183,40,41,TX,TY)){PADInput("l");}
      if (InBox(6,234,49,41,TX,TY)){PAGE=PAGE_SHIFTPAD;Slide(PAGE);PADPrint();}
      if (InBox(64,234,40,41,TX,TY)){PADInput("z");}
      if (InBox(112,234,40,41,TX,TY)){PADInput("x");}
      if (InBox(159,234,40,41,TX,TY)){PADInput("c");}
      if (InBox(207,234,40,41,TX,TY)){PADInput("v");}
      if (InBox(254,234,40,41,TX,TY)){PADInput("b");}
      if (InBox(301,234,40,41,TX,TY)){PADInput("n");}
      if (InBox(349,234,40,41,TX,TY)){PADInput("m");}
      if (InBox(396,234,73,41,TX,TY)){PADDelete();}
      break;
    case PAGE_SHIFTPAD:
      if (InBox(387,13,80,51,TX,TY)){SavePAD(PAD);PAGE=GetPAGE(PAD);Slide(PAGE);}
      if (InBox(6,81,40,41,TX,TY)){PADInput(".");}
      if (InBox(54,81,40,41,TX,TY)){PADInput(",");}
      if (InBox(101,81,40,41,TX,TY)){PADInput("?");}
      if (InBox(149,81,40,41,TX,TY)){PADInput("!");}
      if (InBox(197,81,40,41,TX,TY)){PADInput("@");}
      if (InBox(244,81,40,41,TX,TY)){PADInput("/");}
      if (InBox(292,81,40,41,TX,TY)){PADInput("_");}
      if (InBox(339,81,40,41,TX,TY)){PADInput("-");}
      if (InBox(387,81,40,41,TX,TY)){PADInput("+");}
      if (InBox(434,81,40,41,TX,TY)){PADInput("=");}
      if (InBox(6,132,40,41,TX,TY)){PADInput("Q");}
      if (InBox(54,132,40,41,TX,TY)){PADInput("W");}
      if (InBox(101,132,40,41,TX,TY)){PADInput("E");}
      if (InBox(149,132,40,41,TX,TY)){PADInput("R");}
      if (InBox(197,132,40,41,TX,TY)){PADInput("T");}
      if (InBox(244,132,40,41,TX,TY)){PADInput("Y");}
      if (InBox(292,132,40,41,TX,TY)){PADInput("U");}
      if (InBox(339,132,40,41,TX,TY)){PADInput("I");}
      if (InBox(387,132,40,41,TX,TY)){PADInput("O");}
      if (InBox(434,132,40,41,TX,TY)){PADInput("P");}
      if (InBox(30,183,40,41,TX,TY)){PADInput("A");}
      if (InBox(78,183,40,41,TX,TY)){PADInput("S");}
      if (InBox(125,183,40,41,TX,TY)){PADInput("D");}
      if (InBox(173,183,40,41,TX,TY)){PADInput("F");}
      if (InBox(220,183,40,41,TX,TY)){PADInput("G");}
      if (InBox(267,183,40,41,TX,TY)){PADInput("H");}
      if (InBox(315,183,40,41,TX,TY)){PADInput("J");}
      if (InBox(362,183,40,41,TX,TY)){PADInput("K");}
      if (InBox(410,183,40,41,TX,TY)){PADInput("L");}
      if (InBox(6,234,49,41,TX,TY)){PAGE=PAGE_TOUCHPAD;Slide(PAGE);PADPrint();}
      if (InBox(64,234,40,41,TX,TY)){PADInput("Z");}
      if (InBox(112,234,40,41,TX,TY)){PADInput("X");}
      if (InBox(159,234,40,41,TX,TY)){PADInput("C");}
      if (InBox(207,234,40,41,TX,TY)){PADInput("V");}
      if (InBox(254,234,40,41,TX,TY)){PADInput("B");}
      if (InBox(301,234,40,41,TX,TY)){PADInput("N");}
      if (InBox(349,234,40,41,TX,TY)){PADInput("M");}
      if (InBox(396,234,73,41,TX,TY)){PADDelete();}
      break;
    default:
      break;
  }
}

int main(){
  serial_open();
  wiringPiSetup();

  lcd_init();
  DrawBox(0,0,SCREEN_W,SCREEN_H,WHITE);
  delayms(300);
  image_show(0,0,0,"img/logo.bmp");
  delayms(2000);
  DrawBox(0,0,SCREEN_W,TOPBAR_H,TOPBAR);
  image_show(0,TOPBAR_H,1,"img/main.bmp");

  read_wifiname(wifiname);
  read_wifipass(wifipass);
  read_xspeed(xspeed);
  read_yspeed(yspeed);
  read_zspeed(zspeed);
  read_p0temp(p0temp);
  read_e0len(e0len);
  read_e0speed(e0speed);
  read_printname(printname);
  sliderUnit=read_slider();
  if (sliderUnit==1) sliderCount=0.1;
  if (sliderUnit==2) sliderCount=1;
  if (sliderUnit==3) sliderCount=10;

  touch_init();
  text_init();
  while(1){
    loop();
  }
  text_close();
  serial_close();

  return 0;
}
