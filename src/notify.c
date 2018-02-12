/************************************************************
  Dated 9th April 2008
 ************************************************************/
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
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
extern pthread_mutex_t _Tmplock;
static char Pgpath[300];
static char dbsfile[300];
static char lockfile[300];
static char mailfile[300];
static char Tmpfile[300];
static char Audfile[300];
static int Nitems=0;
static long Day=0;
static int Curmin=0;
static int Update=1;
static int Wait=10;
typedef struct _ntfy {
	long day;
	int hr;
	int min;
	char code;
	int rep;
	char msg[MLNG+1];
	int status;
} NOTIFY;
typedef struct _date_time {
	long date;
	int hr;
	int minutes;
} TIME;
static NOTIFY Ntfy[NOOFNOTIFY];
static int UpdateAction(NOTIFY *Ntfy);
static int UpdateTable(void);
static long Date(void);
static long curtime_in_mins(void);
static int Readdb(void);
static int GetDateTime(void);
static void  Cleandb(void);
static int Sortdb(void);
static int CheckStatus(NOTIFY N);
static int runjob(char *arg);
void DeleteAction(int item);
void DeleteActions(int *sw,char **menu);
static int Months[12]= {31,28,31,30,31,30,31,31,30,31,30,31};
int flite(int argc,char **argv);
#define Lock pthread_mutex_lock(&_Tmplock)
#define Unlock pthread_mutex_unlock(&_Tmplock)
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **)){
   int ret =0;
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2],argc;
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   while(job[i]==' ') i++;
   strcpy(buff,job+i);
   i=0;
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
     if(pt[0]=='\"') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\"')pos++;
      buff[pos]='\0';
      i++;
     }
     else {
       args[i]=buff+pos;
       pos +=strlen(pt);
       i++;
       if(buff[pos]< ' ') break;
       buff[pos]='\0';
     }
     pos++;
     while(buff[pos]==' ') pos++;
//     printf("%s ",args[i-1]);
   }
//   printf("\n");
   args[i]=NULL;
   if(i==0) return 0;
   argc=i;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
     dup(pip[1]);
     close(pip[1]);
     if(function != NULL) function(argc,args);
     printf("END:\n");
     exit(0);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL){
        ret=ProcessOut(pip[0],pip2[1],pid);
//        printf("Killing %d\n",pid);
        kill(pid,9);
     }
     waitpid(pid,&status,0);
     return ret;
   }
}
void nsleep_o(unsigned long nsec) {
    struct timespec delay = { nsec / 1000000000, nsec % 1000000000 };
    pselect(0, NULL, NULL, NULL, &delay, NULL);
}
int GetActionsCount(void) {
  int ret;
  pthread_mutex_lock(&_Tmplock);
  ret= Nitems;
  pthread_mutex_unlock(&_Tmplock);
  return ret;
}
void GetActionsDisplay(char **pt) {
  int i;
  char buff[500];
  pthread_mutex_lock(&_Tmplock);
  for(i=0;i<Nitems;i++) {
    sprintf(buff,"%8.8ld %2.2d %2.2d %c %6d %s\0",
     Ntfy[i].day,Ntfy[i].hr,Ntfy[i].min,Ntfy[i].code, Ntfy[i].rep,Ntfy[i].msg);
     buff[49]='\0';
    strcpy(pt[i],buff);
  }
  pthread_mutex_unlock(&_Tmplock);
}

void GetActionsItemDisplay(int item, char *pt) {
  int i;
  char buff[500];
  pt[0]='\0';
  pthread_mutex_lock(&_Tmplock);
  for(i=0;i<Nitems;i++) {
    if(item != i) continue;
    sprintf(buff,"%8.8ld %2.2d %2.2d %c %6d %s\0",
     Ntfy[i].day,Ntfy[i].hr,Ntfy[i].min,Ntfy[i].code, Ntfy[i].rep,Ntfy[i].msg);
     buff[49]='\0';
    strcpy(pt,buff);
    break;
  }
  pthread_mutex_unlock(&_Tmplock);
}

