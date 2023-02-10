#include <kulina.h>
extern int RESTART;
extern unsigned int Red,Green,Blue;
extern int Dred,Dgreen,Dblue;
extern int XM,YM,XL,YL;
static int RetVal=0;
int doEvents(int item);
void *MakePixmapImage(int xl,int yl,int r,int g,int b,float fac);
char *xpm0= NULL;
int  clockMenubrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIE *E;void *pt; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  E = (DIE *)kgGetWidget(Tmp,i);
  RetVal=item;
  RESTART=doEvents(item);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  clockMenubrowser1init(DIE *E,void *pt) {
}
int clockMenuinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int clockMenucleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
// Freeing is not needed as it done inside ar cleanup.
//  pt = D->pt;
//  if(pt!= NULL) kgFreeImage(pt);
//  D->pt=NULL;
//  kgFreeImage(xpm0);
//  printf("Free Image okay\n");

  return ret;
}
int clockMenuCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
      ret=1;
    }
  }
  return ret;
}
int clockMenuResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
int clockMenuWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
void ModifyclockMenuGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int clockMenuGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0=  (char *)MakePixmapImage(75,93,Red,Green,Blue,0.1);
  DIP p0 = { 
    'p',
    42,27,  
    118,121,  
    (void *)xpm0,
    -1, /* bkgr colour */ 
      0,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p0.Wid,(char *)"clockMenuWidget2");
  p0.item = -1;
  char **menu1 ; 
  menu1= (char **)malloc(sizeof(char *)*5);
  menu1[4]=NULL;
  menu1[0]=(char *)malloc(13);
  strcpy(menu1[0],(char *)"!c10Settings");
  menu1[1]=(char *)malloc(11);
  strcpy(menu1[1],(char *)"!c10Alerts");
  menu1[2]=(char *)malloc(15);
  strcpy(menu1[2],(char *)"!c03Stop Clock");
  menu1[3]=(char *)malloc(10);
  strcpy(menu1[3],(char *)"!c10About");
  DIE e1 = { 
    'e',
    37,28,  
    111,120,   
    4,  
    (int *)v[0],
    NULL,
    menu1 ,
    NULL,clockMenubrowser1callback, /* *args, callback */
    20,4,22,1,0,0,0 
  };
  strcpy(e1.Wid,(char *)"");
  e1.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+3));
  d =dtmp+i; 
  d[2].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIE));
  *d[1].e = e1;
  d[1].e->item = -1;
  d[2].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeclockMenuGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Scrollmenu1  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = clockMenuGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int clockMenu( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = clockMenuGroup(&D,v,pt);
  d = D.d;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 1733;   /* Position of Dialog */ 
  D.yo = 29;
  D.xl = 149;    /*  Length of Dialog */
  D.yl = 149;    /*  Width  of Dialog */
  D.xl = 100;    /*  Length of Dialog */
  D.yl = 100;    /*  Width  of Dialog */
  if(D.xl < XL) D.xl =XL;
  if(D.yl < YL) D.yl =YL;
  D.xo = XM- D.xl/2;
  D.yo = YM- D.yl/2;
  D.Initfun = clockMenuinit;    /*   init fuction for Dialog */
  D.Cleanupfun = clockMenucleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 1.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 0;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 1;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = clockMenuCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = clockMenuResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = clockMenuWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
  D.StackPos = -1;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = d[0].p;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres;
     int xl,yl;
     DIP *p0;
     DIE *e0;
     p0= d[0].p;
     e0 = d[1].e;
     kgDisplaySize(&xres,&yres);
     xl = p0->x2-p0->x1+1;
     yl = p0->y2-p0->y1+1;
     p0->x1 = (D.xl -xl)/2;
     p0->y1 = (D.yl -yl)/2;
     p0->x2 = p0->x1 +(xl-1);
     p0->y2 = p0->y1 +(yl-1);
     xl = e0->x2-e0->x1;
     yl = e0->y2-e0->y1;
     e0->x1 = p0->x1+1;
     e0->y1 = p0->y1+1;
     e0->x2 = e0->x1 +xl;
     e0->y2 = e0->y1 +yl;

  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyclockMenuGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int RunclockMenu(void *arg) {
/*************************************************

    Scrollmenu1  1 data value

*************************************************/
   int   v0 = 1;
   void* v[1];
   v[0]=(void *)(&v0);
   void *pt=NULL; /* pointer to send any extra information */
   int ret;
   RetVal =1;
   ret = clockMenu(NULL,v,pt );
//   printf("clockMenu over\n");
//   fflush(stdout);
   return RESTART;
}
