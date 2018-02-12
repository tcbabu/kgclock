#include "kulina.h"
#include <time.h>
#include  <sys/time.h>
#include <fcntl.h>
extern unsigned int Red,Green,Blue;
extern int XL,YL,XM,YM,XO,YO;
extern int DateFont,DateColor;
extern int Dred,Dgreen,Dblue;
int ShowDateDiainit(void *Tmp) {
  unsigned long Waitsecs=0;
  int ret = 0,i,j,OK,xl,yl ,count=0,k;
  DIG *fid;
  float ln,length=0,fac=1.0;
  float w_x1=0.0,w_y1=0.0,w_x2=686,w_y2=226,dyl,dxl,yy;
  float r,g,b, h, s, v,vorg;
  KBEVENT kbe;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  char *pt,buf[5000],flname[200],*str[500];;
  char *msg;
#if 0
  char *Month[]={(char *)"JAN",(char *)"FEB",(char *)"MAR",(char *)"APR",(char *)"MAY",(char *)"JUN",(char *)"JUL",(char *)"AUG",(char *)"SEP",(char *)"OCT",(char *)"NOV",(char *)"DEC"};
  char *Day[]=  {(char *)"SUN",(char *)"MON",(char *)"TUE",(char *)"WED",(char *)"THU",(char *)"FRI",(char *)"SAT"};
#else
#if 0
  char *Month[]={(char *)"Jan    ! ",(char *)"Feb    ! ",(char *)"Mar    ! ",(char *)"Apr    ! ",(char *)"May    ! ",(char *)"Jun    ! ",(char *)"Jul    ! ",(char *)"Aug    ! ",(char *)"Sep    ! ",(char *)"Oct    ! ",(char *)"Nov    ! ",(char *)"Dec    ! "};
#else
  char *Month[]={(char *)"January",(char *)"February",(char *)"March",(char *)"April  ! ",(char *)"May    ! ",(char *)"June   ! ",(char *)"July   ! ",(char *)"August ! ",(char *)"September",(char *)"October",(char *)"November",(char *)"December"};
#endif
  char *Day[]=  {(char *)"Sun",(char *)"Mon",(char *)"Tue",(char *)"Wed",(char *)"Thu",(char *)"Fri",(char *)"Sat"};
#endif
  time_t tp,t;
  struct tm *dt;
  void *img,*img1,*img2;
  xl = D->xl;
  yl = D->yl;
  w_x2 = xl;
  w_y2 = yl;
  kgDropFocus(D);
  while(1) {
    t=time(&tp);
    dt = localtime(&t);
//    sprintf(buf,"!c15!f12!w81%-s\\n!c15!f12!w81!z43%2.2d %-s",Day[dt->tm_wday],dt->tm_mday,Month[dt->tm_mon]);
#if 0
    sprintf(buf,"%-s\\n!z43%2.2d %-s",Day[dt->tm_wday],dt->tm_mday,Month[dt->tm_mon]);
#else
    sprintf(buf,"!y!h32%2.2d!h23!k!x!x!x!h45%-s!r!z23%-s",dt->tm_mday,Day[dt->tm_wday],Month[dt->tm_mon]);
#endif
    Waitsecs=24*3600-(dt->tm_hour*3600+dt->tm_min*60+dt->tm_sec);
//    printf("Waitsecs :%ld\n",Waitsecs);
    msg = buf;
    fid = (DIG *)kgInitImage((int)xl,(int)yl,8);
    kgUserFrame(fid,w_x1,w_y1,w_x2,w_y2);
    RGBtoHSV(Dred,Dgreen,Dblue,&h,&s,&vorg);
    v =0.75*vorg;
    HSVtoRGB(&r,&g,&b,h,s,v);
    kgChangeColor(fid,251,30,30,30);
    kgChangeColor(fid,253,(int)r,(int)g,(int)b);
   
    if(vorg < 0.9 ) {
       v = vorg*1.4;
       if(v>1.0) v=1.0;
       HSVtoRGB(&r,&g,&b,h,s,v);
       kgChangeColor(fid,252,(int)r,(int)g,(int)b);
    }
    else kgChangeColor(fid,252,250,250,250);
    kgTextColor(fid,DateColor);
    kgTextFont(fid,DateFont);
    kgTextSize(fid,30.,25.,0.);
    pt=buf;
    j=1;
    k=0;
    while(1) {
      i=0;
      if(pt[i]=='\0') break;
//      gphMove2f(fid,40.,yl-j*45.);
      j++;
      OK=1;
      while(pt[i]!='\0') {
        OK=0;
        if((pt[i]=='\n')){ pt[i]='\0';break;}
        if(((pt[i]=='\\')&&(pt[i+1]=='n'))){ pt[i]='\0';i++;break;}
        i++;
        OK=1;
      }
      str[k++]=pt;
      ln = kgStringLength(fid,pt);
      if(length< ln)length=ln;
      if(OK) break;
      pt = pt+i+1;
    }
    if(k>0 ) {
      dyl = yl/k;
      fac = 0.9*xl/(length);
      dxl = fac*25;
      kgTextSize(fid,dyl*0.5,dxl,0.);
      yy= yl - dyl*0.7;
      kgTextColor(fid,251);
      for(i=0;i<k;i++) {
         length=kgStringLength(fid,str[i]);
//TCBTCB
         kgMove2f(fid,(xl-length)*0.5+2,yy-2);
         kgWriteText(fid,str[i]);
         yy -=dyl;
      }
#if 1
      yy= yl - dyl*0.7;
      kgTextColor(fid,252);
      for(i=0;i<k;i++) {
         length=kgStringLength(fid,str[i]);
         kgMove2f(fid,(xl-length)*0.5-1,yy);
         kgWriteText(fid,str[i]);
         yy -=dyl;
      }
#endif
      yy= yl - dyl*0.7;
      kgTextColor(fid,253);
      for(i=0;i<k;i++) {
         length=kgStringLength(fid,str[i]);
         kgMove2f(fid,(xl-length)*0.5+1,yy-1);
         kgWriteText(fid,str[i]);
         yy -=dyl;
      }
#if 1
      yy= yl - dyl*0.7;
      kgTextColor(fid,DateColor);
      for(i=0;i<k;i++) {
         length=kgStringLength(fid,str[i]);
         kgMove2f(fid,(xl-length)*0.5,yy);
         kgWriteText(fid,str[i]);
         yy -=dyl;
      }
#endif
    }
    img = kgGetResizedImage(fid);
    kgCloseImage(fid);
    kgCleanBackground(D,0,0,xl,yl,1.0);
    kgImage(D,img,0,0,xl,yl,0.0,1.0);
    kgFreeImage(img);

    kgUpdateOn(D);
    sleep(Waitsecs);
  }
  return ret;
}
int ShowDateDiaCallBack(void *Tmp,void *tmp) {
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
int ShowDateDia( void *parent ) {
  int ret=1;
  DIALOG D;
  DIA d[2];
  DIP p0 = { 
    'p',
    5,4,  
    398,122,  
(char *)"##" , // Pixmap info
    -1,0,0,0.0 /* bkgr colour */ 
  };
  d[0].p = &p0;
  d[1].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 0;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo   = XM-XL*1.50*0.5;
  D.xo   = XM-XL*0.5;
  D.yo =  YM+YL/2;
  D.xl = (float)XL*1.5;
  D.xl = (float)XL;
  D.yl = (float)YL*0.50;
 
  D.Initfun = ShowDateDiainit;    /*   init fuction for Dialog */
  D.kbattn = 0;;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 1.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 0;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 1;    /*  1 for stickyness */
  D.Resize =0;
#if 0 
  D.Callback = ShowDateDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
  D.StackPos = -1;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = NULL;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
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
  if(D.parent != NULL) {
    D.gc = ((DIALOG *)D.parent)->gc;
  }
  else kgColorTheme(&D,Red,Green,Blue);
//  kgColorTheme(&D,230,230,230);    /*  set colors for gui*/
//  ModifyShowDateDiaGc(&(D.gc));    /*  set colors for gui*/
  D.SearchList=NULL;
  ret= kgUi(&D);
  return ret;
}
void *RunShowDateDia(void *arg) {
/*************************************************


*************************************************/
   ShowDateDia(NULL );
   return NULL;
}