int ThreadSleep(int secs,int usecs) {
    int retval;
    struct timeval tv1;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv1.tv_sec = secs;
    tv1.tv_usec = usecs;
    retval = select(0, &rfds, NULL, NULL, &tv1);
    return retval;
}
static int Daysofmonth(int yr,int month) {
	int leap=0;
	if ((month==2)&&(yr%4)== 0) leap=1;
	return Months[month-1]+leap;
}
static long adddays(long date,int days) {
	int yr,month,day,mdays,daystmp;
	yr = date/10000;
	month = (date%10000)/100;
	day = (date%100);
	daystmp = day+days;
	mdays = Daysofmonth(yr,month);
	while(mdays < daystmp) {
		month = month+1;
		daystmp -= mdays;
		if(month > 12 ) {yr++;month -=12;}
	        mdays = Daysofmonth(yr,month);
	}
	return (long)(yr*10000+month*100+daystmp);
}
static TIME *addminutes(TIME *dt,int minutes) {
	int curmin,hr,d;
	TIME dttmp;
	dttmp = *dt;
	curmin = dt->hr*60+dt->minutes+minutes;
	hr = curmin/60;
	dttmp.minutes = curmin%60;
	if( (d=hr/24) != 0) dttmp.date = adddays(dttmp.date,d);
	dttmp.hr = hr%24;
	*dt = dttmp;
	return dt;
}
static TIME *AddMinutes(TIME *dt,int minutes) {
	long today,ctime,hr,min,mintmp;
	today = Date();
	ctime = curtime_in_mins();
	hr = ctime/60;
	min = ctime%60;
	addminutes(dt,minutes);
	while((dt->date < today)) addminutes(dt,minutes);
	mintmp = dt->hr*60+dt->minutes;
	while((dt->date == today) && (mintmp < ctime)){
	   addminutes(dt,minutes);
	   mintmp = dt->hr*60+dt->minutes;
	}
	return dt;
}
static int CrrMinutes(TIME *dt,int minutes) {
        int ret=0;
	long today,ctime,hr,min,mintmp;
	today = Date();
	ctime = curtime_in_mins();
	hr = ctime/60;
	min = ctime%60;
	while((dt->date < today)) {
          ret=1;
          addminutes(dt,minutes);
        }
	mintmp = dt->hr*60+dt->minutes;
	while((dt->date == today) && (mintmp < ctime)){
           ret=1;
	   addminutes(dt,minutes);
	   mintmp = dt->hr*60+dt->minutes;
	}
	return ret;
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
static int runfestival(char *arg){
   int pid,status;
   char buff[300];
   pid = fork();
   if(pid == 0) { /* child process */
     if(fork()!=0) exit(0); /* to avoid zombie */
     if(arg[0] != '/') {
         strcpy(buff,Pgpath);
         strcat(buff,arg);
     }
     else {
         strcpy(buff,arg);
     }
//     execl(pgrpath,"festival","--tts",buff,NULL);
//     flite(2,NULL);
     runfunction(buff,NULL,flite);
     fprintf(stderr,"Failed to execute festival\n");
     sleep(5);
     exit(1);
   }
   else {   /* parent process */
     waitpid(pid,&status,0);
     return 1;
   }
}

static int runaplay(char *arg){
   int pid,status;
   char buff[300];
   static char *pgrpath=NULL;
   static int entry=0;
   if(entry==0) {
     entry =1;
     pgrpath=kgWhich((char *)"aplay");
   }
   if (pgrpath==NULL) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
     if(fork()!=0) exit(0); /* to avoid zombie */
     if(arg[0] != '/') {
         strcpy(buff,Pgpath);
         strcat(buff,arg);
     }
     else {
         strcpy(buff,arg);
     }
     execl(pgrpath,"aplay",buff,NULL);
     fprintf(stderr,"Failed to execute aplay\n");
     sleep(5);
     exit(1);
   }
   else {   /* parent process */
     waitpid(pid,&status,0);
     return 1;
   }
}
static int PlayWav(char *wav) {
	char comm[1000];
	strcpy(comm,"aplay -q ");
	strcat(comm,wav);
	system(comm);
}
static int runflite(char *arg){
   int pid,status;
   pid = fork();
   if(pid == 0) { /* child process */
     if(fork()!=0) exit(0); /* to avoid zombie */
     else {
       char buff[300];
       sprintf(buff,"flite -t \"%-s\" -o /tmp/%-d.wav -voice"
          " slt",arg,getpid()); 
       runfunction(buff,NULL,flite);
       sprintf(buff,"/tmp/%-d.wav" ,getpid());
       PlayWav(buff);
       remove(buff);
       exit(1);
     }
   }
   else {   /* parent process */
     waitpid(pid,&status,0);
     return 1;
   }
}

