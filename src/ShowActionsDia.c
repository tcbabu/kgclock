#include <kulina.h>
int ShowActionsDia( void *parent ,char **menu0) {
  int ret=1;
  DIALOG D;
  DIA d[3];
  DIS s0 = { 
    's',
    9,7,  
    368,207,   
    8,  
    NULL,
    NULL,
    menu0 ,
    NULL,NULL,22,6,22,1,1,1,0 /* *args, callback */
  };
  int butn1 =1;
  int sw1[1]  = { 
    1  
  };
  char *titles1[]  = { 
    " Okay", 
    NULL
  };
  void *xpm1[]  = { 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL
  };
  int bkgr1[]  = { 
    -1, 
    -1
  };
  char *butncode1  = NULL;
  DILN H1 = { 
    'H',
    141,301,  
    230,334,
    4,0,
    80, 
    25, 
    1,1, 
    &butn1, 
    sw1,
    titles1,
    butncode1,
    NULL,NULL,/* arg, Callbak */
    xpm1,bkgr1 /* pointers to xpms and colors */
  };
  d[0].s = &s0;
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
  D.xo = 137;   /* Position of Dialog */ 
  D.yo = 19;
  D.xl = 375;    /*  Length of Dialog */
  D.yl = 345;    /*  Width  of Dialog */
  D.Initfun = NULL;
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
#if 0 
  D.Callback = ShowActionsDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = NULL;    /*  any data to be passed by user*/
  D.StackPos=0;
  D.Shapexpm=NULL;
  strcpy(D.name,"Kulina Clock");    /*  Dialog name you may change */
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
void *RunShowActionsDia(void *arg,char **menu) {
   ShowActionsDia(arg,menu );
   return NULL;
}
