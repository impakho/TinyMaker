#!/usr/bin/env python

#=========================   NOTE  ====================================
# This script will convert a jpg image file to a 24bit-BMP file,
# The image will be resized and rotated to fit for a 480x320 LCD 
# The max. original side-size of a jpg image is limited to abt. 3500-pixels,   
# otherwise it may crash and quit.  
#
# Example:   RMjpg2bmp.py  /tmp/ P   (convert jpg files in /tmp/ to a 
# 24bit BMP filev, storing in name of Pxxx, such as P1.bmp, P2.bmp.....etc.
#----------------------------- ----------------------------------------

from PIL import Image
import os
import time
import glob
import types
import sys


strpath=sys.argv[1]  #-----user input directory

#strK='N'   #-- string add before a newfile name


k=1
token=1 

while(token):
   try:
      piclist=glob.glob(strpath+".jpg")

      if piclist==False:
          print "No picture found in the directory!"
          token=0 
      print type(piclist)
      for pitem in piclist:
	  im=Image.open(pitem)
          print "Open image file:",pitem
	  print "Format of the pictur: ",im.format,'HV:',im.size,im.mode

          #----------resize the picture and/or rotate  to fit for 480x320 LCD size---------
          imH=float(im.size[0])  #--H
          imV=float(im.size[1])  #--V
          im=im.transpose(Image.FLIP_TOP_BOTTOM)
          omH=imH
          omV=imV
          '''if imH > imV:  #----adjust pic and make sure V(height)>H(width)
             print "imH > imV"
             im=im.rotate(90)
             print "rotate 90deg finished!"
             imH=float(im.size[0])
             imV=float(im.size[1])'''
          '''if (imV/imH) >= (320.0/480.0):
             print "imV/imH >=320/480"
             omH=int(imH*(320/imV))
             omH=omH>>2
             omH=omH<<2
             omV=320
          if (imV/imH) < (320.0/480.0):
             print "imV/imH < 320/480"
             omV=int(imV*(480/imH))
             omV=omV>>2
             omV=omV<<2  
             omH=480'''

          print "omH=",omH," omV=",omV
          om=im.resize((omH,omV))  #--(width,height)
         
          strsave=strpath+'.bmp'
          print strsave
          om.save(strsave)
          print "Transfer the picture to 480x320BMP successfully!"
          k+=1
      token=0

   except KeyboardInterrupt:
	  print "Exit by User or System."
	  raise 

   except:
          print "Error!"
          raise

