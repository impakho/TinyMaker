#include <stdio.h>
#include "serial.c"
#include <pthread.h>

void delayms(int s){
 int k;
 for(k=0;k<s;k++)
      usleep(1000);
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

void thread(){
  char read_line[256];
  char gcode_line[256];
  FILE *fp;
  if ((fp=fopen("gcode/test.gcode","r"))==NULL) return;
  while ((fgets(read_line,sizeof(read_line),fp))!=NULL){
    lightGcode(read_line,gcode_line);
    if (strlen(gcode_line)>2){
      printf(gcode_line);
      gcode_send(gcode_line);
      gcode_wait("ok");
    }
  }
  fclose(fp);
}

int main(){
  serial_open();
  delayms(3000);

  pthread_t id;
  int i,ret;
  ret=pthread_create(&id,NULL,(void *) thread,NULL);

  if (ret!=0){
    printf("Create pthread error!\n");
    exit(1);
  }
  delayms(5000);
  while (pthread_cancel(id)!=0);
  //pthread_join(id,NULL);


  //while(1);
  serial_close();
  return 0;
}
