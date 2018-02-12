#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <pthread.h>
#include "kulina.h"
#include "defines.h"
extern unsigned int Red,Green,Blue;
extern pthread_mutex_t _Tmplock;
static char Pgpath[300];
static char dbsfile[300];
static char lockfile[300];
static long Day=0;
static int Curmin=0;
typedef struct _date_time {
	long date;
	int hr;
	int minutes;
} TIME;
void Setupdate(void);
static long Date(void);
static long curtime_in_mins(void);
static int GetDateTime(void);
void UpdateAndWrite(void);
static char message[300]=" ",flname[200]=" ";
//static int repeat=1440,action=1; //1440 is a day
static int repeat=0,action=1;
static  int hr,minutes;
int  setnotifydiabrowser1callback(int key,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  switch(key) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      gscanf(NULL,(char *)"Give Text:%60s",message);
      break;
    case 4: 
      gscanf(NULL,(char *)"Give Text:%60s",message);
      break;
    case 5: 
//      file_browser(NULL,50,50,flname,"*");
//      strcpy(message,flname);
      break;
    case 6: 
      gscanf(NULL,(char *)"Give Job:%60s",message);
      break;
  }
  return ret;
}
int  setnotifydiatextbox1callback(int key,int i,void *Tmp) {
  DIA *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = ((DIALOG *)Tmp)->d;
  T = D[i].t;
  e = T->elmt;
  return ret;
}
int  setnotifydiabrowser2callback(int key,int i,void *Tmp) {
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
int  setnotifydiabrowser3callback(int key,int i,void *Tmp) {
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
int  setnotifydiabrowser4callback(int key,int i,void *Tmp) {
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
int  setnotifydiabrowser5callback(int key,int i,void *Tmp) {
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
int  setnotifydiahoribar1callback(int key,int i,void *Tmp) {
  DIA *D;DILN *B; 
  DIW *W;
  int n,ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].H;
  W = D[0].w;
  n = B->nx;
  action=*(W->df);
  ret = action -1;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int  setnotifydiatextbox2callback(int key,int i,void *Tmp) {
  DIA *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = ((DIALOG *)Tmp)->d;
  T = D[i].t;
  e = T->elmt;
  return ret;
}
int setnotifydiainit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  return ret;
}
int setnotifydiaCallBack(void *Tmp,void *tmp) {
  int ret = 0,x1,y1,x2,y2;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
       x1 = D->xo; y1= D->yo;x2= x1+D->xl;y2= y1+D->yl;
       if( (kbe->x -x1)*(x2-kbe->x) < 0) ret=1;
       if( (kbe->y-y1)*(y2-kbe->y) < 0) ret=1;
    }
  }
  if(ret) kgPushBackEvent(D);
  return ret;
}
int  setnotifydiabrowser6callback(int item,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int setnotifydia( void *parent ,void *v0 ,void *v1 ,void *v2 ,void *v3 
 ,void *v4 ,void *v5 ,void *v6 ,void *v7 ) {
  int ret=1;
  DIALOG D;
  DIA d[14];
  char *menu0[]  = { 
    (char *)"(No action)",
    (char *)"Ring Bell",
    (char *)"Read Message",
    (char *)"Splash Message",
    (char *)"!c03Powerdown",
    (char *)"Execute job",
    NULL 
  };
  DIW w0 = { 
    'w',
    136,16,  
    375,45,   
    6,  
    (int *)v0,
    (char *)"Action" ,
    menu0 ,
    NULL,setnotifydiabrowser1callback,0 /* *args, callback */
  };
  T_ELMT e1[1]  = { 
    {(char *)"Year%4d", v1, 1}
  };
  DIT t1 = { 
    't',
    17,104,  
    144,139,
    20, 
    1,1, 
    e1,
    1,1,
    NULL,setnotifydiatextbox1callback /* args,Call back */
  };
  t1.type =0;
  char *menu2[]  = { 
    (char *)"Jan", (char *)"Feb", (char *)"Mar", (char *)"Apr", (char *)"May", (char *)"Jun", (char *)"Jul", (char *)"Aug", (char *)"Sep", (char *)"Oct", (char *)"Nov", (char *)"Dec", NULL 
  };
  DIW w2 = { 
    'w',
    146,106,  
    215,135,   
    6,  
    (int *)v2,
    (char *)"" ,
    menu2 ,
    NULL,setnotifydiabrowser2callback,0 /* *args, callback */
  };
  char *menu3[]  = { 
    (char *)"1", (char *)"2", (char *)"3", (char *)"4", (char *)"5", (char *)"6", (char *)"7", (char *)"8", (char *)"9", (char *)"10", (char *)"11", (char *)"12", (char *)"13", (char *)"14", (char *)"15", (char *)"16", (char *)"17", (char *)"18", (char *)"19",
    (char *)"20", (char *)"21", (char *)"22", (char *)"23", (char *)"24", (char *)"25", (char *)"26", (char *)"27", (char *)"28", (char *)"29", (char *)"30", (char *)"31", NULL 
  };
  char *menu4[]  = { 
    (char *)"0",(char *)"1", (char *)"2", (char *)"3", (char *)"4", (char *)"5", (char *)"6", (char *)"7", (char *)"8", (char *)"9", (char *)"10", (char *)"11", (char *)"12", (char *)"13", (char *)"14", (char *)"15", (char *)"16", (char *)"17",
    (char *)"18", (char *)"19", (char *)"20", (char *)"21", (char *)"22", (char *)"23",  NULL 
  };
  char *menu5[]  = { 
    (char *)"0", (char *)"1", (char *)"2", (char *)"3", (char *)"4", (char *)"5", (char *)"6", (char *)"7", (char *)"8", (char *)"9", (char *)"10", (char *)"11", (char *)"12", (char *)"13", (char *)"14", (char *)"15", (char *)"16", (char *)"17",
    (char *)"18", (char *)"19", (char *)"20", (char *)"21", (char *)"22", (char *)"23", (char *)"24", (char *)"25", (char *)"26", (char *)"27", (char *)"28", (char *)"29", (char *)"30", (char *)"31",
    (char *)"32",(char *)"33",(char *)"34",(char *)"35",(char *)"36",(char *)"37",(char *)"38",(char *)"39",(char *)"40",(char *)"41",(char *)"42",(char *)"43",(char *)"44",(char *)"45",(char *)"46",(char *)"47",(char *)"48",(char *)"49",
    (char *)"50",(char *)"51",(char *)"52",(char *)"53",(char *)"54",(char *)"55",(char *)"56",(char *)"57",(char *)"58",(char *)"59",
    NULL 
  };
  DIW w3 = { 
    'w',
    215,106,  
    275,135,   
    6,  
    (int *)v3,
    (char *)"" ,
    menu3 ,
    NULL,setnotifydiabrowser3callback,0 /* *args, callback */
  };
  DIW w4 = { 
    'w',
    308,106,  
    370,135,   
    6,  
    (int *)v4,
    (char *)"" ,
    menu4 ,
    NULL,setnotifydiabrowser4callback,0 /* *args, callback */
  };
  DIW w5 = { 
    'w',
    380,106,  
    440,135,   
    6,  
    (int *)v5,
    (char *)"" ,
    menu5 ,
    NULL,setnotifydiabrowser5callback,0 /* *args, callback */
  };
  int butn1 =1;
  int sw6[1]  = { 
    1  
  };
  char *titles6[]  = { 
    (char *)"Add", 
    NULL
  };
  void *xpm6[]  = { 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL, /* pixmap info */ 
    NULL
  };
  int bkgr6[]  = { 
    -1, 
    -1
  };
  char *butncode6  = NULL;
  DILN H6 = { 
    'H',
    217,225,  
    304,255,
    4,0,
    80, 
    25, 
    1,1, 
    &butn1, 
    sw6,
    titles6,
    butncode6,
    NULL,setnotifydiahoribar1callback,/* arg, Callbak */
    xpm6,bkgr6, /* pointers to xpms and colors */
     3,0.500000 /* button type and roundinfg factor(0-0.5) */
  };
  T_ELMT e7[1]  = { 
    {(char *)"Repeat Inrevel %5d", v6, 1}
  };
  DIT t7 = { 
    't',
    79,171,  
    314,205,
    20, 
    1,1, 
    e7,
    1,1,
    NULL,setnotifydiatextbox2callback /* args,Call back */
  };
  t7.type =0;
  DIM m8 = { 
    'm',
    124,61,  
    165,85,  0,0
  };
  strcpy(m8.msg,(char *)"Date");
  DIM m9 = { 
    'm',
    349,61,  
    390,85,  
    0,0
  };
  strcpy(m9.msg,(char *)"Time");
  DIM m10 = { 
    'm',
    319,86,  
    360,110,  
    0,0
  };
  strcpy(m10.msg,(char *)"Hour");
  DIM m11 = { 
    'm',
    381,86,  
    440,110,  
    0,0
  };
  strcpy(m11.msg,(char *)"Minute");
  char *menu12[]  = { 
    (char *)"minutes",
    (char *)"hours",
    (char *)"days",
    NULL 
  };
  DIW w12 = { 
    'w',
    316,173,  
    421,202,   
    3,  
    (int *)v7,
    (char *)"" ,
    menu12 ,
    NULL,setnotifydiabrowser6callback,0 /* *args, callback */
  };
  d[0].w = &w0;
  d[1].t = &t1;
  d[2].w = &w2;
  d[3].w = &w3;
  d[4].w = &w4;
  d[5].w = &w5;
  d[6].H = &H6;
  d[7].t = &t7;
  d[8].m = &m8;
  d[9].m = &m9;
  d[10].m = &m10;
  d[11].m = &m11;
  d[12].w = &w12;
  d[13].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 12;
  D.tw = 4;
  D.bw = 47;
  D.lw = 4;
  D.rw = 4;
  D.xo = 20;   /* Position of Dialog */ 
  D.yo = 45;
  D.xl = 465;    /*  Length of Dialog */
  D.yl = 286;    /*  Width  of Dialog */
//  Print_gui_data(&D,"/root/setnotify.rc");
  D.Initfun = setnotifydiainit;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.150000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
#if 1 
  D.Callback = setnotifydiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
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
//  ModifysetnotifydiaGc(&(D.gc));    /*  set colors for gui*/
  D.SearchList=NULL;
  ret= kgUi(&D);
  return ret;
}
void *Runsetnotifydia(void *arg) {
/*************************************************

    Browser1  1 data value
    Text_Box1  1 data values
    Browser2  1 data value
    Browser3  1 data value
    Browser4  1 data value
    Browser5  1 data value
    Text_Box2  1 data values

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   int   v2 = 1;
   int   v3 = 1;
   int   v4 = 1;
   int   v5 = 1;
//   int   v6 = 1440;
   int   v6 = 0;
   int   v7 = 1;
   int ret;
   GetDateTime();
   hr = Curmin/60;
   minutes = Curmin%60;
   v1 = Day/10000;
   v2 = (Day%10000)/100;
   v3 = (Day%100);
   v4 = hr+1;
   v5=minutes+1;
   action =1;
   ret =setnotifydia(arg ,&v0 ,&v1 ,&v2 ,&v3 ,&v4 ,&v5 ,&v6 ,&v7);
   Day = v1*10000+v2*100+v3;
   hr = v4-1;
   minutes=v5-1;
   switch(v7) {
     case 1:
     default:
       repeat = v6;
       break;
     case 2:
       repeat = v6*60;
       break;
     case 3:
       repeat = v6*1440;
       break;
   }
   return NULL;
}
static long Date(void) {
  time_t tp,t;
  struct tm *dt;
  t=time(&tp);
  dt = localtime(&t);
  tp = (dt->tm_mday)+(dt->tm_mon+1)*100+(dt->tm_year)*10000;
  if(tp<200000L) tp +=20000000L;
  else tp+=19000000L;
  return(tp);
 }
static long curtime_in_secs(void) {
  time_t tp,t;
  struct tm *dt;
  t=time(&tp);
  dt = localtime(&t);
  tp = ((dt->tm_hour)%24)*3600+(dt->tm_min)*60+(dt->tm_sec);
  return(tp);
}
static long curtime_in_mins(void) {
  time_t tp,t;
  struct tm *dt;
  t=time(&tp);
  dt = localtime(&t);
  tp = ((dt->tm_hour)%24)*60+(dt->tm_min);
  return(tp);
}
static int waitforlock(void) {
	FILE *fp;
	while( (fp=fopen(lockfile,"r"))!= NULL) fclose(fp);
	return 1;
}
static int touchlock(void) {
	FILE *fp;
	fp = fopen(lockfile,"w");
	fclose(fp);
	return 1;
}
static int clearlock(void) {
	remove(lockfile);
	return 1;
}
static int GetDateTime(void) {
   Day = Date();
   Curmin = curtime_in_mins();
   return Curmin;
}
void * setnotify(void *arg) {
	FILE *fp;
	char buff[200],flname[100];
	int rep,i;
	char code;
//	if(arg != NULL ) {strcpy(Pgpath,arg);strcat(Pgpath,"/");}
//	else {
        {
           strcpy(Pgpath,getenv("HOME"));
           strcat(Pgpath,"/.kgclock/");
        }
	strcpy(dbsfile,Pgpath);
	strcat(dbsfile,"notify.dbs");
        Runsetnotifydia(arg);
	switch(action) {
	   case 1: return NULL;
	   case 2: code='R';break;
	   case 3: code='M';break;
	   case 4: code='S';break;
	   case 5: code='P';break;
	   case 6: code='E';break;
	   default:	     
		     printf("Error: Unknown Code |%c|\n",code);
	             return NULL;
	}
        pthread_mutex_lock(&_Tmplock);
	fp = fopen(dbsfile,"a");
	if((code=='P')||(code=='R')) fprintf(fp,"%ld %d %d %c %d\n",Day,hr,minutes,code,repeat);
	else  fprintf(fp,"%ld %d %d %c %d %s\n",Day,hr,minutes,code,repeat,message);
        fclose(fp);
        UpdateAndWrite();
	pthread_mutex_unlock(&_Tmplock);
	return (void *)1;
}
