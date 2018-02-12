#include <stdio.h>
int main(void) {
  int ir,count=0;
  char sig[10]={"$$$$$$$$"};
  char buff[2000];
  FILE *fp,*ip;
  fp = fopen("clock","a");
//  fwrite(sig,1,8,fp);
  ip= fopen("beep.wav","r");
  if(ip==NULL) return 1;
  count=0;
  while ((ir=fread(buff,1,1024,ip)) ==1024) {fwrite(buff,1,1024,fp);count++;}
  fwrite(buff,1,ir,fp);
  printf("Wrote: %d bytes\n",count*1024+ir);
//  fwrite(sig,1,8,fp);
  fclose(ip);
  ip= fopen("phone.wav","r");
  if(ip==NULL) return 1;
  count=0;
  while ((ir=fread(buff,1,1024,ip)) ==1024) {fwrite(buff,1,1024,fp);count++;}
  fwrite(buff,1,ir,fp);
  printf("Wrote: %d bytes\n",count*1024+ir);
//  fwrite(sig,1,8,fp);
  fclose(ip);
  fclose(fp);
  return 0;
}
