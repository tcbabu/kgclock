#include "kulina.h"
#include "kulina.h"
#include "methan.c"
extern unsigned int Red,Green,Blue;
void * setnotify(void *arg);
int GetActionsCount(void);
void GetActionsDisplay(char **pt);
void DeleteAction(int item);
void DeleteActions(int *sw,char **menu);
static int delitem=0;
static char **pt,**amenu;
int  DelActionDiabrowser1callback(int key,int i,void *Tmp) {
  DIA *D;DIE *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].e;
  delitem=key-1;
//  printf("del item :%d :%d\n",delitem,Dgetmenuitem(Tmp,i));
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  DelActionDiahoribar1callback(int key,int i,void *Tmp) {
  char msg[60];
  ThumbNail **th;
  DIALOG *D;DILN *B; 
  DIX *X;
  int n,ret=0,*sw,count=0,k,Nitems,j; 
  char **menu,**pt;
  
  D = ((DIALOG *)Tmp);
  B = (DILN *)kgGetWidget(Tmp,i);
  n = B->nx;
  X = (DIX *)kgGetWidget(Tmp,0);
  th = (ThumbNail **)X->list;
  n=X->nitems;
  switch(key) {
    case 1: 
      for(k=0;k<n;k++) th[k]->sw=1;
      kgUpdateWidget(X);
      break;
    case 2: 
      for(k=0;k<n;k++) th[k]->sw=0;
      kgUpdateWidget(X);
      break;
    case 3: 
      for(k=0;k<n;k++) if(th[k]->sw==1) count++;
      if(count==0) return 0;
      sprintf(msg,"Delete the Selected %d Action(s) ?",count);
      if(kgCheckMenu(NULL,200,300,msg,0)){
        for(k=0;k<n;k++) if(th[k]->sw==1) DeleteAction(k);
//        DeleteActions(sw,menu);
        kgFreeThumbNails(th);
        Nitems=GetActionsCount();
        kgFreeDouble((void **)D->pt);
        pt = (char **)malloc(sizeof(char *)*(Nitems+1));
        for(j=0;j<Nitems;j++) pt[j]=(char *)malloc(50);
        pt[Nitems]=NULL;
        D->pt = (void *)pt;
        GetActionsDisplay(pt);
        th = kgStringToThumbNails(pt);
        X->list=(void **)th;
        kgUpdateWidget(X);
      }
      else return 0;
      break;
  }
  return ret;
}
int DelActionDiainit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  delitem=0;
  return ret;
}
int DelActionDiaCallBack(void *Tmp,void *tmp) {
  int ret = 0;
  int x1,y1,x2,y2;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
       x1 = D->xo; y1= D->yo;x2= x1+D->xl;y2= y1+D->yl;
       if( (kbe->x -x1)*(x2-kbe->x) < 0) ret=1;
       if( (kbe->y-y1)*(y2-kbe->y) < 0) ret=1;
//       printf("%d %d %d %d: %d\n",x1,y1,x2,y2,ret);
    }
  }
  if(ret) kgPushBackEvent(D);
  return ret;
}
int DelActionDia( void *parent ,void *v0 ,char **menu0) {
  int ret=1,*swx,n=0,i;
  void **xpm;
  DIALOG D;
  DIA d[3];
  DIX e0 = { 
    'x',
    5,4,  
    409,260,   
    8,0,
    370,24,
    1,24,
    1,10,  
    (int *)v0,
    NULL,
    NULL,
    NULL,DelActionDiabrowser1callback, /* *args, callback */
    10,22,0,1,1,1,0
  };
  int butn1 =1;
  int sw1[3]  = { 
    1, 
    1, 
    1  
  };
  char *titles1[]  = { 
    (char *)"Select All", 
    (char *)"Clear ", 
    (char *)"Delete", 
    NULL
  };
  void *xpm1[]  = { 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, 
    NULL, 
    NULL, 
    NULL, 
    NULL, 
    NULL, 
    NULL
  };
  int bkgr1[]  = { 
    -1, 
    -1, 
    -1, 
    -1
  };
  char *butncode1  = NULL;
  DILN H1 = { 
    'H',
    59,265,  
    373,298,
    4,0,
    100, 
    25, 
    3,1, 
    &butn1, 
    sw1,
    titles1,
    butncode1,
    NULL,DelActionDiahoribar1callback,/* arg, Callbak */
    xpm1,bkgr1, /* pointers to xpms and colors */
     5,0.300000 /* button type and roundinfg factor(0-0.5) */
  };
  e0.list = (void **)kgStringToThumbNails(menu0);
  kgInitUi(&D);
  d[0].x = &e0;
  d[1].H = &H1;
  d[2].t = NULL;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 40;   /* Position of Dialog */ 
  D.yo = 40;
  D.xl = 414;    /*  Length of Dialog */
  D.yl = 300;    /*  Width  of Dialog */
  D.Initfun = DelActionDiainit;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.15;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
#if 1 
  D.Callback = DelActionDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = (void *)menu0;    /*  any data to be passed by user*/
  D.StackPos=0;
  D.Shapexpm=NULL;
  D.Resize=0;
  strcpy(D.name,"Kulina Clock Actions");    /*  Dialog name you may change */
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
//  kgColorTheme(&D,230,230,230);
  if(D.parent != NULL) {
    D.gc = ((DIALOG *)D.parent)->gc;
  }
  else kgColorTheme(&D,Red,Green,Blue);
  D.SearchList=NULL;
  ret= kgUi(&D);
  kgFreeDouble((void **)(D.pt));
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
int ShowActionsDiaCallBack(void *Tmp,void *tmp) {
  int ret = 0;
  int x1,y1,x2,y2;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
       x1 = D->xo; y1= D->yo;x2= x1+D->xl;y2= y1+D->yl;
       if( (kbe->x -x1)*(x2-kbe->x) < 0) ret=1;
       if( (kbe->y-y1)*(y2-kbe->y) < 0) ret=1;
//       printf("%d %d %d %d: %d\n",x1,y1,x2,y2,ret);
    }
  }
  if(ret) kgPushBackEvent(D);
  return ret;
}
int ShowActionsDia( void *parent ,char **menu0) {
  int ret=1;
  DIALOG D;
  DIA d[3];
  DIS s0 = { 
    's',
    9,7,  
    388,267,   
    10,  
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
    (char *)" Okay", 
    NULL
  };
  void *xpm1[]  = { 
    NULL, /* pixmap info */ 
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
    141,266,  
    230,299,
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
    ,4,0.5
  };
  d[0].s = &s0;
  d[1].H = &H1;
  d[1].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 100;   /* Position of Dialog */ 
  D.yo = 100;
  D.xo = 50;   /* Position of Dialog */ 
  D.yo = 40;
  D.xl = 395;    /*  Length of Dialog */
  D.yl = 300;    /*  Width  of Dialog */
  D.Initfun = NULL;
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.15;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize=0;
#if 1 
  D.Callback = ShowActionsDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = NULL;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina Clock");    /*  Dialog name you may change */
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
//  kgColorTheme(&D,230,230,230);
  D.SearchList=NULL;
  ret= kgUi(&D);
  return ret;
}
void *RunShowActionsDia(void *arg,char **menu) {
   ShowActionsDia(arg,menu );
   return NULL;
}
int  SetAlertDiabutnbox1callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  DIS *M;
  char **menu;
  int n,ret =1,Nitems,j,item=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  M = (DIS *)kgGetWidget(Tmp,0);
  
  switch(key) {
    case 2: 
      ret=0;
      setnotify(Tmp);
      amenu = M->menu;
      j=0;
      while(amenu[j]!= NULL) free(amenu[j++]);
      free(amenu);
      Nitems=GetActionsCount();
      amenu = (char **)malloc(sizeof(char *)*(Nitems+1));
      for(j=0;j<Nitems;j++) amenu[j]=(char *)malloc(50);
      amenu[Nitems]=NULL;
      GetActionsDisplay(amenu);
      M->menu=amenu;
      break;
    case 1: 
#if 0
      Nitems=GetActionsCount();
      pt = (char **)malloc(sizeof(char *)*(Nitems+1));
      for(j=0;j<Nitems;j++) pt[j]=(char *)malloc(50);
      pt[Nitems]=NULL;
      GetActionsDisplay(pt);
      ShowActionsDia(Tmp,pt);
      j=0;
      while(pt[j]!= NULL) free(pt[j++]);
      free(pt);
#endif
      ret=0;
      break;
    case 3: 
      Nitems=GetActionsCount();
      pt = (char **)malloc(sizeof(char *)*(Nitems+1));
      for(j=0;j<Nitems;j++) pt[j]=(char *)malloc(50);
      pt[Nitems]=NULL;
      GetActionsDisplay(pt);
      DelActionDia(Tmp,&item,pt);
      amenu = M->menu;
      j=0;
      while(amenu[j]!= NULL) free(amenu[j++]);
      free(amenu);
      Nitems=GetActionsCount();
      amenu = (char **)malloc(sizeof(char *)*(Nitems+1));
      for(j=0;j<Nitems;j++) amenu[j]=(char *)malloc(50);
      amenu[Nitems]=NULL;
      GetActionsDisplay(amenu);
      M->menu=amenu;
      ret=0;
      break;
    case 4: 
      ret=1;
      break;
  }
  return ret;
}
int  SetAlertDiabutnbox1callback_o(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  char **menu;
  int n,ret =1,Nitems,j,item=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
#if 1  
  switch(key) {
    case 2: 
      ret=0;
      if(!setnotify(Tmp)) break;
    case 1: 
#if 1
      Nitems=GetActionsCount();
      pt = (char **)malloc(sizeof(char *)*(Nitems+1));
      for(j=0;j<Nitems;j++) pt[j]=(char *)malloc(50);
      pt[Nitems]=NULL;
      GetActionsDisplay(pt);
      ShowActionsDia(Tmp,pt);
      j=0;
      while(pt[j]!= NULL) free(pt[j++]);
      free(pt);
#endif
      ret=0;
      break;
    case 3: 
      Nitems=GetActionsCount();
      pt = (char **)malloc(sizeof(char *)*(Nitems+1));
      for(j=0;j<Nitems;j++) pt[j]=(char *)malloc(50);
      pt[Nitems]=NULL;
      GetActionsDisplay(pt);
      DelActionDia(Tmp,&item,pt);
      ret=0;
      break;
    case 4: 
      ret=1;
      break;
  }
#endif 
  return ret;
}
void  SetAlertDiabrowser1init(DIS *S,void *pt) {
}
void  SetAlertDiabutton1init(DIN *B,void *pt) {
}
void  SetAlertDiasplbutton1init(DIL *B,void *pt) {
}
int SetAlertDiacleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int SetAlertDiainit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  return ret;
}
int SetAlertDiaCallBack(void *Tmp,void *tmp) {
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
int  SetAlertDiahoribar1callback(int key,int i,void *Tmp) {
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
void ModifySetAlertDiaGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int SetAlertDiaGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIP p0 = { 
    'p',
    25,56,  
    341,279,  
    NULL , // Pixmap info
    -1,0,0, /* bkgr colour */ 
    0.0
  };
  strcpy(p0.Wid,(char *)"SetAlertDiaWidget1");
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Show");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Add");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='';
  butn1[2].sw=1;
  strcpy(butn1[2].title,(char *)"Delete");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-1;
  butn1[2].butncode='';
  DIN b1 = { 
    'n',
    5,4, 
    219,34,
    0,0, 
    70,
    24,
    3,1,
    9,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
   SetAlertDiabutnbox1callback_o,
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"SetAlertDiaWidget2");
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"Quit");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode='';
  DIL h2 = { 
    'h',
    436,346,
    492,377,
    4,0,
    50,
    25,
    1,1,
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
//    SetAlertDiasplbutton1callback, /*  Callbak */
    SetAlertDiahoribar1callback,
      NULL  /* any args */
  };
  strcpy(h2.Wid,(char *)"SetAlertDiaWidget3");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+4));
  d =dtmp+i; 
  d[0].t = (DIT *)malloc(sizeof(DIS));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  SetAlertDiabutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIL));
  SetAlertDiasplbutton1init(&h2,pt) ;
  *d[2].h = h2;
  d[2].h->item = -1;
  d[3].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeSetAlertDiaGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = SetAlertDiaGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int SetAlertDia( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  int j;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = SetAlertDiaGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 2;
  D.df = 2;
  D.tw = 38;
  D.bw = 35;
  D.lw = 4;
  D.rw = 3;
  D.xo = 235;   /* Position of Dialog */ 
  D.yo = 150;
  D.xl = 493;    /*  Length of Dialog */
  D.yl = 379;    /*  Width  of Dialog */
  D.Initfun = SetAlertDiainit;    /*   init fuction for Dialog */
  D.Cleanupfun = SetAlertDiacleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 1;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.150000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 1;    /*   for Resize option */
  D.MinHeight = 1;    /*   for Resize option */
#if 1 
  D.Callback = SetAlertDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = NULL;
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = SetAlertDiaWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifySetAlertDiaGc(&(D.gc));    /*  set colors for gui*/
  if(D.parent != NULL) {
    D.gc = ((DIALOG *)D.parent)->gc;
  }
  else kgColorTheme(&D,Red,Green,Blue);
  D.SearchList=NULL;
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int SetAlertDia_o( void *parent ,void *v0 ) {
  int ret=1;
  DIALOG D;
  DIA d[4];
  DIP p0 = { 
    'p',
    25,56,  
    341,279,  
(char *)"##" , // Pixmap info
    -1,0,0, /* bkgr colour */ 
    0.0
  };
  char *titles1[]  = { 
    (char *)"Show", 
    (char *)"Add", 
    (char *)"Delete", 
    NULL
  };
  void *xpm1[]  = { 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL
  };
  int bkgr1[]  = { 
    -1, 
    -1, 
    -1, 
    -1
  };
  char *butncode1  = NULL;
  int sw1[]={1,1,1};
  DIB n1 = { 
    'N',
    5,4,  
    219,34,
    0,0,  
    70, 
    24, 
    3,1, 
    (int *)v0, 
    sw1,
    titles1,
    butncode1,
    NULL,SetAlertDiabutnbox1callback_o, /* args, Callbak */
    xpm1,bkgr1, /* pointers to xpms and colors */
      1,0.150000 /* button type and roundinfg factor(0-0.5) */
  };
  int butn1 =1;
  int sw2[1]  = { 
    1  
  };
  char *titles2[]  = { 
    (char *)"Quit", 
    NULL
  };
  void *xpm2[]  = { 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL
  };
  int bkgr2[]  = { 
    -1, 
    -1
  };
  char *butncode2  = NULL;
  DILN H2 = { 
    'H',
    436,346,  
    492,377,
    4,0,
    50, 
    25, 
    1,1, 
    &butn1, 
    sw2,
    titles2,
    butncode2,
    NULL,SetAlertDiahoribar1callback,/* arg, Callbak */
    xpm2,bkgr2, /* pointers to xpms and colors */
     3,0.150000 /* button type and roundinfg factor(0-0.5) */
  };
  d[0].p = &p0;
  d[1].b = &n1;
  d[2].H = &H2;
  d[3].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 2;
  D.df = 2;
  D.tw = 38;
  D.bw = 35;
  D.lw = 4;
  D.rw = 4;
  D.xo = 235;   /* Position of Dialog */ 
  D.yo = 150;
  D.xl = 493;    /*  Length of Dialog */
  D.yl = 379;    /*  Width  of Dialog */
  D.Initfun = SetAlertDiainit;    /*   init fuction for Dialog */
  D.kbattn = 0;;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.150000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 1;    /*  1 for stickyness */
  D.Resize=0;
#if 0 
  D.Callback = SetAlertDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
  D.StackPos = 1;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = NULL;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina Clock Notifications");    /*  Dialog name you may change */
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
//  ModifySetAlertDiaGc(&(D.gc));    /*  set colors for gui*/
  D.SearchList=NULL;
//  Print_gui_data(&D,"/root/SetAlert.rc");
  ret= kgUi(&D);
  return ret;
}
void *RunSetAlertDia(void *arg) {
/*************************************************

    Buttonbox1 (new) 1 data value

*************************************************/
   int   v0 = 1;
   void* v[1];
   v[0]=(void *)(&v0);
//   printf("Calling SetAlertDia\n");
#if 0
   SetAlertDia_o(NULL,v );
#else
   SetAlertDia(NULL,v ,NULL);
#endif
//   printf("SetAlertDia Okay\n");
   return NULL;
}
