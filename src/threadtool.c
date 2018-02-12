#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>

#define BUSY 1
#define FREE 0
static int **PIPES;
static int **RPIPES;
static pthread_mutex_t Thlock;
static int nCores;
typedef struct _Thaction
{
    void *(*threadFunc)(void *);
    void *arg;
}Thaction;
typedef struct _Thread
{
    pthread_t id;
    int num;
    int status;
#if 0
    sem_t sem;
    void *(*threadFunc)(void *);
    void *arg;
#endif
}Thread; 
static int ActSize; 
static Thread *threads;
static int *Thds;
int getCores();
void init_threads(int thds);
void do_it_any_thread(void *(*threadFunc)(void *),void *arg);
void wait_threads();
void *threadFunc(void *arg);
int getCores()
{
    FILE *fp;
       char line[100];
       int cores=0;
       fp=fopen("/proc/cpuinfo","r");
       if(fp==NULL)
       {
               fprintf(stderr,"Unable to get coreinfo! Assuming one core!\n");
               nCores=1;
       }
       while(!feof(fp))
       {
               fgets(line,100,fp);
               if(strncmp(line,"processor",9)==0) cores++;
       }
       printf("CORES AVAILABLE: %d\n",cores);
       return cores;
} 
void init_threads(int thds)
{
    int i;
       nCores = thds;
//       pthread_mutex_init(&Thlock,NULL);
       threads=(Thread *)malloc(sizeof(Thread)*nCores);
       PIPES = (int **) malloc(sizeof(int *)*nCores);
       RPIPES = (int **) malloc(sizeof(int *)*nCores);
       for(i=0;i<nCores;i++) {
          PIPES[i]= (int *)malloc(sizeof(int)*2);
          RPIPES[i]= (int *)malloc(sizeof(int)*2);
          pipe(PIPES[i]);
          pipe(RPIPES[i]);
       }
       if(threads==NULL)
       {
         printf("Error allocating memory\n");
         exit(1);
       }
       ActSize = sizeof(Thaction);
    
       for(i=0;i<nCores;i++)
       {
            threads[i].num=i;
            pthread_create(&(threads[i].id),NULL,threadFunc,threads+i);
       }
}
void do_it_any_thread(void *(*threadFunc)(void *),void *arg)
{
       Thaction job;
       static int nTh=0;
       job.threadFunc=threadFunc;
       job.arg = arg;
       write(PIPES[nTh][1],&job, ActSize);
       nTh++;
       nTh %= nCores;
}
void wait_threads()
{
    int i,count;
    Thaction job,rjob;
    job.threadFunc=NULL;
    job.arg =NULL;

#if 0
       Thaction *job;
       while(1) {
         pthread_mutex_lock(&Thlock);
         Resetlink(Jobq);
         job = (Thaction *)Getrecord(Jobq);
         pthread_mutex_unlock(&Thlock);
         if(job == NULL) break;
       }
#endif
       for(i=0;i<nCores;i++) write(PIPES[i][1],&job, ActSize);
       for(i=0;i<nCores;i++) read(RPIPES[i][0],&rjob, ActSize);
#if 0
       while(1) {
         count =0;
         for(i=0;i<nCores;i++)
         if( threads[i].status== BUSY ) count++;
         if(count==0 ) break;
         usleep(10);
       }
       while(1) {
         count =0;
         for(i=0;i<nCores;i++)
         if( threads[i].status== BUSY ) count++;
         if(count==0 ) break;
       }
#endif
} 
void *threadFunc(void *Th)
{
   Thaction job;
   Thread *th;
   int num;
   th = (Thread *)Th;
   num = (th)->num;
   while(1) {
          read(PIPES[num][0],&job, ActSize);
          th->status=BUSY;
          if(job.threadFunc == NULL) write(RPIPES[num][1],&job,ActSize);
          else job.threadFunc(job.arg);
          th->status=FREE;
//           printf("Executed job\n");
   }
} 
