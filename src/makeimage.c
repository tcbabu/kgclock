#include <kulina.h>

void *MakePixmapImage(int xl,int yl,int r,int g,int b,float fac) {
   void *Img,*fid,*Img1;
   float h,s,v,rf,gf,bf,vm;
   fid = kgInitImage(xl,yl,4);
   kgUserFrame((DIG *)fid,-2.,-2.,(float)xl+2,(float)yl+2);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
   vm = 1.2*v;
   if(vm > 1.0) vm=1.0;
   HSVtoRGB(&rf,&gf,&bf,h,s,vm);
   kgChangeColor((DIG *)fid,252,(int)rf,(int)gf,(int)bf);
   HSVtoRGB(&rf,&gf,&bf,h,s,0.6*v);
   kgChangeColor((DIG *)fid,253,(int)rf,(int)gf,(int)bf);
   kgChangeColor((DIG *)fid,251,(int)r,(int)g,(int)b);
   kgRoundedRectangleFill((DIG *)fid,(float)xl*0.5,(float)yl*0.5,(float)xl+2,(float)yl+2,0,253,fac);
   HSVtoRGB(&rf,&gf,&bf,h,s,0.9*v);
   kgChangeColor((DIG *)fid,253,(int)rf,(int)gf,(int)bf);
   kgRoundedRectangleFill((DIG *)fid,(float)xl*0.5,(float)yl*0.5,(float)xl+1,(float)yl+1,0,253,fac);
   kgRoundedRectangleFill((DIG *)fid,(float)xl*0.5,(float)yl*0.5,(float)xl,(float)yl,0,252,fac);
   kgRoundedRectangleFill((DIG *)fid,(float)xl*0.5,(float)yl*0.5,(float)xl-4,(float)yl-4,0,251,fac);
   Img = kgGetSharpImage(fid);
   kgCloseImage(fid);
   return Img;
}
