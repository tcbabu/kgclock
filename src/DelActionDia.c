#include <kulina.h>
staic int delitem=0;
int  DelActionDiabrowser1callback(int key,int i,void *Tmp) {
  DIA *D;DIE *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].e;
  delitem=key-1;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  DelActionDiahoribar1callback(int key,int i,void *Tmp) {
  DIA *D;DILN *B; 
  int n,ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].H;
  n = B->nx;
  switch(key) {
    case 1: 
      break;
    case 2: 
      break;
  }
  return ret;
}
int DelActionDiainit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  return ret;
}
int DelActionDiaCallBack(void *Tmp,void *tmp) {
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
int DelActionDia( void *parent ,void *v0 ,char **menu0) {
  int ret=1;
  DIALOG D;
  DIA d[3];
  DIE e0 = { 
    'e',
    5,4,  
    409,260,   
    10,  
    v0,
    NULL,
    menu0 ,
    NULL,DelActionDiabrowser1callback /* *args, callback */
    ,20,4,22,1,1,1,0
  };
  int butn1 =1;
  int sw1[2]  = { 
    1, 
    1  
  };
  char *titles1[]  = { 
    " Okay", 
    " Cancel", 
    NULL
  };
  void *xpm1[]  = { 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL,
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL,
    NULL
  };
  int bkgr1[]  = { 
    -1, 
    -1, 
    -1
  };
  char *butncode1  = NULL;
  DILN H1 = { 
    'H',
    92,273,  
    305,306,
    4,0,
    100, 
    25, 
    2,1, 
    &butn1, 
    sw1,
    titles1,
    butncode1,
    NULL,DelActionDiahoribar1callback,/* arg, Callbak */
    xpm1,bkgr1 /* pointers to xpms and colors */
  };
  d[0].e = &e0;
  d[1].H = &H1;
  d[2].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 189;   /* Position of Dialog */ 
  D.yo = 56;
  D.xl = 414;    /*  Length of Dialog */
  D.yl = 319;    /*  Width  of Dialog */
  D.Initfun = DelActionDiainit;    /*   init fuction for Dialog */
  D.kbattn = 1;    /*  1 for drawing keyborad attention */
  D.butattn = 1;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
#if 0 
  D.Callback = DelActionDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = NULL;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     GetDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     GetDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
  }    /*  end of fullscreen mode */
//  kgDefaultGuiColors(&(D.gc));    /*  set colors for gui*/
  D.SearchList=NULL;
  ret= kgUi(&D);
  return ret;
}
void *RunDelActionDia(void *arg) {
/*************************************************

    Scrollmenu1  1 data value

*************************************************/
   int   v0 = 1;
   DelActionDia(NULL ,&v0 ,NULL);
   return NULL;
}
