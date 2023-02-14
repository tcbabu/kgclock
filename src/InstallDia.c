#include <pthread.h>
#include <kulina.h>
extern unsigned int Red,Green,Blue;
void * SplashThread(void * msg);
#define System(cmd) { \
  int rval; \
  rval = system(cmd); \
}
int InstallDiainit(void *Tmp) {
  SPLASHMSG msg;
  pthread_t Pth;
  int ret = 1;
  char flname[200],command[500];
  char *res;
  DIALOG *D;
  D = (DIALOG *)Tmp;
    kgPrintf(D,0,(char *)"Setting up Kulina Clock");
    kgPrintf(D,0,(char *)"Display must support True Color and Alpha Channel");
    kgUpdateOn(D);
    msg.xsize=600;
    msg.ysize=120;
    msg.xpm = NULL;
//TCB
    msg.message = (char *) malloc(100);
//    msg.message=(char *)"Display must support\nTrue Color and Alpha Channel";
    strncpy(msg.message,(char *)"Display must support\nTrue Color and Alpha Channel",98);
    msg.font =16;
    msg.fontcolor=0;
    msg.bkcolor=0xc00f0f0f;
//    pthread_create(&Pth,NULL,SplashThread,&msg);
    kgPrintf(D,0,(char *)"Making $HOME/.kgclock");
    res=kgWhich((char *)"paplay");
    if(res==NULL) res=kgWhich((char *)"kgmplayer");
    if(res==NULL) res=kgWhich((char *)"mplayer");
    if(res==NULL) res=kgWhich((char *)"aplay");
    if(res!= NULL) {kgPrintf(D,0,res);free(res);}
    else {
        kgPrintf(D,0,(char *)"Could not find a player' no bell support");
        kgUpdateOn(D);
        kgSplashDia(-1,-1,400,120,NULL,(char *)"Could not find a 'player'\n no bell support",16,0,0xc00f0f0f);
    }
    free(msg.message);
#if 0
    res=kgWhich((char *)"festival");
    if(res!= NULL) {kgPrintf(D,0,res);free(res);}
    else {
       kgPrintf(D,0,(char *)"Could not find 'festival' no Message read support");
       kgUpdateOn(D);
       kgSplashDia(-1,-1,400,120,NULL,(char *)"Could not find 'festival'\n no Message read support",16,0,0xc00f0f0f);
    }
#endif
#if 0
    kgPrintf(D,0,(char *)"Pl select 'wav' file for beep sound");
    kgUpdateOn(D);
    flname[0]='\0';
    if(kgFolderBrowser(NULL,400,200,flname,(char *)"*.wav")){
      sprintf(command,"cp %-s %-s/.kgclock/beep.wav",flname,getenv("HOME"));
//      printf("%s\n",command);
      System(command);
    }
    kgPrintf(D,0,(char *)"Pl select 'wav' file for bell sound");
    kgUpdateOn(D);
    flname[0]='\0';
    if(kgFolderBrowser(NULL,400,200,flname,(char *)"*.wav")){
      sprintf(command,"cp %-s %-s/.kgclock/phone.wav",flname,getenv("HOME"));
      System(command);
    }
#else
      
      sprintf(command,"mkdir -p   %-s/.kgclock",getenv("HOME"));
      System(command);
      sprintf(command,"cp /usr/share/kgclock/beep.wav  %-s/.kgclock/beep.wav",getenv("HOME"));
      System(command);
      sprintf(command,"cp /usr/share/kgclock/bell.wav  %-s/.kgclock/phone.wav",getenv("HOME"));
      System(command);
#endif
//    pthread_join(Pth,NULL);
  return ret;
}
int InstallDiaCallBack(void *Tmp,void *tmp) {
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
int InstallDia( void *parent ) {
  int ret=1;
  DIALOG D;
  DIA d[2];
  DII i0 = { 
    'i',
    12,12,  
    388,240,  
    40,10   
  };
  d[0].i = &i0;
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
  D.xo = 100;   /* Position of Dialog */ 
  D.yo = 100;
  D.xl = 396;    /*  Length of Dialog */
  D.yl = 250;    /*  Width  of Dialog */
  D.Initfun = InstallDiainit;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize =0;
#if 0 
  D.Callback = InstallDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = NULL;    /*  any data to be passed by user*/
  D.StackPos=0;
  D.Shapexpm= NULL;
  strcpy(D.name,"Kulina Clock Setup");    /*  Dialog name you may change */
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
//  else kgColorTheme(&D,Red,Green,Blue);
  ret= kgUi(&D);
  return ret;
}
void *RunInstallDia(void *arg) {
/*************************************************


*************************************************/
   InstallDia(NULL );
   return NULL;
}
