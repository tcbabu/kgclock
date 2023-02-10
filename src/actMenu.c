#include <kulina.h>
#include "actMenuCallbacks.h"
int actMenu( void *parent ,void *v0 ) {
  int ret=1;
  DIALOG D;
  DIA d[2];
  char *menu0[]  = { 
    (char *)"Config",
    (char *)"Setalert",
    (char *)"Close",
    (char *)"About",
    NULL 
  };
  DIE e0 = { 
    'e',
    -10,-10,  
    106,126,   
    4,  
    (int *)v0,
    NULL,
    menu0 ,
    NULL,NULL
    ,20,4,22,1,1,1,0
  };
  d[0].e = &e0;
  d[1].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 0;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 81;    /*  Length of Dialog */
  D.yl = 111;    /*  Width  of Dialog */
  D.Initfun = NULL;
  D.kbattn = 0;;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 0.340000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 0;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
#if 0 
  D.Callback = actMenuCallBack;    /*  default callback */
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
int RunactMenu(void *arg) {
/*************************************************

    Scrollmenu1  1 data value

*************************************************/
   int   v0 = 1;
   actMenu(NULL ,&v0 );
   return v0;
}
