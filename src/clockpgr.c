#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define D_THREAD
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <dirent.h>

#include <signal.h>
#include <sys/select.h>

#include "kulina.h"
//#include "clockCallbacks.h"
#include <time.h>
#include "images.c"
#include "logo.c"
#include "mask.c"
#define NANGS 60  // DO NOT CHANGE THIS UNLESS YOU MAKE PROPER CORRECTIONS
#define WIDTH 306
#define NOSECARMRESO 5
// timeresolution in secs
pthread_mutex_t _Tmplock=PTHREAD_MUTEX_INITIALIZER;
void RunSetAlertDia(void *);
DIALOG *ClockD;
int RESTART=0;
int TIMERESO=1;
float Transparency;
float scalefac=0.65;
//unsigned int Red=220,Green=240,Blue=140;
unsigned int Red=230,Green=240,Blue=230;
int Xpos=-1,Ypos=-1,DateFont=21,DateColor=75,HarmColor=76,MarmColor=77,SarmColor=78,DotColor=79;
int Dred=230,Dgreen=230,Dblue=230;
int Hred=0,Hgreen=0,Hblue=255;
int Mred=0,Mgreen=255,Mblue=0;
int Sred=255,Sgreen=0,Sblue=0;
int Tred=255,Tgreen=255,Tblue=0;
int Armtype=0;
int Nosecarm=0;
int Redarm=1;
int Type=10;
int Shadow=0,ShadowColor=74;
int ShDate=1;
int Daemon=1;
int Soff=4;
int Color=1;
int pid;
int pipeid[2];
int pipeid1[2];
int pipeid2[2];
int sig[3]={1,1,0};
int rsig[3]={1,1,0};
int XO,YO,XM,YM,XL,YL;
float omangle=-1;
pthread_t Pth,Nth,Dth;
typedef struct _offstr { int xo,yo;} OFFSTR;
GMIMG *img=NULL,*rtimg,*rzimg,*hourarm,*minutearm,*secarm,*dot,*harm[120],*marm[120],*sarm[120];
GMIMG *tmpimg=NULL,*shimg,*armimg;
GMIMG *himg,*mimg,*simg[60],*HMimg=NULL,*Bimg=NULL;
GMIMG *MaskImg=NULL;
OFFSTR hoff[120],moff[120],soff[120];
void GetActionsDisplay(char **pt);
void *RunInstallDia(void *arg);
void *eventMonitor(void *Tmp);
void *dummyMonitor(void *Tmp);
void *RunShowDateDia(void *arg);
int RunclockMenu(void *arg);
void *DrawClockPicture(double r);


int hour,minute,sec;
void *RunConfigDia(void *arg);
void * notify(void *arg);
void * TakeActions(void *);
void * setnotify(void *arg);
int Writedb(void);
void DeleteAction(int item);
int GetActionsCount(void);


void *ShowDate(void *arg) {
  int xl,yl;
  char buff[200];
  char *Month[]={(char *)"JAN",(char *)"FEB",(char *)"MAR",(char *)"APR",(char *)"MAY",(char *)"JUN",(char *)"JUL",(char *)"AUG",(char *)"SEP",(char *)"OCT",(char *)"NOV",(char *)"DEC"};
  char *Day[]=  {(char *)"SUN",(char *)"MON",(char *)"TUE",(char *)"WED",(char *)"THU",(char *)"FRI",(char *)"SAT"};
  time_t tp,t;
  struct tm *dt;
  xl = (float)XL*0.4;
  yl =(float)YL*0.14;

  t=time(&tp);
  dt = localtime(&t);
  sprintf(buff,"!c15!f08%-s\\n!c15!f08!z32%2.2d %-s",Day[dt->tm_wday],dt->tm_mday,Month[dt->tm_mon]);
  if(yl>40) kgSplashDia(XM-xl,YM-40,2*xl,80,NULL,buff,10,0,0x00000000);
  else      kgSplashDia(XM-xl,YM-yl,2*xl,2*yl,NULL,buff,10,0,0x00000000);
  return NULL;
}
void DisplayDate(void){
  static int entry=0;
  if(entry) {
   if(pthread_tryjoin_np(Dth,NULL)) return;
  }
  entry=1;
  pthread_create(&Dth,NULL,ShowDate,NULL);
  return;
}
int getWait(void) {
  struct timeval tv;
  struct tm t;
  int waitsec=0;
  gettimeofday(&tv,NULL);
  localtime_r(&(tv.tv_sec),&t);
  hour = (t.tm_hour)%12;
  minute = t.tm_min;
  sec = t.tm_sec;
  waitsec = TIMERESO - (sec%TIMERESO);
  if(waitsec < 1) waitsec=1;
  return waitsec;
}
int getmicroWait(void) {
  struct timeval tv;
  struct tm t;
  int waitmicrosec=0,wsec=0;;
  gettimeofday(&tv,NULL);
  localtime_r(&(tv.tv_sec),&t);
  hour = (t.tm_hour)%12;
  minute = t.tm_min;
  sec = t.tm_sec;
  wsec = TIMERESO;
  if(wsec ==0 ) wsec=1;
  waitmicrosec = wsec*1000000 - ((sec%wsec )*1000000+tv.tv_usec);
  if(waitmicrosec < 1) waitmicrosec=1;
//  printf( "%d %d\n",tv.tv_usec,waitmicrosec);
  return waitmicrosec;
}
 
