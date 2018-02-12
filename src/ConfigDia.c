int  ConfigDiabrowser1callback(int key,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabrowser2callback(int key,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabrowser3callback(int key,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabrowser4callback(int key,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  ConfigDiabrowser5callback(int key,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
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
int ConfigDia( void *parent ,void *v0 ,void *v1 ,void *v2 ,void *v3 
 ,void *v4 ,void *v5 ) {
  int ret=1;
  DIALOG D;
  DIA d[8];
  char *menu0[]  = { 
    (char *)"Type1",
    (char *)"Type2_______",
    (char *)"Type3",
    (char *)"Type4",
    (char *)"Type6",
    (char *)"Type7",
    (char *)"Type8",
    (char *)"Type9",
    NULL 
  };
  DIW w0 = { 
    'w',
    58,31,  
    315,60,   
    4,  
    (int *)v0,
    (char *)"Clock Type" ,
    menu0 ,
    NULL,ConfigDiabrowser1callback,0 /* *args, callback */
  };
  char *menu1[]  = { 
    (char *)"Type 0",
    (char *)"Type 1",
    (char *)"Type 2",
    NULL 
  };
  DIW w1 = { 
    'w',
    112,66,  
    315,95,   
    3,  
    (int *)v1,
    (char *)"Arm   Type" ,
    menu1 ,
    NULL,ConfigDiabrowser2callback,0 /* *args, callback */
  };
  char *menu2[]  = { 
    (char *)"No",
    (char *)"Yes",
    NULL 
  };
  DIW w2 = { 
    'w',
    175,101,  
    315,130,   
    2,  
    (int *)v2,
    (char *)"Shadow" ,
    menu2 ,
    NULL,ConfigDiabrowser3callback,0 /* *args, callback */
  };
  char *menu3[]  = { 
    (char *)"No",
    (char *)"Yes",
    NULL 
  };
  DIW w3 = { 
    'w',
    121,136,  
    315,165,   
    2,  
    (int *)v3,
    (char *)"Colored Arms" ,
    menu3 ,
    NULL,ConfigDiabrowser4callback,0 /* *args, callback */
  };
  char *menu4[]  = { 
    (char *)"No",
    (char *)"Yes",
    NULL 
  };
  DIW w4 = { 
    'w',
    130,171,  
    315,200,   
    2,  
    (int *)v4,
    (char *)"Seconds Arm" ,
    menu4 ,
    NULL,ConfigDiabrowser5callback,0 /* *args, callback */
  };
  DIF f5 = { 
    'f',
    80,206,  
    315,252,   
    0.000000,1.000000,  
    100,  
    (double *)v5,
    (char *)"Scale Fac" ,
    NULL,ConfigDiafslide1callback /* *args, callback */
  };
  int butn1 =1;
  int sw6[1]  = { 
    1  
  };
  char *titles6[]  = { 
    (char *)" Okay", 
    NULL
  };
  void *xpm6[]  = { 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL
  };
  int bkgr6[]  = { 
    -1, 
    -1
  };
  char *butncode6  = ""; 
  DILN H6 = { 
    'H',
    166,286,  
    255,319,
    4,0,
    80, 
    25, 
    1,1, 
    &butn1, 
    sw6,
    titles6,
    butncode6,
    NULL,ConfigDiahoribar1callback,/* arg, Callbak */
    xpm6,bkgr6, /* pointers to xpms and colors */
     3,1.000000 /* button type and roundinfg factor(0-0.5) */
  };
  d[0].w = &w0;
  d[1].w = &w1;
  d[2].w = &w2;
  d[3].w = &w3;
  d[4].w = &w4;
  d[5].f = &f5;
  d[6].H = &H6;
  d[7].t = NULL;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 6;
  D.tw = 4;
  D.bw = 46;
  D.lw = 4;
  D.rw = 4;
  D.xo = 370;   /* Position of Dialog */ 
  D.yo = 95;
  D.xl = 373;    /*  Length of Dialog */
  D.yl = 324;    /*  Width  of Dialog */
  D.Initfun = ConfigDiainit;    /*   init fuction for Dialog */
  D.kbattn = 0;;    /*  1 for drawing keyborad attention */
  D.butattn = 1;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
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
  D.pt = NULL;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina Clock Config");    /*  Dialog name you may change */
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
  kgDefaultGuiTheme(&(D.gc));    /*  set colors for gui*/
//  kgGrayGuiTheme(&(D.gc));    /*  set colors for gui*/
  ModifyConfigDiaGc(&(D.gc));    /*  set colors for gui*/
  D.SearchList=NULL;
  ret= kgUi(&D);
  return ret;
}
void *RunConfigDia(void *arg) {
/*************************************************

    Browser1  1 data value
    Browser2  1 data value
    Browser3  1 data value
    Browser4  1 data value
    Browser5  1 data value
    Floatslidebar1  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   int   v2 = 1;
   int   v3 = 1;
   int   v4 = 1;
   double v5 = 0.0;
   FILE *fp;
   char flname[300];
   v5= scalefac;
   v0 = Type+1;
   v1 = Armtype+1;
   v2 = Shadow+1;
   v3 = Color+1;
   v4 = (Nosecarm+1)%2
   ConfigDia(NULL ,&v0 ,&v1 ,&v2 ,&v3 
     ,&v4 ,&v5 );
   scalefac =v5;
   Type = v0-1;
   Armtype = v1-1;
   Shadow  = v2-1;
   Color = v3 - 1;
   Nosecarm =v4%2;
   strcpy(flname,getenv("HOME"));
   strcat(flname,"/.kgclock");
   mkdir(flname);
   strcat(flname,"/Config");
   fp = fopen(flname,"w");
   if(fp != NULL) {
    fprintf(fp,"%d %d %d %d %d %f",Type,ArmType,Shadow,Color,Nosecarm,scalefac);
    fclose(fp);
   }
   return NULL;
}