static int runjob(char *job){
   int pid,status;
   char *args[50],buff[1000],pt[100];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
   strcpy(buff,job);
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
     args[i]=buff+pos;
     pos +=strlen(pt);
     i++;
     if(buff[pos]< ' ') break;
     buff[pos]='\0';
     pos++;
   }
   args[i]=NULL;
   if(i==0) return 0;
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
     if(fork()!=0) exit(0); /* to avoid zombie */
     execv(pgrpath,args);
     fprintf(stderr,"Failed to execute aplay\n");
     sleep(5);
     exit(1);
   }
   else {   /* parent process */
     waitpid(pid,&status,0);
     free(pgrpath);
     return 1;
   }
}
static int SayText(char *mes){
#if 0
	char comm[1000];
	FILE *aud;
        aud = fopen(Audfile,"w");
	fprintf(aud,"%s\n",mes);
	fclose(aud);
	runfestival(Audfile);
	/*
	sprintf(comm,"festival --tts audiofile");
	system(comm);
	*/
#else
        runflite(mes);
#endif
	return 1;
}
static int PlayFile(char *file){
	runaplay(file);
	/*
	char comm[1000];
	sprintf(comm,"festival --tts %s",file);
	system(comm);
	*/
}
static int Ring(void) {
	char comm[1000];
	strcpy(comm,"aplay -q ");
	strcat(comm,Pgpath);
	strcat(comm,"phone.wav");
	system(comm);
}
static int Beep(void) {
	char comm[1000];
	strcpy(comm,"aplay -q ");
	strcat(comm,Pgpath);
	strcat(comm,"beep.wav");
	system(comm);
}
static void  Cleandb(void) {
	int i=0,j;
        i=0;
        while(i<Nitems) {
          Ntfy[i].status=CheckStatus(Ntfy[i]);
          if(Ntfy[i].status==0) {
            j=i+1;
            while(j<Nitems) {Ntfy[j-1]=Ntfy[j];j++;};
            Nitems--;
          }
          i++;
        }
       return;
}
static int Sortdb(void) {
	int i=0,j,tmp,minutes;
	int  mintmp;
	long daytmp;
	NOTIFY Ntmp;
        Cleandb();
	for(i=0;i<(Nitems-1);i++) {
		tmp =i;
		daytmp = Ntfy[i].day;
		mintmp = Ntfy[i].hr*60+Ntfy[i].min;
		for(j=i+1;j<Nitems;j++) {
			if(Ntfy[j].day > daytmp )continue;
			if(Ntfy[j].day == daytmp ){
	                  minutes = Ntfy[j].hr*60+Ntfy[j].min;
			  if(minutes >= mintmp ) continue;
			  else {
		            tmp =j;
		            daytmp = Ntfy[tmp].day;
		            mintmp = Ntfy[tmp].hr*60+Ntfy[tmp].min;
			  }
			}
		}
		if(i!=tmp) {
			Ntmp = Ntfy[tmp];
			Ntfy[tmp]=Ntfy[i];
			Ntfy[i] = Ntmp;
		}
	}
	return 1;
}
static int CheckStatus(NOTIFY N) {
    int mints,ret=1;
    if(N.status==0) ret=0;
    else if(N.rep == 0) {
  	GetDateTime();
	mints = N.hr*60+N.min;
	if( Day > N.day ) ret= 0;
	else if((Day == N.day)&&(Curmin>mints)) ret= 0;
    }
    return ret;
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
static int Readdb(void){
	int i=0,k,ch;
	FILE *fp;
	Nitems=0;
	fp = fopen(dbsfile,"r");
	if(fp!= NULL) {
		while(fscanf(fp,"%ld %d %d %c %d",&(Ntfy[i].day),&(Ntfy[i].hr),
			&(Ntfy[i].min),&(Ntfy[i].code),&(Ntfy[i].rep)) >0){
		  while((ch=fgetc(fp))==' ');
		  k=0;
		  while(ch >= ' ') {
		    Ntfy[i].msg[k++]=ch;
		    ch=fgetc(fp);
                    if(k>MLNG) k=MLNG;
		  }
		  Ntfy[i].msg[k]='\0';
		  Ntfy[i].status = 1;
		  Ntfy[i].status = CheckStatus(Ntfy[i]);
		  if(i==(NOOFNOTIFY -1)) {i=NOOFNOTIFY;break;}
		  i++;
		}
		fclose(fp);
	}
	Nitems = i;
//        Cleandb(); //modified
	return 1;
}
int Writedb(void) {
	int i;
	FILE *fp;
	TIME t;
        Lock;
	fp = fopen(dbsfile,"w");
	if(fp!=NULL){
	  for(i=0;i<Nitems;i++) {
		if(Ntfy[i].status==0 ) {
	          if(Ntfy[i].rep == 0) continue;
		  else {
		    t.date = Ntfy[i].day;
		    t.hr   = Ntfy[i].hr;
		    t.minutes  = Ntfy[i].min;
		    AddMinutes(&t,Ntfy[i].rep);
		    Ntfy[i].day = t.date;
		    Ntfy[i].hr = t.hr;
		    Ntfy[i].min = t.minutes;
		  }
		}
		fprintf(fp,"%ld %d %d %c %d %s\n",Ntfy[i].day,Ntfy[i].hr,
                         Ntfy[i].min,Ntfy[i].code,Ntfy[i].rep,Ntfy[i].msg);
	  }
	  fclose(fp);
        }
        Unlock;
	return 1;
	
}
void Setupdate(void) {Update=1;}
void UpdateAndWrite(void) {
	FILE *fp;
        int i;
        Readdb();
        Cleandb();
        UpdateTable();
        Sortdb();
	fp = fopen(dbsfile,"w");
	if(fp!=NULL){
	  for(i=0;i<Nitems;i++) {
		fprintf(fp,"%ld %d %d %c %d %s\n",Ntfy[i].day,Ntfy[i].hr,
                         Ntfy[i].min,Ntfy[i].code,Ntfy[i].rep,Ntfy[i].msg);
//		printf("%ld %d %d %c %d %s\n",Ntfy[i].day,Ntfy[i].hr,
//                         Ntfy[i].min,Ntfy[i].code,Ntfy[i].rep,Ntfy[i].msg);
	  }
	  fclose(fp);
	}
        Update=0;
        return;
}
static int Updatedb(void) {
	int i,ret=1;
	FILE *fp;
	TIME t;
        Lock;
        if(!Update) { UpdateTable(); Sortdb();ret= 0; }
        else UpdateAndWrite();
        Unlock;
	return ret;
}
void DeleteAction(int item) {
	int i,count=0;
	FILE *fp;
	TIME t;
        Lock;
//        Update=1;
	fp = fopen(dbsfile,"w");
	if(fp!=NULL){
          count=0;
	  for(i=0;i<Nitems;i++) {
                if(i==item) continue;
		if(Ntfy[i].status==0 ) {
	          if(Ntfy[i].rep == 0) continue;
		  else {
		    t.date = Ntfy[i].day;
		    t.hr   = Ntfy[i].hr;
		    t.minutes  = Ntfy[i].min;
		    AddMinutes(&t,Ntfy[i].rep);
		    Ntfy[i].day = t.date;
		    Ntfy[i].hr = t.hr;
		    Ntfy[i].min = t.minutes;
		  }
		}
                count++;
		fprintf(fp,"%ld %d %d %c %d %s\n",Ntfy[i].day,Ntfy[i].hr,
                         Ntfy[i].min,Ntfy[i].code,Ntfy[i].rep,Ntfy[i].msg);
	  }
	  fclose(fp);
        }
        Nitems=count;
        Readdb();
        Cleandb();
        UpdateTable();
        Sortdb();
        Update=0;
        Unlock;
}
void DeleteActions(int *sw,char **pt) {
	int i,count=0;
	FILE *fp;
	TIME t;
        char buff[500];
        Lock;
//        Update=1;
	fp = fopen(dbsfile,"w");
	if(fp!=NULL){
          count=0;
	  for(i=0;i<Nitems;i++) {
                if(sw[i]==1){
                    sprintf(buff,"%8.8ld %2.2d %2.2d %c %6d %s\0",
                         Ntfy[i].day,Ntfy[i].hr,Ntfy[i].min,Ntfy[i].code, Ntfy[i].rep,Ntfy[i].msg);
                    buff[49]='\0';
                    if(strcmp(buff,pt[i])==0) continue;
                }
		if(Ntfy[i].status==0 ) {
	          if(Ntfy[i].rep == 0) continue;
		  else {
		    t.date = Ntfy[i].day;
		    t.hr   = Ntfy[i].hr;
		    t.minutes  = Ntfy[i].min;
		    AddMinutes(&t,Ntfy[i].rep);
		    Ntfy[i].day = t.date;
		    Ntfy[i].hr = t.hr;
		    Ntfy[i].min = t.minutes;
		  }
		}
                count++;
		fprintf(fp,"%ld %d %d %c %d %s\n",Ntfy[i].day,Ntfy[i].hr,
                         Ntfy[i].min,Ntfy[i].code,Ntfy[i].rep,Ntfy[i].msg);
	  }
	  fclose(fp);
        }
        Nitems=count;
        Readdb();
        Cleandb();
        UpdateTable();
        Sortdb();
        Update=0;
        Unlock;
}
static int UpdateAction(NOTIFY *Ntfy) {
	int i,ret=0;
	FILE *fp;
	TIME t;
	if(Ntfy->rep != 0){
		    t.date = Ntfy->day;
		    t.hr   = Ntfy->hr;
		    t.minutes  = Ntfy->min;
		    AddMinutes(&t,Ntfy->rep);
		    Ntfy->day = t.date;
		    Ntfy->hr = t.hr;
		    Ntfy->min = t.minutes;
	}
        else Ntfy->status=0;
	return 1;
}
static int UpdateTable(void) {
	int i,ret=0;
	FILE *fp;
	TIME t;
        Cleandb();
	for(i=0;i<Nitems;i++) {
		if(Ntfy[i].status==0 )continue;
	        if(Ntfy[i].rep != 0){
		    t.date = Ntfy[i].day;
		    t.hr   = Ntfy[i].hr;
		    t.minutes  = Ntfy[i].min;
		    CrrMinutes(&t,Ntfy[i].rep);
		    Ntfy[i].day = t.date;
		    Ntfy[i].hr = t.hr;
		    Ntfy[i].min = t.minutes;
		}
	}
	return 1;
}

void * SplashThread(void * msg) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    RunkgSplashDia(msg);
    return NULL;
}
void * EmptyThread(void * msg) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    return NULL;
}