void *NeedlesImage_org(float ln,float hangle,float mangle,float sangle) {
   int i,MAG=3,Ang;
   DIG *fid;
   double pi=3.14159265359;
   void *img=NULL;
   static char *tmpdir=NULL,flname[200];
   double cs,ss,angle,xx,yy;
   float length=0.0,fac,th,tw,size,dl,dr;
   float h,s,v,red,blue,green,vmax=1.0,vmin=0.7,x[6],y[6],fmin=0.8,fmax=1.0;
   int r,g,b;
   float hx[10],hy[10],hv[10];
   float hxs[10],hys[10];
   if(scalefac < 0.76 ){
     MAG+=1;
   }
   if(scalefac < 0.4 )MAG=4;
   MAG=2;
   fid = (DIG *)kgInitImage(2*ln,2*ln,MAG);
   if(fid != NULL ) {
#if 1
      kgUserFrame(fid,(float)(-ln),(float)(-ln),(float)(ln),(float)(ln));
      size = 0.5*ln;
      dl = size*0.06;
      if(dl<2) dl=2;
      hx[0]= -ln*.02; hy[0]=0.0;
      hx[1]=hx[0],hy[1]=dl*0.5; hx[2]=size;hy[2]=hy[1];
      hx[3]= size; hy[3]=0.0;
      hx[4]= hx[2],hy[4]=-hy[2]; hx[5]=hx[1];hy[5]=-hy[1];
      angle = (360.0 -hangle+90)*pi/180.0;
      cs = cos(angle);
      ss = sin(angle);
      kgGetDefaultRGB(HarmColor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmin= v*fmin; vmax = v*fmax;
      kgMixHsvColors(fid,200,32,h,s,vmin,vmax);
      kgSetShadeParams(fid,vmax,vmin,200, 32, 0);
      kgLineColor(fid,HarmColor);

      for(i=0;i<6;i++) { 
         xx = hx[i]*cs+hy[i]*ss;
         yy = hx[i]*ss-hy[i]*cs;
         hx[i]=xx;hy[i]=yy;
         hv[i]=vmin;
      }
      hv[3]=vmax;hv[4]=vmax;hv[2]=vmax;
      hv[0]=vmin;hv[1]=vmin;hv[5]=vmin;
      if(Shadow) {
       for(i=0;i<6;i++) { 
        hxs[i] = hx[i]+0.5;
        hys[i] = hy[i]-1.5;
       }
       kgPolyFill(fid,6,hxs,hys,0,ShadowColor);
      }
      kgGouraudFill(fid,6,hx,hy,hv);
      dr = 2*ln*0.045;
      if(dr< dl) dr=dl;
      kgRoundedRectangleShade0(fid,0.,0.,dr,dr,HarmColor,0.5,vmin,vmax);
      kgGetDefaultRGB(MarmColor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmin= v*fmin; vmax = v*fmax;
      kgMixHsvColors(fid,200,32,h,s,vmin,vmax);
      kgSetShadeParams(fid,vmax,vmin,200, 32, 0);
      kgLineColor(fid,MarmColor);
      size = 0.7*ln;
      dl = size*0.04;
      if(dl<1.5) dl=1.5;
      hx[0]= -ln*.02; hy[0]=0.0;
      hx[1]= hx[0],hy[1]=dl*0.5; hx[2]=size;hy[2]=hy[1];
      hx[3]= size; hy[3]=0.0;
      hx[4]= hx[2],hy[4]=-hy[2]; hx[5]=hx[1];hy[5]=-hy[1];
      angle = (360.0 -mangle+90)*pi/180.0;
      cs = cos(angle);
      ss = sin(angle);
      for(i=0;i<6;i++) { 
         xx = hx[i]*cs+hy[i]*ss;
         yy = hx[i]*ss-hy[i]*cs;
         hx[i]=xx;hy[i]=yy;
      }
      hv[3]=vmax;hv[4]=vmax;hv[2]=vmax;
      hv[0]=vmin;hv[1]=vmin;hv[5]=vmin;
      if(Shadow) {
       for(i=0;i<6;i++) { 
        hxs[i] = hx[i]+1.5;
        hys[i] = hy[i]-2.0;
       }
       kgPolyFill(fid,6,hxs,hys,0,ShadowColor);
      }
      kgGouraudFill(fid,6,hx,hy,hv);
      dr = 2*ln*0.0375;
      if(dr< dl) dr=dl;
      kgRoundedRectangleShade0(fid,0.,0.,dr,dr,MarmColor,0.5,vmin,vmax);
      if(Nosecarm==0) {
        kgGetDefaultRGB(SarmColor,&r,&g,&b);
        RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
        vmin= v*fmin; vmax = v*fmax;
        kgMixHsvColors(fid,200,32,h,s,vmin,vmax);
        kgSetShadeParams(fid,vmax,vmin,200, 32, 0);
        kgLineColor(fid,SarmColor);
        size = 0.85*ln;
        dl = size*0.02;
        if(dl<1.5) dl=1.5;
//        if(dl<2) dl=2;
        hx[0]= -ln*.1; hy[0]=0.0;
        hx[1]= hx[0],hy[1]=dl*0.5; hx[2]=size;hy[2]=hy[1];
        hx[3]= size; hy[3]=0.0;
        hx[4]= hx[2],hy[4]=-hy[2]; hx[5]=hx[1];hy[5]=-hy[1];
        angle = (360.0 -sangle+90)*pi/180.0;
        cs = cos(angle);
        ss = sin(angle);
        for(i=0;i<6;i++) { 
           xx = hx[i]*cs+hy[i]*ss;
           yy = hx[i]*ss-hy[i]*cs;
           hx[i]=xx;hy[i]=yy;
        }
        hv[3]=vmax;hv[4]=vmax;hv[2]=vmax;
        hv[0]=vmin;hv[1]=vmin;hv[5]=vmin;
        if(Shadow) {
         for(i=0;i<6;i++) { 
          hxs[i] = hx[i]+2;
          hys[i] = hy[i]-3;
         }
         kgPolyFill(fid,6,hxs,hys,0,ShadowColor);
        }
        kgGouraudFill(fid,6,hx,hy,hv);
      }
      kgLineColor(fid,DotColor);
      dr = 2*ln*0.03;
      if(dr< dl) dr=dl;
      kgRoundedRectangleShade0(fid,0.,0.,dr,dr,SarmColor,0.5,vmin,vmax);
      kgGetDefaultRGB(DotColor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmin= v*fmin;
      vmax = v*fmax;
      kgRoundedRectangleShade0(fid,0.,0.,2*ln*0.02,2*ln*0.02,DotColor,0.5,vmin,vmax);
      kgCircle2f(fid,0.0,0.0,ln*0.02);
#else
#endif
      img = kgGetResizedImage(fid);
//      img = kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void *HMNeedlesImage(float ln,float hangle,float mangle) {
   int i,MAG=3,Ang;
   DIG *fid;
   double pi=3.14159265359;
   void *img=NULL;
   static char *tmpdir=NULL,flname[200];
   double cs,ss,angle,xx,yy;
   float length=0.0,fac,th,tw,size,dl,dr;
   float h,s,v,red,blue,green,vmax=1.0,vmin=0.7,x[6],y[6],fmin=0.8,fmax=1.0;
   int r,g,b;
   float hx[10],hy[10],hv[10];
   float hxs[10],hys[10];
   if(scalefac < 0.76 ){
     MAG+=1;
   }
   if(scalefac < 0.4 )MAG=4;
   MAG=4;
   fid = (DIG *)kgInitImage(2*ln,2*ln,MAG);
   if(fid != NULL ) {
      kgUserFrame(fid,(float)(-ln),(float)(-ln),(float)(ln),(float)(ln));
      size = 0.5*ln;
      dl = size*0.06; // Hour Needle width
      if(dl<2) dl=2;
      hx[0]= -ln*.04; hy[0]=0.0;
      hx[1]=hx[0],hy[1]=dl*0.5; hx[2]=size;hy[2]=hy[1];
      hx[3]= size; hy[3]=0.0;
      hx[4]= hx[2],hy[4]=-hy[2]; hx[5]=hx[1];hy[5]=-hy[1];
      angle = (360.0 -hangle+90)*pi/180.0;
      cs = cos(angle);
      ss = sin(angle);
      kgGetDefaultRGB(HarmColor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmin= v*fmin; vmax = v*fmax;
      kgMixHsvColors(fid,200,32,h,s,vmin,vmax);
      kgSetShadeParams(fid,vmax,vmin,200, 32, 0);
      kgLineColor(fid,HarmColor);

      for(i=0;i<6;i++) { 
         xx = hx[i]*cs+hy[i]*ss;
         yy = hx[i]*ss-hy[i]*cs;
         hx[i]=xx;hy[i]=yy;
         hv[i]=vmin;
      }
      hv[3]=vmax;hv[4]=vmax;hv[2]=vmax;
      hv[0]=vmin;hv[1]=vmin;hv[5]=vmin;
      if(Shadow) {
       for(i=0;i<6;i++) { 
        hxs[i] = hx[i]+0.5;
        hys[i] = hy[i]-1.5;
       }
       kgPolyFill(fid,6,hxs,hys,0,ShadowColor);
      }
      kgGouraudFill(fid,6,hx,hy,hv);
      dr = 2*ln*0.045;
      if(dr< dl) dr=dl;
      kgRoundedRectangleShade0(fid,0.,0.,dr,dr,HarmColor,0.5,vmin,vmax);
      kgGetDefaultRGB(MarmColor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmin= v*fmin; vmax = v*fmax;
      kgMixHsvColors(fid,200,32,h,s,vmin,vmax);
      kgSetShadeParams(fid,vmax,vmin,200, 32, 0);
      kgLineColor(fid,MarmColor);
      size = 0.7*ln;
      dl = size*0.05;//minute needle width
      if(dl<2) dl=2;
      hx[0]= -ln*.04; hy[0]=0.0;
      hx[1]= hx[0],hy[1]=dl*0.5; hx[2]=size;hy[2]=hy[1];
      hx[3]= size; hy[3]=0.0;
      hx[4]= hx[2],hy[4]=-hy[2]; hx[5]=hx[1];hy[5]=-hy[1];
      angle = (360.0 -mangle+90)*pi/180.0;
      cs = cos(angle);
      ss = sin(angle);
      for(i=0;i<6;i++) { 
         xx = hx[i]*cs+hy[i]*ss;
         yy = hx[i]*ss-hy[i]*cs;
         hx[i]=xx;hy[i]=yy;
      }
      hv[3]=vmax;hv[4]=vmax;hv[2]=vmax;
      hv[0]=vmin;hv[1]=vmin;hv[5]=vmin;
      if(Shadow) {
       for(i=0;i<6;i++) { 
        hxs[i] = hx[i]+1.5;
        hys[i] = hy[i]-2.0;
       }
       kgPolyFill(fid,6,hxs,hys,0,ShadowColor);
      }
      kgGouraudFill(fid,6,hx,hy,hv);
      dr = 2*ln*0.0375;
      if(dr< dl) dr=dl;
      kgRoundedRectangleShade0(fid,0.,0.,dr,dr,MarmColor,0.5,vmin,vmax);
      img = kgGetResizedImage(fid);
//      img = kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void *SNeedleImage(float ln,float sangle) {
   int i,MAG=3,Ang;
   DIG *fid;
   double pi=3.14159265359;
   void *img=NULL;
   static char *tmpdir=NULL,flname[200];
   double cs,ss,angle,xx,yy;
   float length=0.0,fac,th,tw,size,dl,dr;
   float h,s,v,red,blue,green,vmax=1.0,vmin=0.7,x[6],y[6],fmin=0.8,fmax=1.0;
   int r,g,b;
   float hx[10],hy[10],hv[10];
   float hxs[10],hys[10];
   if(scalefac < 0.76 ){
     MAG+=1;
   }
   if(scalefac < 0.4 )MAG=4;
   MAG=3;
   fid = (DIG *)kgInitImage(2*ln,2*ln,MAG);
   if(fid != NULL ) {
      kgUserFrame(fid,(float)(-ln),(float)(-ln),(float)(ln),(float)(ln));
      size = 0.5*ln;
      dl = size*0.06;
      if(dl<2) dl=2;
      if(Nosecarm==0) {
        kgGetDefaultRGB(SarmColor,&r,&g,&b);
        RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
        vmin= v*fmin; vmax = v*fmax;
        kgMixHsvColors(fid,200,32,h,s,vmin,vmax);
        kgSetShadeParams(fid,vmax,vmin,200, 32, 0);
        kgLineColor(fid,SarmColor);
        size = 0.85*ln;
        dl = size*0.02;
        if(dl<1.5) dl=1.5;
//        if(dl<2) dl=2;
        hx[0]= -ln*.1; hy[0]=0.0;
        hx[1]= hx[0],hy[1]=dl*0.5; hx[2]=size;hy[2]=hy[1];
        hx[3]= size; hy[3]=0.0;
        hx[4]= hx[2],hy[4]=-hy[2]; hx[5]=hx[1];hy[5]=-hy[1];
        angle = (360.0 -sangle+90)*pi/180.0;
        cs = cos(angle);
        ss = sin(angle);
        for(i=0;i<6;i++) { 
           xx = hx[i]*cs+hy[i]*ss;
           yy = hx[i]*ss-hy[i]*cs;
           hx[i]=xx;hy[i]=yy;
        }
        hv[3]=vmax;hv[4]=vmax;hv[2]=vmax;
        hv[0]=vmin;hv[1]=vmin;hv[5]=vmin;
        if(Shadow) {
         for(i=0;i<6;i++) { 
          hxs[i] = hx[i]+2;
          hys[i] = hy[i]-3;
         }
         kgPolyFill(fid,6,hxs,hys,0,ShadowColor);
        }
        kgGouraudFill(fid,6,hx,hy,hv);
      }
      kgLineColor(fid,DotColor);
      dr = 2*ln*0.03;
      if(dr< dl) dr=dl;
      kgRoundedRectangleShade0(fid,0.,0.,dr,dr,SarmColor,0.5,vmin,vmax);
      kgGetDefaultRGB(DotColor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmin= v*fmin;
      vmax = v*fmax;
      kgRoundedRectangleShade0(fid,0.,0.,2*ln*0.02,2*ln*0.02,DotColor,0.5,vmin,vmax);
      kgCircle2f(fid,0.0,0.0,ln*0.02);
      img = kgGetResizedImage(fid);
//      img = kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
 
void putDisplay(DIALOG *D,int ln,int offset) {
  int ih,im,is,hflip,hflop,mflip,mflop,sflip,sflop,hi,mi,si;
  float hangle,mangle,sangle,dangle;
  int indx;
  GMIMG *Nimg;
  dangle = 360.0/NANGS;
  hangle = hour*30.0+(minute/60.0)*30.0;
  hangle =((int)(hangle+0.00001)/1)*1;
  mangle = minute*6+sec*0.1;
  mangle =((int)(mangle+0.00001)/3)*3;
  sangle = sec*6;
    kgCleanBackground(D,offset,offset,ln,ln,Transparency);
    tmpimg =(GMIMG *)kgCleanImage(tmpimg);
    if(omangle != mangle ) {
      Bimg =(GMIMG *)kgCleanImage(Bimg);
      Bimg = (GMIMG *)kgMergeImages(Bimg,img,0,0);
      HMimg = (GMIMG *)HMNeedlesImage(ln*0.4,hangle,mangle);
      Bimg = (GMIMG *)kgMergeImages(Bimg,HMimg,0,0);
      omangle=mangle;
      kgFreeImage(HMimg);
      HMimg=NULL;
    }
    tmpimg = (GMIMG *)kgMergeImages(tmpimg,Bimg,0,0);
    indx = (int)(sec)%60;
    if(simg[indx]==NULL) {
      simg[indx]=  (GMIMG *)SNeedleImage(ln*0.4,(float)indx*6.0);
    }
    tmpimg = (GMIMG *)kgMergeImages(tmpimg,simg[indx],0,0);
    kgImage(D,tmpimg,offset,offset,ln,ln,1.0,1.0);
    kgUpdateOn(D);
//    kgSkipEvents(D);
}
int CheckAndProcessEvent(DIALOG *D) {
#if 1
  fd_set rfds;
  struct timeval tv1;
  KBEVENT kbe;
  int xp,yp,retval,waitsec;
  int pid;
        while(1) {
         if(kgCheckEvent(D,&kbe)){
//          if(kgCheckMousePressEvent(D,&kbe)){
          if(kbe.event !=1) { 
//            printf("Got 1\n");
            continue;
          }
          if(kbe.button == 3 ) {
            if(ShDate!=1) DisplayDate();
            continue;
          }
          if(kbe.button !=1)continue;
          xp = abs(kbe.x- D->xl/2);
          yp = abs(kbe.y- D->yl/2);
//          printf("GotEvent %d %d %d %d %d %d\n",kbe.x,kbe.y, D->xl/2,D->yl/2,xp,yp);
          if( (xp<20)&&(yp<20)) {
            FD_ZERO(&rfds);
            FD_SET(pipeid[0], &rfds);
            FD_SET(pipeid1[0], &rfds);
            tv1.tv_sec = 0;
            tv1.tv_usec = 100;
            retval = select(pipeid1[0]+1, &rfds, NULL, NULL, &tv1);
//            printf("retval = %d\n",retval);
            rsig[0]=1;
            if(retval ) {
              if(FD_ISSET(pipeid[0],&rfds)) read(pipeid[0],rsig+0,sizeof(int));
              if(FD_ISSET(pipeid1[0],&rfds)) read(pipeid1[0],rsig+0,sizeof(int));
#ifdef D_THREAD
              pthread_join(Pth,NULL);
#endif
              if(rsig[0]==0) return 0;
#ifdef D_THREAD
              pthread_create(&Pth,NULL,eventMonitor,(void *)D);
#else
              if((pid=fork())==0) {
                if(fork()==0) {
//                 printf("eventMonitor \n");
                 eventMonitor(D);
                 exit(0);
                }
                else exit(0);
              } 
              else {
                 int status;
                 waitpid(pid,&status,0);
              }
#endif
            }
            else break;
          }
        }
        else break;
       }
#endif
  return 1;
}
void MakeClockImages() {
  char flname[100],*tmpdir;
  float angle=0,hangle,mangle,sangle;
  int xres,yres,xl=WIDTH,yl=WIDTH,ln,offset,xp,yp; 
  xl =WIDTH*scalefac;
  yl=WIDTH*scalefac; 
  offset=0;
  ln = xl;
  kgDisplaySize(&xres,&yres); 
  switch(Type) {
     case 0:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum_str);
         break;
     case 1:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum1_str);
         break;
     case 2:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum2_str);
         break;
     case 3:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum3_str);
         break;
     case 4:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum4_str);
         break;
     case 5:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum16_str);
         break;
     case 6:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum6_str);
         break;
     case 7:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum7_str);
         break;
     case 8:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum8_str);
         break;
     case 9:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum0_str);
         break;
     case 10:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum10_str);
         break;
     case 11:
         rzimg = (GMIMG *)kgGetImageCopy(NULL,(void *)&clocknum15_str);
         break;
     default:
         rzimg = (GMIMG *)DrawClockPicture(280.0);
         break;
  }
  img = (GMIMG *)kgResizeImage(rzimg,scalefac);
  tmpimg = (GMIMG *)kgCopyImage(img);
  Bimg   = (GMIMG *)kgCopyImage(img);
  kgFreeImage(rzimg);
}
void * UpdateClock(void * arg){
  char flname[100],tmpdir[200];
  float angle=0,hangle,mangle,sangle;
  int xres,yres,xl=WIDTH,yl=WIDTH,ln,offset,xp,yp; 
  int i,ih,im,waitsec,retval;
  fd_set rfds;
  struct timeval tv1;
  DIALOG *D;
  int count=0;
  KBEVENT kbe;

  D= (DIALOG *)arg;
  omangle=-1;
  xl =WIDTH*scalefac;
  yl=WIDTH*scalefac; 
  offset=0;
  ln = D->xl;
  kgUpdateOn(D);
  for(i=0;i<60;i++) simg[i]= NULL;
  waitsec=getmicroWait();
  putDisplay(D,ln,offset);
#ifdef D_THREAD
  pthread_create(&Pth,NULL,dummyMonitor,arg);
#else
  write(pipeid[1],sig+1,sizeof(int));
#endif
  while(1) {
    if(!CheckAndProcessEvent(D)) break;
    FD_ZERO(&rfds);
    FD_SET(pipeid1[0], &rfds);
    tv1.tv_sec = waitsec/1000000;
    tv1.tv_usec = waitsec%1000000;
    retval = select(pipeid1[0]+1, &rfds, NULL, NULL, &tv1);
    if(retval ) {
       rsig[0]=1;
       if(FD_ISSET(pipeid1[0],&rfds)){
         read(pipeid1[0],rsig+0,sizeof(int));
         if(rsig[0]==0){
#ifdef D_THREAD
           pthread_join(Pth,NULL);
#endif
           break;
         }
       }
       waitsec=getmicroWait();
       continue;
    }
    waitsec=getmicroWait();
    putDisplay(D,ln,offset);
  }
  for(i=0;i<60;i++) kgFreeImage(simg[i]);
  kgFreeImage(Bimg);
  kgFreeImage(tmpimg);
  return NULL;
}

