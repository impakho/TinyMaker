#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

int BAUDRATE = B115200;
int fd;
char fdDev[16];

void getDev(char *dev){
  FILE *fp;
  char devName[10];
  fp=popen("ls /dev|grep ttyACM|cut -d: -f1","r");
  sprintf(devName,"ttyUSB0");
  fgets(devName,10,fp);
  devName[strlen(devName)-1]='\0';
  sprintf(dev,"/dev/%s",devName);
  pclose(fp);
}

int serial_open(){
  getDev(fdDev);
  fd = open(fdDev, O_RDWR | O_NOCTTY);
  if (fd == -1) return 0;
  struct termios newtio;
  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR | ICRNL;
  newtio.c_oflag = 0;
  newtio.c_lflag = ICANON;
  tcflush(fd,TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);
  return 1;
}

void serial_close(){
  close(fd);
}

void serial_flush(){
  tcflush(fd,TCIOFLUSH);
}

void serial_readline(char *str){
  char buffer[1024];
  buffer[0]='\0';
  int n = read(fd, buffer, sizeof(buffer));
  if (n > 0) buffer[n]='\0';
  sprintf(str,"%s",buffer);
}

int serial_write(char *str){
  int n = write(fd, str, strlen(str));
  return n;
}

void gcode_send(char *str){
  serial_flush();
  serial_write(str);
}

int gcode_verify(char *str){
  char vstr[1024];
  vstr[0]='\0';
  serial_readline(vstr);
  if (strstr(vstr,str)!=NULL) return 1;
  return 0;
}

void gcode_wait(char *str){
  char vstr[1024];
  vstr[0]='\0';
  serial_readline(vstr);
  while (strstr(vstr,str)==NULL){
    vstr[0]='\0';
    serial_readline(vstr);
  }
}

void gcode_readline(char *str){
  serial_readline(str);
}

int printer_status(){
  gcode_send("M114\n");
  return gcode_verify("X:");
}
