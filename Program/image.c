#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 8192 // --- to be times of SPIBUFF
#define SPIBUFF 4096   //-- spi write buff-size for SPI_Write()
#define STRBUFF 64   // --- size of file name

FILE *fp=NULL;

void image_show(unsigned short x, unsigned short y, unsigned char flag, unsigned char *s){
 int i,j,k,nn;
 uint32_t total; // --total bytes of pic file
 uint16_t residual; // --residual after total divided by BUFFSIZE
 uint16_t nbuff;
 uint8_t dataR,dataG,dataB; //---RGB data
 uint16_t dataRGB; //--- 16bits RGB data for LCD
 uint16_t Hs,He,Vs,Ve; //--GRAM area difinition parameters
 uint16_t Hb,Vb; //--GRAM area corner gap distance from origin
 uint16_t mtmp,ntmp; 

 char strf[STRBUFF]; //--for file name

 //FILE *fp=NULL;
 uint8_t buff[BUFFSIZE];
 long offp; //offset position
 uint16_t picWidth, picHeight;
 offp=18; // file offset  position of picture Width and Height


 
      //------------------- ask for BMP file path --------------
        memset(strf,0,STRBUFF);
        strcpy(strf,s);
        //gets(strf);

        offp=18; // reset file offset for pic width and height

   //------------------------------ open file and show picture ------------
         fp=fopen(strf,"rb");
         if(fp==NULL)
         //{
           //printf("\n Fail to open the file!\n");
          //----------------if loop--------------
          return; 
          //}
         //else
          //printf("\n %s opened successfully!",strf);

    //------------    seek position and readin picWidth and picHeight   ----------
        if(fseek(fp,offp,SEEK_SET)!=0)
          return;
          //printf("\n Fail to offset seek position!");
        k=fread(buff,1,8,fp);
        //printf("\n %d bytes of data read in",k);
        picWidth=buff[3]<<24|buff[2]<<16|buff[1]<<8|buff[0];
        picHeight=buff[7]<<24|buff[6]<<16|buff[5]<<8|buff[4];
        //printf("\n picWidth=%d    picHeight=%d \n",picWidth,picHeight);
 
     // ----------------  calculate GRAM area ------------------
        /*****   WARNING: picWidth must be an 4xtimes number!! ******/
        if (flag){
          Hs=x; He=x+picWidth-1;
          Vs=y; Ve=y+picHeight-1;
        }else{
          Hb=(480-picWidth+1)/2;
          Vb=(320-picHeight+1)/2;
          Hs=Hb; He=Hb+picWidth-1;
          Vs=Vb; Ve=Vb+picHeight-1;
        }
        //printf("Hs=%d,  He=%d \n",Hs,He);
        //printf("Vs=%d,  Ve=%d \n",Vs,Ve);

        GRAM_Block_Set(Hs,He,Vs,Ve);  //--GRAM area set
        //printf("GRAM Block Set finished!\n");
     // -------------- prepare for LCD GRAM fast write ---------
        // WriteComm(0x3a); WriteData(0x66);  //set pixel format 18bits pixel
        //WriteComm(0x36); WriteData(0x00);   // 0 RGB ???--set color order as GBR,same as BMP file 
        //printf("GRAM fast write preparation finished!\n");
         
     //-------------------- read RGB data from the file  and write to GRAM ----------------
         total=picWidth*picHeight*3; //--total bytes for BGR data
         nbuff=total/BUFFSIZE;
         residual=total%BUFFSIZE;

         offp=54; //--- start point where BGR data begins

         for(i=0;i<nbuff;i++)
         {
             fseek(fp,offp+i*BUFFSIZE,SEEK_SET);
             fread(buff,BUFFSIZE,1,fp);
             for(j=0;j<BUFFSIZE/SPIBUFF;j++)
              {
                 //-----------write BGR interface to LCD, first 6bits of each 8bits for very color is valid ------------
                 WriteNData(buff+j*SPIBUFF,SPIBUFF);
               }
             //printf("Write %d_buff finished!\n",i);
         }   

     //------------------    residual data    -----------------
     fseek(fp,offp+nbuff*BUFFSIZE,SEEK_SET);
     fread(buff,residual,1,fp);
     mtmp=residual/SPIBUFF;
     ntmp=residual%SPIBUFF;
     //printf("ntmp=%d\n",ntmp);
     if(mtmp>0)
        for(j=0;j<mtmp;j++)
        {
                 //-----------write BGR interface to LCD, first 6bits of each 8bits for very color is valid ------------
                 WriteNData(buff+j*SPIBUFF,SPIBUFF);
         }
      
      if(ntmp>0)
         WriteNData(buff+mtmp*SPIBUFF,ntmp);//--ntmp=0 will cause JFFS2 file sys failure 
      //printf("---------------------   Finish drawing the picture -------------\n\n");

//WriteComm(0x29);WriteData(0x00); //--display on
//delayms(5000); //---put delay in python show prograam
//WriteComm(0x28);WriteData(0x00);  //--displya off

fclose(fp); //--fp will not set to NULL however close
fp=NULL; // --reset fp to NULL

}