void clockdia1gbox1init(int i,void *Tmp) {
  DIALOG *D;
  DIG *G;
  D = (DIALOG *)Tmp;
  G = D->d[i].g;
  return ;
}
int clockdia1init(void *Tmp) {
  int ret = 0;
  
  DIALOG *D;
  D = (DIALOG *)Tmp;
  ClockD= D;
  if(ShDate==1) pthread_create(&Dth,NULL,RunShowDateDia,NULL);
  UpdateClock(D);
  kgDropFocus(D);
  return ret;
}
int clockdia1CallBack(void *Tmp,void *tmp) {
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
void *MakeMaskImage(int size) {
   DIG *fid;
   char *tmpdir;
   void *img;
   char flname[300];
   MakeClockImages( );
   if(Type==6) return NULL;
   fid =(DIG *)kgInitImage(size,size,1);
   if(fid== NULL) return NULL;
   kgUserFrame(fid,0.,0.,(float)size,(float)size);
   kgLineColor(fid,10);
   kgBoxFill(fid,0.0,0.0,(float)size,(float)size,15,0);
   kgArcFill(fid,(float)(size)*0.5,(float)(size)*0.5,(float)(size)*0.5,0.0,360.,0,13);
   img=kgGetResizedImage(fid);
//   img=kgGetSharpImage(fid);
   kgCloseImage(fid); 
   return img;
}
int clockdia1( void *parent ) {
  int ret=1;
  int xres,yres,offset,ln;
  DIALOG D;
  DIA d[2];
  DIP p0 = { 
    'p',
    10,10,  
    310,310,  
//"##clocknum.png" , // Pixmap info
    NULL,
    -1,0,0,0.0 /* bkgr colour */ 
  };
  d[0].p = &p0;
  d[1].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 5;
  D.df = 4;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  kgDisplaySize(&xres,&yres); 
  offset = 0*scalefac;
  D.xl =WIDTH*scalefac;
  ln =D.xl-4*offset;
  D.yl=WIDTH*scalefac; 
  D.xo = xres-D.xl*1.25-offset;
  D.yo = D.yl*0.2;
  p0.x1=0;
  p0.y1=0;
  p0.x2=D.xl;
  p0.y2=D.yl;
  D.Initfun = clockdia1init;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 1.0;    /*  float 1.0 for full transparency */
  Transparency=D.transparency;
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 0;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 1;    /*  1 for stickyness */
  D.StackPos = -1;    /*  1 for stickyness */
  D.Resize=0;
#if 0 
  D.Callback = clockdia1CallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
  D.parent = parent;    /*  1 for not showing in task bar*/
  strcpy(D.name,"Kulina Clock");    /*  Dialog name you may change */
  D.pt=NULL;
  MaskImg = (GMIMG *)MakeMaskImage(D.xl);
  D.Shapexpm =MaskImg;
//  D.Shapexpm= NULL;
  if((Xpos !=-1)&&(Ypos!=-1)) {
   int xo,yo;
   xo = D.xo;
   yo = yres -D.yl*1.8;
   D.xo = Xpos -D.xl/2;
   D.yo = Ypos -D.yl/2;
   if(D.xo<0) D.xo=0;
   if(D.yo<0) D.yo=0;
   if(D.xo > xo) D.xo=xo;
   if(D.yo > yo) D.yo=yo;
  }
  XO = D.xo;
  YO = D.yo;
  XM = D.xo+D.xl/2;
  YM = D.yo+D.yl/2;
  XL = D.xl;
  YL = D.yl;
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
  }    /*  end of fullscreen mode */
//  kgDefaultGuiTheme(&(D.gc));    /*  set colors for gui*/
  kgColorTheme(&D,Red,Green,Blue);
  D.SearchList=NULL;
  ret= kgUi(&D);
  kgFreeImage(img);
  kgFreeImage(MaskImg);
  img=NULL;
  MaskImg=NULL;
  return ret;
}
void *Runclockdia1(void *arg) {
/*************************************************


*************************************************/
   clockdia1(NULL );
   return NULL;
}
void *dummyMonitor(void *Tmp) {
  write(pipeid[1],sig+1,sizeof(int));
  write(pipeid2[1],sig+1,sizeof(int));
  return NULL;
}
int doEvents(int item) {
   void *img,*img1;
   int ret=0,x,y,xm,ym,offset,ln,xres,yres,xo,yo;
   int Color;
   ym = YM-40;
           switch(item) {
             case 1:
               RunConfigDia(NULL);
               RESTART=1;
               write(pipeid1[1],sig+2,sizeof(int));
               ret=1;
               break;
             case 2:
               RESTART=1;
//TCB
               RunSetAlertDia(NULL);
               write(pipeid[1],sig+1,sizeof(int));
               ret=0;
               break;
             case 3:
               RESTART=0;
               write(pipeid1[1],sig+2,sizeof(int));
               ret=1;
               break;
             case 4:
               Color = Red*1000000+Green*1000+Blue;
               img=NULL; img1=NULL;
//               img = kgBorderedRectangle(420,380,-Color,0.05);
//               img1 = kgGetImageCopy(NULL,&About_str);
#if 1
               img = kgBorderedRectangle(416,440,-Color,0.02);
//               img1 = kgGetImageCopy(NULL,&About_str);
               img1 = kgGetImageCopy(NULL,&logo_str);
               kgMergeImages(img,img1,0,0);
               kgAddTransparency(img,0.1);
               kgFreeImage(img1);
               kgSplashDia(-1,-1,426,450,(char *)img,(char *)"",10,0,0);
#else
               img = kgGetImageCopy(NULL,&logo_str);
//               kgSplashDia(-1,-1,430,390,(char *)img,(char *)"",10,0,0);
               kgSplashDia(-1,-1,380,420,(char *)img,(char *)"",10,0,0);
#endif
               if(img!= NULL) kgFreeImage(img);
               write(pipeid[1],sig+1,sizeof(int));
               ret=0;
               break;
             default:
               ret=0;
               write(pipeid[1],sig+1,sizeof(int));
               break;
           } 
   return item;
}
void *eventMonitor_o(void *Tmp) {
   int ret=0,x,y,xm,ym,offset,ln,xres,yres,xo,yo;
   char *Menu[]={(char *)"Settings",(char *)"Actions",(char *)"!c03Close",(char *)"About",(char *)"GoBack",NULL};
   xm = XM-40;
   ym = YM-40;
           switch(RunclockMenu(NULL)) {
             case 1:
               RunConfigDia(NULL);
               RESTART=1;
               write(pipeid1[1],sig+2,sizeof(int));
               ret=1;
               break;
             case 2:
               RunSetAlertDia(NULL);
               write(pipeid[1],sig+1,sizeof(int));
               ret=0;
               break;
             case 3:
               RESTART=0;
               write(pipeid1[1],sig+2,sizeof(int));
               ret=1;
               break;
             case 4:
               kgSplashDia(-1,-1,360,480,(char *)&About_str,(char *)"",
                               10,0,0xf0cfcfcf);
               write(pipeid[1],sig+1,sizeof(int));
               ret=0;
               break;
             default:
               ret=0;
               write(pipeid[1],sig+1,sizeof(int));
               break;
           } 
   return NULL;
}
void *eventMonitor(void *Tmp) {
   if(!RunclockMenu(NULL)) write(pipeid[1],sig+1,sizeof(int));
   write(pipeid2[1],sig+1,sizeof(int));
   return NULL;
}
void Runclock(void) {
  int status;
/*************************************************


*************************************************/
   pipe(pipeid);
   pipe(pipeid1);
   pipe(pipeid2);
   Soff = (Soff*scalefac)+0.5;
   pthread_create(&Nth,NULL,TakeActions,NULL);
   Runclockdia1(NULL);
   pthread_cancel(Nth);
   pthread_join(Nth,NULL);
}
int  ConfigDiafslide1callback(double val,int i,void *Tmp) {
  DIA *D;DIF *F; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  F = D[i].f;
  return ret;
}
int  ConfigDiahoribar1callback(int key,int i,void *Tmp) {
  DIA *D;DILN *B; 
  int n,ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].H;
  n = B->nx;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabutnbox1callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  int xres,yres,xo,yo;
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  kgGetRootPos(&Xpos,&Ypos);
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabrowser2callback(int item,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  DateFont = item-1;
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabutnbox2callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  int r,g,b;
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  kgGetColor(NULL,300,300,&Dred,&Dgreen,&Dblue);
  kgDefineColor(DateColor,Dred,Dgreen,Dblue);
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabutnbox3callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  char flname[200],fltr[100]="*.wav";
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  flname[0]='\0';
  if(kgFolderBrowser(NULL,300,400,flname,fltr)==1) {
     char command[500];
     sprintf(command,"cp %s %-s/.kgclock/beep.wav",flname,getenv("HOME"));
     printf("%s\n",command);
     system(command);
  }

  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabutnbox4callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  char flname[200],fltr[100]="*.wav";
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  flname[0]='\0';
  if(kgFolderBrowser(NULL,300,400,flname,fltr)==1) {
     char command[500];
     sprintf(command,"cp %s %-s/.kgclock/phone.wav",flname,getenv("HOME"));
     printf("%s\n",command);
     system(command);
  }

  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabutnbox5callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  int r,g,b;
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  kgGetColor(NULL,300,300,&Hred,&Hgreen,&Hblue);
  kgDefineColor(HarmColor,Hred,Hgreen,Hblue);
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabutnbox6callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  int r,g,b;
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  kgGetColor(NULL,300,300,&Mred,&Mgreen,&Mblue);
  kgDefineColor(MarmColor,Mred,Mgreen,Mblue);
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabutnbox7callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  int r,g,b;
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  kgGetColor(NULL,300,300,&Sred,&Sgreen,&Sblue);
  kgDefineColor(SarmColor,Sred,Sgreen,Sblue);
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabutnbox8callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  kgGetColor(NULL,300,300,&Tred,&Tgreen,&Tblue);
  kgDefineColor(DotColor,Tred,Tgreen,Tblue);
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int ConfigDiainit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  return ret;
}
int ConfigDiaCallBack(void *Tmp,void *tmp) {
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int  ConfigDiabrowser3callback(int item,int i,void *Tmp) {
  ThumbNail **th;
  DICH *C;DIALOG *D;void *pt; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  C = (DICH *)kgGetWidget(Tmp,i);
  th = (ThumbNail **)C->list;
  Shadow= th[0]->sw;
  ShDate =th[1]->sw;
  Nosecarm = (th[2]->sw+1)%2;
//  printf("Shadow:ShDate:Nosecarm: %d %d %d\n",Shadow,ShDate,Nosecarm);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  ConfigDiabrowser3init(DICH *C,void *pt) {
  ThumbNail **th;
  th = (ThumbNail **)C->list;
//  printf("Shadow:ShDate:Nosecarm: %d %d %d\n",Shadow,ShDate,Nosecarm);
  th[0]->sw= Shadow;
  th[1]->sw= ShDate;
  th[2]->sw= (Nosecarm+1)%2;
}
#if 0
#else
int ConfigDia( void *parent,void **v,void *pt) {
  int ret=1;
  DIALOG D;
  DIA d[15];
  char *menu0[]  = { 
    (char *)"Type1",
    (char *)"Type2",
    (char *)"Type3",
    (char *)"Type4",
    (char *)"Type5",
    (char *)"Type6",
    (char *)"Type7",
    (char *)"Type8",
    (char *)"Type9",
    (char *)"Type10",
    (char *)"Type11",
    (char *)"Type12",
    (char *)"Type13",
    NULL 
  };
  DIW w0 = { 
    'w',
    141,50,  
    347,78,   
    8,  
    (int *)v[0],
    (char *)"Clock Type" ,
    menu0 ,
    NULL,NULL, /* *args, callback */
    0 
  };
  DIF f1 = { 
    'f',
    209,85,  
    353,132,   
    0.250000,1.000000,  
    100,  
    (double *)v[1],
//    (char *)"Clock Size" ,
    (char *)"" ,
    NULL,ConfigDiafslide1callback /* *args, callback */
  };
  int butn1 =1;
  int sw2[1]  = { 
    1  
  };
  char *titles2[]  = { 
    (char *)" Okay", 
    NULL
  };
  void *xpm2[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr2[]  = { 
    -1, 
    -1
  };
  char *butncode2  = (char*)" ";
  DILN H2 = { 
    'H',
    346,415,  
    435,448,
    4,0,
    80, 
    25, 
    1,1, 
    &butn1, 
    sw2,
    titles2,
    butncode2,
    NULL,ConfigDiahoribar1callback,/* arg, Callbak */
    xpm2,bkgr2, /* pointers to xpms and colors */
     4,0.50000 /* button type and roundinfg factor(0-0.5) */
  };
  char *titles3[]  = { 
    (char *)"Select Clock Position", 
    NULL
  };
  void *xpm3[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr3[]  = { 
    -1, 
    -1
  };
  char *butncode3  = NULL;
  int sw3[]={1,1,1};
  DIB n3 = { 
    'b',
    164,13,  
    323,44,
    0,0,  
    154, 
    25, 
    1,1, 
    (int *)v[2], 
    sw3,
    titles3,
    butncode3,
    NULL,ConfigDiabutnbox1callback, /* args, Callbak */
    xpm3,bkgr3, /* pointers to xpms and colors */
      4,0.500000 /* button type and roundinfg factor(0-0.5) */
  };
  char *menu4[]  = { 
    (char *)"!f1800 !f00 Times-Roman",
    (char *)"!f1801 !f01 Times-Italic",
    (char *)"!f1802 !f02 Times-Bold",
    (char *)"!f1803 !f03 Times-BoldItalic",
    (char *)"!f1804 !f04 Bookman-Light",
    (char *)"!f1805 !f05 Bookman-LightItalic",
    (char *)"!f1806 !f06 Bookman-Demi",
    (char *)"!f1807 !f07 Bookman-DemiItalic",
    (char *)"!f1808 !f08 Helvetica",
    (char *)"!f1809 !f09 Helvetica-Oblique",
    (char *)"!f1810 !f10 Helvetica-Bold",
    (char *)"!f1811 !f11 Helvetica-BoldOblique",
    (char *)"!f1812 !f12 Helvetica-Narrow",
    (char *)"!f1813 !f13 Helvetica-Narrow-Oblique",
    (char *)"!f1814 !f14 Helvetica-Narrow-Bold",
    (char *)"!f1815 !f15 Helvetica-Narrow-BoldOblique",
    (char *)"!f1816 !f16 Courier",
    (char *)"!f1817 !f17 Courier-Oblique",
    (char *)"!f1818 !f18 Courier-Bold",
    (char *)"!f1819 !f19 Courier-BoldOblique",
    (char *)"!f1820 !f20 Charter-Roman",
    (char *)"!f1821 !f21 Charter-Italic",
    (char *)"!f1822 !f22 Charter-Bold",
    (char *)"!f1823 !f23 Charter-BoldItalic",
    (char *)"!f1824 !f24 Palatino-Roman",
    (char *)"!f1825 !f25 Palatino-Italic",
    (char *)"!f1826 !f26 Palatino-Bold",
    (char *)"!f1827 !f27 Palatino-BoldItalic",
    (char *)"!f1828 !f28 NewCenturySchlbk-Roman",
    (char *)"!f1829 !f29 NewCenturySchlbk-Italic",
    (char *)"!f1830 !f30 NewCenturySchlbk-Bold",
    (char *)"!f1831 !f31 NewCenturySchlbk-BoldItalic",
    (char *)"!f1832 !f32 AvantGarde-BookOblique",
    (char *)"!f1833 !f33 AvantGarde-Demi",
    (char *)"!f1834 !f34 ZapfChancery-MediumItalic",
    (char *)"!f1835 !f35 ZapfDingbats",
    (char *)"!f1836 !f36 Symbol",
    NULL 
  };
  DIW w4 = { 
    'w',
    59,185,  
    420,212,   
    8,  
    (int *)v[3],
    (char *)"Date Font" ,
    menu4 ,
    NULL,ConfigDiabrowser2callback, /* *args, callback */
    0 
  };
  char *titles5[]  = { 
    (char *)"Date Color", 
    NULL
  };
  void *xpm5[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr5[]  = { 
    75, 
    -1
  };
  char *butncode5  = NULL;
  DIB n5 = { 
    'b',
    129,305,  
    235,336,
    0,0,  
    100, 
    25, 
    1,1, 
    (int *)v[4], 
    sw3,
    titles5,
    butncode5,
    NULL,ConfigDiabutnbox2callback, /* args, Callbak */
    xpm5,bkgr5, /* pointers to xpms and colors */
      2,0.250000 /* button type and roundinfg factor(0-0.5) */
  };
  char *titles6[]  = { 
    (char *)"!f35) !f23Beep Sound", 
    NULL
  };
  void *xpm6[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr6[]  = { 
    -1, 
    -1
  };
  char *butncode6  = NULL;
  DIB n6 = { 
    'b',
    194,219,  
    300,250,
    0,0,  
    100, 
    25, 
    1,1, 
    (int *)v[5], 
    sw3,
    titles6,
    butncode6,
    NULL,ConfigDiabutnbox3callback, /* args, Callbak */
    xpm6,bkgr6, /* pointers to xpms and colors */
      2,0.250000 /* button type and roundinfg factor(0-0.5) */
  };
  char *titles7[]  = { 
    (char *)"!f35% !f23Ring Sound", 
    NULL
  };
  void *xpm7[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr7[]  = { 
    -1, 
    -1
  };
  char *butncode7  = NULL;
  DIB n7 = { 
    'b',
    194,256,  
    300,287,
    0,0,  
    100, 
    25, 
    1,1, 
    (int *)v[6], 
    sw3,
    titles7,
    butncode7,
    NULL,ConfigDiabutnbox4callback, /* args, Callbak */
    xpm7,bkgr7, /* pointers to xpms and colors */
      2,0.250000 /* button type and roundinfg factor(0-0.5) */
  };
  char *titles8[]  = { 
    (char *)"!c15Hour Arm Color", 
    NULL
  };
  void *xpm8[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr8[]  = { 
    76, 
    -1
  };
  char *butncode8  = NULL;
  DIB n8 = { 
    'b',
    69,350,  
    175,381,
    0,0,  
    100, 
    25, 
    1,1, 
    (int *)v[7], 
    sw3,
    titles8,
    butncode8,
    NULL,ConfigDiabutnbox5callback, /* args, Callbak */
    xpm8,bkgr8, /* pointers to xpms and colors */
      2,0.250000 /* button type and roundinfg factor(0-0.5) */
  };
  char *titles9[]  = { 
    (char *)"!c15Minute Arm Color", 
    NULL
  };
  void *xpm9[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr9[]  = { 
    77, 
    -1
  };
  char *butncode9  = NULL;
  DIB n9 = { 
    'b',
    194,350,  
    300,381,
    0,0,  
    100, 
    25, 
    1,1, 
    (int *)v[8], 
    sw3,
    titles9,
    butncode9,
    NULL,ConfigDiabutnbox6callback, /* args, Callbak */
    xpm9,bkgr9, /* pointers to xpms and colors */
      2,0.250000 /* button type and roundinfg factor(0-0.5) */
  };
  char *titles10[]  = { 
    (char *)"!c15Second Arm Color", 
    NULL
  };
  void *xpm10[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr10[]  = { 
    78, 
    -1
  };
  char *butncode10  = NULL;
  DIB n10 = { 
    'b',
    324,350,  
    430,381,
    0,0,  
    100, 
    25, 
    1,1, 
    (int *)v[9], 
    sw3,
    titles10,
    butncode10,
    NULL,ConfigDiabutnbox7callback, /* args, Callbak */
    xpm10,bkgr10, /* pointers to xpms and colors */
      2,0.250000 /* button type and roundinfg factor(0-0.5) */
  };
  char *titles11[]  = { 
    (char *)"Dot Color", 
    NULL
  };
  void *xpm11[]  = { 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr11[]  = { 
    79, 
    -1
  };
  char *butncode11  = NULL;
  DIB n11 = { 
    'b',
    269,305,  
    375,336,
    0,0,  
    100, 
    25, 
    1,1, 
    (int *)v[10], 
    sw3,
    titles11,
    butncode11,
    NULL,ConfigDiabutnbox8callback, /* args, Callbak */
    xpm11,bkgr11, /* pointers to xpms and colors */
      2,0.250000 /* button type and roundinfg factor(0-0.5) */
  };

  DIM m12 = { 
    'm',
    144,105,  
    202,127,  
    1,0
  };
  strcpy(m12.msg,(char *)"Clock Size");
  m12.hide=0;
  char *menu13[]  = { 
    (char *)"Draw Shadow",
    (char *)"Show Date",
    (char *)"Seconds Arm",
    NULL 
  };
  ThumbNail **th0 ;
  DICH c13 = { 
    'c',
    60,140,  
    442,173,   
//    99,0,  
//    109,0,  
    115,0,  
    0, 
    25, 
    1,3, 
    -2960686,1, 
    (int *)v[11], 
    NULL, 
    NULL, 
    NULL,ConfigDiabrowser3callback, /* *args, callback */
    4,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu13);
  c13.list=(void **)th0;
  d[0].w = &w0;
  d[1].f = &f1;
  d[2].H = &H2;
  d[3].b = &n3;
  d[4].w = &w4;
  d[5].b = &n5;
  d[6].b = &n6;
  d[7].b = &n7;
  d[8].b = &n8;
  d[9].b = &n9;
  d[10].b = &n10;
  d[11].b = &n11;
  d[12].m = &m12;
  d[13].c = &c13;
  ConfigDiabrowser3init(&c13,pt) ;
  d[14].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 13;
  D.tw = 4;
  D.bw = 46;
  D.lw = 4;
  D.rw = 4;
  D.xo = 370;   /* Position of Dialog */ 
  D.yo = 95;
  D.xl = 457;    /*  Length of Dialog */
  D.yl = 478;    /*  Width  of Dialog */
  D.Initfun = ConfigDiainit;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.150000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize=0;
#if 0 
  D.Callback = ConfigDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
  strcpy(D.name,"Clock Settings");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
  }    /*  end of fullscreen mode */
  D.SearchList=(void *)Dopen();    /*  list of directories picture search */
  if(D.parent != NULL) {
    D.gc = ((DIALOG *)D.parent)->gc;
  }
  else kgColorTheme(&D,Red,Green,Blue);
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyConfigDiaGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgFreeDouble((void **)th0);
  Dempty((Dlink *)D.SearchList);
//  insert any memory frees here to avoid leak  
  return ret;
}
void *RunConfigDia(void *arg) {
/*************************************************

    Browser1  1 data value
    Floatslidebar1  1 data value
    Buttonbox1 (new) 1 data value
    Browser2  1 data value
    Buttonbox2 (new) 1 data value
    Buttonbox3 (new) 1 data value
    Buttonbox4 (new) 1 data value
    Buttonbox5 (new) 1 data value
    Buttonbox6 (new) 1 data value
    Buttonbox7 (new) 1 data value
    Buttonbox8 (new) 1 data value
    CheckBox3  1 data value

*************************************************/
   int   v0 = 10;
   double v1 = 0.0;
   int   v2 = 1;
   int   v3 = 1;
   int   v4 = 1;
   int   v5 = 1;
   int   v6 = 1;
   int   v7 = 1;
   int   v8 = 1;
   int   v9 = 1;
   int   v10 = 1;
   int   v11 = 1;
   void* v[12];
   char flname[300];
   DIR *dir;
   FILE *fp;
   v0=Type+1;
   v1= scalefac;
   v3 = DateFont+1;

   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   v[5]=(void *)(&v5);
   v[6]=(void *)(&v6);
   v[7]=(void *)(&v7);
   v[8]=(void *)(&v8);
   v[9]=(void *)(&v9);
   v[10]=(void *)(&v10);
   v[11]=(void *)(&v11);

   void *pt=NULL; /* pointer to send any extra information */
   ConfigDia(NULL,v,pt );
   Type = v0-1;
   scalefac=v1;
   strcpy(flname,getenv("HOME"));
   strcat(flname,"/.kgclock");
   dir = opendir(flname);
   if(dir == NULL) mkdir(flname,0700);
   else closedir(dir);
//   mkdir(flname,0700);
   strcat(flname,"/Config");
   fp = fopen(flname,"w");
   if(fp != NULL) {
    fprintf(fp,"%d %d %d %f\n",Type,Shadow,Nosecarm,scalefac);
    fprintf(fp,"%d %d %d %d\n",ShDate,Xpos,Ypos,DateFont);
    fprintf(fp,"%d %d %d \n",Dred,Dgreen,Dblue);
    fprintf(fp,"%d %d %d \n",Hred,Hgreen,Hblue);
    fprintf(fp,"%d %d %d \n",Mred,Mgreen,Mblue);
    fprintf(fp,"%d %d %d \n",Sred,Sgreen,Sblue);
    fprintf(fp,"%d %d %d \n",Tred,Tgreen,Tblue);
    fclose(fp);
   }
   return NULL;
}
#endif 
void InstallClock(char *kgclock) {
   DIR *dir;
   char flname[300];
   char *pgr;
   FILE *fp;
#if 1
  if(kgCheckTrueColor()==0) {
     if(!kgCheckMenu(NULL,100,200,(char *)"No TureColor Support for Display/Driver: Continue?",0)) exit(0);
  }
#endif
   RunInstallDia(NULL);
   if(kgCheckMenu(NULL,100,200,(char *)"Install a AUTOSTART entry for !f34!z43kgclock?",0)){
     strcpy(flname,getenv("HOME"));
     strcat(flname,"/.config");
     dir = opendir(flname);
     if(dir == NULL) mkdir(flname,0700);
     else closedir(dir);
     strcat(flname,"/autostart");
     dir = opendir(flname);
     if(dir == NULL) mkdir(flname,0700);
     else closedir(dir);
     strcat(flname,"/kgclock.desktop");
     fp = fopen(flname,"w");
     if(fp != NULL) {
       fprintf(fp,"[Desktop Entry]\n");
       pgr = kgWhich((char *)"kgclock");
       fprintf(fp,"Exec=%-s\n",pgr);
       free(pgr);
       fprintf(fp,"X-DCOP-ServiceType=wait\n");
       fprintf(fp,"Name=Desktop Clock\n");
       fprintf(fp,"Type=Service\nX-KDE-StartupNotify=false\nX-KDE-autostart-phase=1\n");
       fclose(fp);
     }
   }
}
int KillOtherJobs(char *name) {
   char buff[300];
   int Id=0,Okay=0,Pid,ln,i;
   char tty[50],dummy[50],program[100];
   FILE *pp;
   char *pt;
   ln =strlen(name);
   while(name[ln]<=' ')name[ln--]='\0';
   while((ln>=0)&&(name[ln]!='/')) ln--;
   pt=name+ln+1;
   Pid = getpid();
   while(!Okay) {
     pp = popen("ps -e","r");
     if(pp==NULL) return 0;
     fgets(buff,299,pp);
     Okay =1;
     fgets(buff,299,pp);
     while( fgets(buff,299,pp) != NULL) {
        sscanf(buff,"%d%s%s%s",&Id,tty,dummy,program);
        if(strcmp(pt,program)==0) {
             if(Id==Pid) continue;
#if 1
             kill(Id,9);
             Okay =0;
#else
             exit(0);
#endif
        }
     }
     pclose(pp);
   }
   return Id;
}
int main(int argc,char **argv) {
  char *pt;
  int pid,ln;
  pthread_t Pth;
  FILE *fp;
  DIR *dir;
  char flname[300];
  
  KillOtherJobs(argv[0]);
  strcpy(flname,getenv("HOME"));
  strcat(flname,"/.kgclock");
  dir = opendir(flname);
  if(dir == NULL){
    mkdir(flname,0700);
    InstallClock(argv[0]);
  }
  else closedir(dir);
  strcat(flname,"/Config");
  fp = fopen(flname,"r");
  if(fp != NULL) {
    fscanf(fp,"%d%d%d%f",&Type,&Shadow,&Nosecarm,&scalefac);
    fscanf(fp,"%d%d%d%d",&ShDate,&Xpos,&Ypos,&DateFont);
    fscanf(fp,"%d%d%d",&Dred,&Dgreen,&Dblue);
    fscanf(fp,"%d%d%d",&Hred,&Hgreen,&Hblue);
    fscanf(fp,"%d%d%d",&Mred,&Mgreen,&Mblue);
    fscanf(fp,"%d%d%d",&Sred,&Sgreen,&Sblue);
    fscanf(fp,"%d%d%d",&Tred,&Tgreen,&Tblue);
    if(scalefac < 0.2 ) scalefac=0.2;
    if(Nosecarm) TIMERESO=NOSECARMRESO;
    else TIMERESO=1;
    fclose(fp);
  }
  if(argc>1) {
    int count =1;
    while(count < argc) {
      pt = argv[count];
      if(argv[count][0]!='-') { count++;continue;}
      switch(argv[count][1]) {
         case 's':
           if(pt[2]!='\0') sscanf(pt+2,"%f",&scalefac);
           else { count++; sscanf(argv[count],"%f",&scalefac);}
           if(scalefac <=0.0 ) scalefac=1.0;
           if(scalefac > 1.0)  scalefac=1.0;
           Soff = (Soff*scalefac)+0.5;
           count++;
           break;
         case 'n':
           if(pt[2]!='\0') sscanf(pt+2,"%d",&Nosecarm);
           else { count++; sscanf(argv[count],"%d",&Nosecarm);}
           count++;
           if( Nosecarm==1) TIMERESO=NOSECARMRESO;
           else TIMERESO=1;
           break;
         case 'w':
           if(pt[2]!='\0') sscanf(pt+2,"%d",&Shadow);
           else { count++; sscanf(argv[count],"%d",&Shadow);}
           count++;
           break;
         case 'd':
           if(pt[2]!='\0') sscanf(pt+2,"%d",&Daemon);
           else { count++; sscanf(argv[count],"%d",&Daemon);}
           count++;
           break;
         case 't':
           if(pt[2]!='\0') sscanf(pt+2,"%d",&Type);
           else { count++; sscanf(argv[count],"%d",&Type);}
           count++;
           break;
         case 'h':
           fprintf(stderr,"Usage: clock [-s <scalefac>] [-n <0/1>]  [-d <1/0>] [-t <type>]\n");
           fprintf(stderr,"       scalefac scale factor for clock size >0.0 and <=1.0 default:1.0\n");
           fprintf(stderr,"       -n for no sec arm\n");
           fprintf(stderr,"       -w <0/1) default 0:for Shadow of arms\n");
           fprintf(stderr,"       -d <1/0> for daemonising default 1\n");
           fprintf(stderr,"       -t clock type int val 0(default),1,2,3...\n");
           fprintf(stderr,"Note : It will take some time for the display to come fully up\n");
           return 1;
         default:
           break;
      }
    }

  }
  if(Daemon) daemon(0,0);
  while (1) {
    if( (pid=fork())==0) {
     kgDefineColor(DateColor,Dred,Dgreen,Dblue);
     kgDefineColor(HarmColor,Hred,Hgreen,Hblue);
     kgDefineColor(MarmColor,Mred,Mgreen,Mblue);
     kgDefineColor(SarmColor,Sred,Sgreen,Sblue);
     kgDefineColor(DotColor ,Tred,Tgreen,Tblue);
     kgDefineColor(ShadowColor,60,60,60);
     notify(NULL);
     Runclock();
     Writedb();
     if(ShDate==1) {
       pthread_cancel(Dth);
       pthread_join(Dth,NULL);
     }
     exit(RESTART);
   }
   else {
     int status;
     waitpid(pid,&status,0);
//     printf("status= %d\n",status);
     if(status == 0) break;
     fp = fopen(flname,"r");
     if(fp != NULL) {
       fscanf(fp,"%d%d%d%f",&Type,&Shadow,&Nosecarm,&scalefac);
       fscanf(fp,"%d%d%d%d",&ShDate,&Xpos,&Ypos,&DateFont);
       fscanf(fp,"%d%d%d",&Dred,&Dgreen,&Dblue);
       fscanf(fp,"%d%d%d",&Hred,&Hgreen,&Hblue);
       fscanf(fp,"%d%d%d",&Mred,&Mgreen,&Mblue);
       fscanf(fp,"%d%d%d",&Sred,&Sgreen,&Sblue);
       fscanf(fp,"%d%d%d",&Tred,&Tgreen,&Tblue);
       if(scalefac < 0.2 ) scalefac=0.2;
       if( Nosecarm==1) TIMERESO=NOSECARMRESO;
       else TIMERESO=1;
       fclose(fp);
     }
   }
  }
}
