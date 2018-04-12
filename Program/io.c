#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void connectWiFi(char *ssid,char *psk){
  FILE *fp1;
  if ((fp1=fopen("wifi.conf","w"))==NULL) return;
  fwrite("network={\n",10,1,fp1);
  fwrite("ssid=\"",6,1,fp1);
  fwrite(ssid,strlen(ssid),1,fp1);
  fwrite("\"\n",2,1,fp1);
  fwrite("psk=\"",5,1,fp1);
  fwrite(psk,strlen(psk),1,fp1);
  fwrite("\"\n",2,1,fp1);
  fwrite("scan_ssid=1\n",12,1,fp1);
  fwrite("}\n",2,1,fp1);
  fclose(fp1);
  FILE *fp2;
  fp=popen("./wifi.sh","r");
  pclose(fp);
}

void getIP(char *ipaddr){
  FILE *fp;
  fp=popen("ifconfig|grep 'inet addr:'|grep -v '127.0.0.1'|cut -d: -f2|awk '{print $1}'","r");
  sprintf(ipaddr,"");
  fgets(ipaddr,20,fp);
  pclose(fp);
}

void save_wifiname(char *data){
  FILE *fp;
  if ((fp=fopen("data/wifiname","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_wifiname(char *dst){
  sprintf(dst,"wifi_name");
  FILE *fp;
  if ((fp=fopen("data/wifiname","r"))==NULL) return;
  fgets(dst,33,fp);
  fclose(fp);
}

void save_wifipass(char *data){
  FILE *fp;
  if ((fp=fopen("data/wifipass","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_wifipass(char *dst){
  sprintf(dst,"wifi_pass");
  FILE *fp;
  if ((fp=fopen("data/wifipass","r"))==NULL) return;
  fgets(dst,65,fp);
  fclose(fp);
}

void save_xspeed(char *data){
  FILE *fp;
  if ((fp=fopen("data/xspeed","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_xspeed(char *dst){
  sprintf(dst,"1200");
  FILE *fp;
  if ((fp=fopen("data/xspeed","r"))==NULL) return;
  fgets(dst,8,fp);
  fclose(fp);
}

void save_yspeed(char *data){
  FILE *fp;
  if ((fp=fopen("data/yspeed","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_yspeed(char *dst){
  sprintf(dst,"1200");
  FILE *fp;
  if ((fp=fopen("data/yspeed","r"))==NULL) return;
  fgets(dst,8,fp);
  fclose(fp);
}

void save_zspeed(char *data){
  FILE *fp;
  if ((fp=fopen("data/zspeed","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_zspeed(char *dst){
  sprintf(dst,"120");
  FILE *fp;
  if ((fp=fopen("data/zspeed","r"))==NULL) return;
  fgets(dst,8,fp);
  fclose(fp);
}

void save_p0temp(char *data){
  FILE *fp;
  if ((fp=fopen("data/p0temp","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_p0temp(char *dst){
  sprintf(dst,"190");
  FILE *fp;
  if ((fp=fopen("data/p0temp","r"))==NULL) return;
  fgets(dst,8,fp);
  fclose(fp);
}

void save_e0len(char *data){
  FILE *fp;
  if ((fp=fopen("data/e0len","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_e0len(char *dst){
  sprintf(dst,"1.0");
  FILE *fp;
  if ((fp=fopen("data/e0len","r"))==NULL) return;
  fgets(dst,8,fp);
  fclose(fp);
}

void save_e0speed(char *data){
  FILE *fp;
  if ((fp=fopen("data/e0speed","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_e0speed(char *dst){
  sprintf(dst,"60.0");
  FILE *fp;
  if ((fp=fopen("data/e0speed","r"))==NULL) return;
  fgets(dst,8,fp);
  fclose(fp);
}

void save_printname(char *data){
  FILE *fp;
  if ((fp=fopen("data/printname","w"))==NULL) return;
  fwrite(data,strlen(data),1,fp);
  fclose(fp);
}

void read_printname(char *dst){
  sprintf(dst,"print_file.gcode");
  FILE *fp;
  if ((fp=fopen("data/printname","r"))==NULL) return;
  fgets(dst,33,fp);
  fclose(fp);
}

void save_slider(int unit){
  FILE *fp;
  if ((fp=fopen("data/slider","w"))==NULL) return;
  switch(unit){
    case 1:fwrite("1",1,1,fp);break;
    case 2:fwrite("2",1,1,fp);break;
    case 3:fwrite("3",1,1,fp);break;
    default:fwrite("2",1,1,fp);break;
  }
  fclose(fp);
}

int read_slider(){
  int dst=2;
  char fpget[2];
  FILE *fp;
  if ((fp=fopen("data/slider","r"))==NULL) return dst;
  fgets(fpget,2,fp);
  switch(fpget[0]){
    case 0x31:dst=1;break;
    case 0x32:dst=2;break;
    case 0x33:dst=3;break;
    default:dst=2;break;
  }
  fclose(fp);
  return dst;
}