void * TakeActions(void *arg) {
     int i,minutes;
     pthread_t Mth;
     pthread_create(&Mth,NULL,EmptyThread,NULL);
#if 1  //as on 09th April 2016
     GetDateTime();
     i=0;
     Lock;
     Readdb();
     while(i<Nitems) {
//          if(Ntfy[i].status==0) {i++;continue;}
	  if(Day >= Ntfy[i].day){ 
	      minutes = Ntfy[i].hr*60+Ntfy[i].min;
	      if(Curmin > minutes){
		 switch(Ntfy[i].code){
		   case 'R':
                     Unlock;
                     Ring();
                     Lock;
		     break;
		   case 'M':
                     Unlock;
		     Beep();
                     SayText(Ntfy[i].msg);
                     Lock;
		     break;
		   case 'F':
                     Unlock;
                     PlayFile(Ntfy[i].msg);
                     Lock;
		     break;
		   case 'E':
		     break;
		   case 'S': // Splash Message
                     {
                       SPLASHMSG msg;
                       Unlock;
                       pthread_join(Mth,NULL);
                       msg.xsize=500;
                       msg.ysize=60;
                       msg.xpm=NULL;
                       msg.message=Ntfy[i].msg;
                       msg.font =23;
                       msg.fontcolor=50;
                       msg.bkcolor = 0xf08f8f8f;
                       kgSplashDia(-1,-1,500,60,NULL,Ntfy[i].msg,msg.font,msg.fontcolor,msg.bkcolor);
 //                      pthread_create(&Mth,NULL,SplashThread,&msg);
                       Lock;
                     }

		     break;
		   case 'P':
		     break;
		   default:
		     break;
		 }
	    }
	  }
	  i++;
     }
     Unlock;
     Cleandb();
#endif

     while(1) {
        GetDateTime();
        Updatedb();
	i=0;
	Wait= 10;  //45
        Lock;
	while(i<Nitems) {
          if(Ntfy[i].status==0) {i++;continue;}
	  if(Day <= Ntfy[i].day){ 
	    if(Day == Ntfy[i].day) {
	      minutes = Ntfy[i].hr*60+Ntfy[i].min;
	      if(Curmin == minutes){
		 switch(Ntfy[i].code){
		   case 'R':
                     UpdateAction(Ntfy+i);
                     Unlock;
                     Ring();
                     Lock;
		     break;
		   case 'M':
                     UpdateAction(Ntfy+i);
                     Unlock;
		     Beep();
                     SayText(Ntfy[i].msg);
                     Lock;
		     break;
		   case 'F':
                     UpdateAction(Ntfy+i);
                     Unlock;
                     PlayFile(Ntfy[i].msg);
                     Lock;
		     break;
		   case 'E':
                     UpdateAction(Ntfy+i);
                     Unlock;
                     runjob(Ntfy[i].msg);
                     Lock;
		     break;
		   case 'S': // Splash Message
                     {
                       SPLASHMSG msg;
                       UpdateAction(Ntfy+i);
                       Unlock;
                       pthread_join(Mth,NULL);
                       msg.xsize=500;
                       msg.ysize=60;
                       msg.xpm=NULL;
                       msg.message=Ntfy[i].msg;
                       msg.font =23;
                       msg.fontcolor=50;
                       msg.bkcolor = 0xf08f8f8f;
                       kgSplashDia(-1,-1,500,60,NULL,Ntfy[i].msg,msg.font,msg.fontcolor,msg.bkcolor);
 //                      pthread_create(&Mth,NULL,SplashThread,&msg);
                       Lock;
                     }

		     break;
		   case 'P':
                     {
                       pthread_t Sth;
                       SPLASHMSG msg;
                       UpdateAction(Ntfy+i);
                       Unlock;
                       Writedb();
		       Beep();  
                       msg.xsize=500;
                       msg.ysize=60;
                       msg.xpm=NULL;
                       msg.message=(char *)"It is time to shutdown the machine";
                       msg.font =23;
                       msg.fontcolor=0;
                       msg.bkcolor = 0xf08f8f8f;
                       SayText((char *)"It is time to shutdown the machine");
//                       kgSplashDia(500,60,NULL,"It is time to shutdown the0 machine");
                       pthread_create(&Sth,NULL,RunkgSplashDia,&msg);
		       ThreadSleep(10,0);
                       Lock;
                       system("/sbin/poweroff");
                     }
		     break;
		   default:
		     break;
		 }
		 Wait= 5;
		 break;
	      }
	    }
	  }
	  else {
	      Ntfy[i].status = 0;
	  }
	  i++;
	}
        Unlock;
        ThreadSleep(Wait,0);
     }
     return NULL;
}
static int GetDateTime(void) {
   Day = Date();
   Curmin = curtime_in_mins();
   return Curmin;
}
void * notify(void *arg) {
	char buff[300];
	if(arg != NULL ) {strcpy(Pgpath,(char *)arg);strcat(Pgpath,(char *)"/");}
	else {
           strcpy(Pgpath,getenv("HOME"));
           strcat(Pgpath,"/.kgclock/");
        }
	strcpy(dbsfile,Pgpath);
	strcat(dbsfile,"notify.dbs");
	strcpy(Audfile,Pgpath);
	strcat(Audfile,"audiofile");
//	TakeActions(NULL);
	return NULL;
}
