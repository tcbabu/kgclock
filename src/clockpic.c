#include <kulina.h>
#include <math.h>
extern int DateColor;
extern int Dred,Dgreen,Dblue;

int DrawMark(DIG *G,double angle,double dangle,double r,double l,double xoff,double yoff,int color) {
  double x,y,Ang,R,cs,ss,ang;
  float xp[4],yp[4];
  ang = angle+dangle*0.5;
  R =r-l;
  cs = cos(ang);
  ss = sin(ang);
  xp[0] = R*cs+xoff;
  yp[0] = R*ss+yoff;
  xp[1]= r*cs+xoff;
  yp[1]= r*ss+yoff;
  ang -=dangle;
  cs = cos(ang);
  ss = sin(ang);
  xp[2]= r*cs+xoff;
  yp[2]= r*ss+yoff;
  xp[3]= R*cs+xoff;
  yp[3]= R*ss+yoff;
  kgPolyFill(G,4,xp,yp,0,color);
  return 1;
}
int Draw_Circle(DIG *G,double angle,double r,double l,int color) {
  double Ang,R,cs,ss,ang;
  float xp[4],yp[4],x,y;
  ang = angle;
  R =r-l*0.5;
  cs = cos(ang);
  ss = sin(ang);
  x = R*cs;
  y = R*ss;
  kgRoundedRectangleFill(G,x,y,(float)l,(float)l,0,color,0.5);
  return 1;
}
void *DrawClockPicture(double r){
  double dang=3.0;
  void *img;
  DIG *G;
  int L,i;
  double l,fac,ang,dl;
  float h,s,v,vorg;
  float Red,Green,Blue;
  L = r*0.5+1;
  l= L;
  dl = l*0.15;
  fac = atan(1.0)/45.0;
  

  G = (DIG *)kgInitImage(2*L+1,2*L+1,8);
  kgUserFrame(G,-l,-l,l,l);
  ang =0.0;
  dang *= fac;
  kgLineColor(G,3);
  RGBtoHSV(Dred,Dgreen,Dblue,&h,&s,&vorg);
  v =0.75*vorg;
  HSVtoRGB(&Red,&Green,&Blue,h,s,v);
  kgChangeColor(G,251,30,30,30);
  kgChangeColor(G,253,(int)Red,(int)Green,(int)Blue);
   
  if(vorg < 0.9 ) {
       v = vorg*1.4;
       if(v>1.0) v=1.0;
       HSVtoRGB(&Red,&Green,&Blue,h,s,v);
       kgChangeColor(G,252,(int)Red,(int)Green,(int)Blue);
  }
  else kgChangeColor(G,252,250,250,250);
  ang =0.0;
  for(i=0;i<12;i++) {
    DrawMark(G,ang,dang,l,dl,+2.,-2.,251);
    ang += (30*fac);
  }
  ang =0.0;
  for(i=0;i<12;i++) {
    DrawMark(G,ang,dang,l,dl,-1.,0.0,252);
    ang += (30*fac);
  }
#if 1
  ang =0.0;
  for(i=0;i<12;i++) {
    DrawMark(G,ang,dang,l,dl,+1.0,-1.0,253);
    ang += (30*fac);
  }
#endif
  ang =0.0;
  for(i=0;i<12;i++) {
    DrawMark(G,ang,dang,l,dl,0.,0.,DateColor);
    ang += (30*fac);
  }
  img = kgGetResizedImage(G);
  
  kgCloseImage(G);
  return img;
}
#if 0
int main(void) {
  void *img;
  img = DrawClockPicture(300);
  kgWriteImage(img,"clock.png");
}
#endif
