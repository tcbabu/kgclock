#include <kulina.h>
//#include "clkimgs.c"
void * GetClockImage(int no);

extern int ClockType;
int  clocktypebrowser1callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
  ClockType = item;
//  printf("ClockType = %d\n",ClockType);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
static void *ToThumbNailImage(void *tmp) {
  void *img,*fmg;
  fmg = (void *)kgFMGtogmImage(tmp);
  img = (void *)kgChangeSizeImage(fmg,92,84);
  kgFreeImage(fmg);
  return img;
}
void  clocktypebrowser1init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
  int nitems=13;
  void *img,*fmg;
  int i;
  char *Names[14]={"Type1","Type2","Type3","Type4","Type5",
	        "Tpye6","Type7","Type8","Type9","type10",
		"Type11","Type12","Type13",NULL};
//  ThumbNail **List = (ThumbNail **) malloc(sizeof(ThumbNail *)*(nitems+1));
//
  ThumbNail **List = (ThumbNail **) kgStringToThumbNails(Names);
  kgSetList(Y,(void **)List);
#if 0
  for(i=0;i<nitems;i++) {
     List[i]= (ThumbNail *)malloc(sizeof(ThumbNail));
  }
  List[nitems]=NULL;
#endif
#if 1
  List[0]->img = ToThumbNailImage(GetClockImage(0));
  List[1]->img = ToThumbNailImage(GetClockImage(1));
  List[2]->img = ToThumbNailImage(GetClockImage(2));
  List[3]->img =  ToThumbNailImage(GetClockImage(3));
  List[4]->img =  ToThumbNailImage(GetClockImage(4));
  List[5]->img =  ToThumbNailImage(GetClockImage(5));
  List[6]->img =  ToThumbNailImage(GetClockImage(6));
  List[7]->img =  ToThumbNailImage(GetClockImage(7));
  List[8]->img =  ToThumbNailImage(GetClockImage(8));
  List[9]->img =  ToThumbNailImage(GetClockImage(9));
  List[10]->img =  ToThumbNailImage(GetClockImage(10));
  List[11]->img =  ToThumbNailImage(GetClockImage(11));
  List[12]->img =  ToThumbNailImage(GetClockImage(12));
#endif
}
int clocktypeinit(void *Tmp) {
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
int clocktypecleanup(void *Tmp) {
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
int Modifyclocktype(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  DIA *d;
  int i,n;
  d = D->d;
  i=0;while(d[i].t!= NULL) {;
     i++;
  };
  n=1;
  return GrpId;
}

int clocktypeCallBack(void *Tmp,void *tmp) {
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
    }
  }
  return ret;
}
int clocktypeResizeCallBack(void *Tmp) {
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
int clocktypeWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
