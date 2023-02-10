#ifndef GL_H
#define GL_H
#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define MAXITEMS 30
#define MAXTITEMLN 500
#define MAXTPRMTLN 150
#define LEFT -1
#define CENTRE 0
#define RIGHT 1
#define TRUE 1
#define FALSE 0
#define SCROLL_DOWN -101
#define SCROLL_UP -201
#define LN_WIDTH 20

typedef struct _thumbnails {
  void **xpms;
  char **menu;
  int *sw;
} THUMBNAILS;
typedef struct _thumbnail {
  char *name;
  void *img;
  int sw;
  int id;
  int state;
} ThumbNail;
typedef struct butn_str {
        int x1; 
        int y1;
        int x2;
        int y2;
        char title[200];
        void *xpmn;
        void *xpmp;
        void *xpmh;
        int bkgr; /* back ground colours */
        int sw;
        unsigned int butncode;
        int state;
        int highli;
        void *imgn;
        void *imgp;
        void *imgh;
        void *D;
        void *Widget;
        int butno;
        void *Bimg0;
        void *Bimg;
} BUT_STR;
typedef struct _splashmsg {
  int xsize,ysize;
  char *xpm,*message;
  int font;int fontcolor;unsigned long bkcolor;
} SPLASHMSG;
typedef struct {
  int char_clr;     /* character colour */
  int fill_clr;     /* fill colour */
  int bodr_clr;     /* border colour */
  int  cur_clr;     /* cursor colour for text box*/
  int high_clr;     /* menu item high light colour */
  int char_hclr;    /* high light colour for character in menu item*/
  int msg_char;     /* character colour for message */
  int msg_fill;     /* fill colour for message */
  int msg_bodr;     /* border colour for message */
  int txt_fill;     /* fill colour for text entry box */
  int txt_char;     /* character colour for text entry box */
  int txt_pchar;    /* colour for text entry box prompt */
  int tabl_fill;    /* colour for table box fill */
  int tabl_line;    /* colour for table box lines */
  int tabl_char;    /* colour for table box characters */
  int tabl_hchar;   /* colour for table box characters(output only) */
  int v_dim;        /* very dim colour */
  int dim;          /* dim colour for 3d effect*/
  int bright;       /* bright colourfor 3d effect */
  int vbright;      /* colour for key board attention */
  int twin_fill;    /* fill colour for text window */
  int twin_char;    /* character colour for text window */
  int twin_bodr ;   /* border colour for text window */
  int info_fill;    /* fill colour for info box */
  int info_char;    /* character colour for info box */
  int but_char;     /* character colour for button title */
  int menu_char;    /* Color for menu item */
  int c_bound;
  int GuiFontSize;
  int MenuFont;
  int PromptFont;
  int ButtonFont;
  int MsgFont;
  int Font;   //internal use
  int FontSize;   //internal use
  int SplashFont;
  int SplashFillColor;
  int SplashBodrColor;
  int SplashCharColor;
  int ProgFillColor;
  int ProgBodrColor;
  int ProgColor;
  int ItemHighColor;
  int InputFontSize;
} Gclr;

typedef struct dia_msg_str {
      char code;
      int x1;
      int y1;
      int x2;        /* bottom right corner */
      int y2; 
      int justification;/*-1,0,1 */
      int hide;   // =1 not shown
      char Wid[50];
      char msg[500];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *D;
      void *Bimg;
} DIM;
typedef struct t_elmt {
     char *fmt;void *v ;int sw;
     int noecho;
     void *img;
     int x1;
     int y1;
     int x2;
     int y2;
     char pr[MAXTPRMTLN];
     char df[MAXTITEMLN];
     int cursor;
     int startchar;
     int ln;
     int hlt,hxs,hxe; // if hlt!= 0 highlight hxs to hxe
} T_ELMT;
typedef struct tbl_elmt { 
     char *fmt;void *v ;int sw;
     int noecho;
     void *img;
     int x1;
     int y1;
     int x2;
     int y2;
     char pr[MAXTPRMTLN];
     char df[MAXTITEMLN];
     int cursor;
     int startchar;
     int ln;
     int hlt,hxs,hxe; // if hlt!= 0 highlight hxs to hxe
} TBL_ELMT;
typedef struct dai_tx_box {
      char code;     /* use 't' for text box and 'T' for table box*/
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int width;     /* width of each box */
      int nx;        /* box distribution in x direction */
      int ny;        /* box distribution in y direction */
      T_ELMT *elmt;  /* nx*ny items of T_ELMT type    */
      int row;       /* position of cursor */
      int col;
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
                     /* Routine to Update */
      int bordr;  // =0 means no border
      int hide;   // =1 not shown
      int Font;
      int FontSize;
      char Wid[50];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *tstr;
      void *D;
      void **pt;  // pointer to internal allocations
      int type;
      void *Bimg;
} DIT;
typedef struct dai_buttons {
      char code;     /* use 'b' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int *df;       /* default button and return value */
      int *sw;       /* swich */
      char **titles; /* button names (nx*ny) values */
      char *butncode;
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      void **xpm;     /* pointer to xpm data */
      int *bkgr;      /* back ground colours */
      int type;  // Button Type
      float fac;  // fact for rounding <0., 0.5 >
      int bordr;
      int hide;   // =1 not shown
      char Wid[50];
      int bval;
      int ptr;       /* to be filled in by dialog */
      int item;
      void *buts;
      void *D;
      void *Bimg;
} DIB;
typedef struct dai_h_bar {
      char code;     /* use 'h' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int type;  // Button Type
      float fac;  // fact for rounding <0., 0.5 >
      int bordr;
      int hide;   // =1 not shown
      int nodrawbkgr;
      int bval;
      void *buts;
      int (*Update)(int ret,int i,void *d);
      void *arg;     /* Any extra information for Update to use*/
      char Wid[50];
      int *df;       /* default button and return value */
      int ptr;       /* to be filled in by dialog */
      int item;
      void *D;
      void *Bimg;
} DIL;
typedef struct dai_buttonsn {
      char code;     /* use 'n' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int type;  // Button Type
      float fac;  // fact for rounding <0., 0.5 >
      int bordr;
      int hide;   // =1 not shown
      int nodrawbkgr;
      int bval;
      void *buts;
      int (*Update)(int ret,int i,void *d);
      void *arg;     /* Any extra information for Update to use*/
      char Wid[50];
      int *df;       /* default button and return value ;internal*/
      int ptr;       /* to be filled in by dialog */
      int item;
      void *D;
      void *Bimg;
} DIN;
typedef struct dai_buttons_new {
      char code;     /* use 'n' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int *df;       /* default button and return value */
      char *prompt;  /* prompt string */
      char **titles; /* button names (nx*ny) values */
      char *butncode;
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      void **xpm;     /* pointer to xpm data */
      int *bkgr;      /* back ground colours */
      int type;  // Button Type
      float fac;  // fact for rounding <0., 0.5 >
      int bordr;
      int hide;   // =1 not shown
      char Wid[50];
      int bval;
      int ptr;       /* to be filled in by dialog */
      int item;
      void *buts;
      void *D;
      void *Bimg;
} DIBN;
typedef struct dai_h_bar_new {
      char code;     /* use 'H' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int *df;       /* default button and return value */
      int *sw;       /* state of button */
      char **titles; /* button names (nx*ny) values */
      char *butncode;
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      void **xpm;     /* pointer to xpm data */
      int *bkgr;      /* back ground colours */
      int type;  // Button Type
      float fac;  // fact for rounding <0., 0.5 >
      int bordr;
      int hide;   // =1 not shown
      char Wid[50];
      int bval;
      int ptr;       /* to be filled in by dialog */
      int item;
      void *buts;
      void *D;
      void *Bimg;
} DILN;
typedef struct dai_f_bar {
      char code;     /* use 'f' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      double min;    /* lower limit */
      double max;    /* upper limit */
      int lngth;     /* length of slide bar */
      double *df;    /*default value; returns final value */
      char *prompt;  /* prompt string */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(double val,int i,void *d);
      int bordr;
      int hide;   // =1 not shown
      int type;
      char Wid[50];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *sptr;
      void *D;
      double val;
      void *Bimg;
} DIF;
typedef struct dai_d_bar {
      char code;     /* use 'd' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int min;      /* lower limit */
      int max;      /* upper limit */
      int lngth;     /* length of slide bar */
      int *df;      /*default value; returns final value */
      char *prompt;  /* prompt string */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int val,int i,void *d);
      int bordr;
      int hide;   // =1 not shown
      int type;
      char Wid[50];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *sptr;
      void *D;
      int val;
      void *Bimg;
} DID;
typedef struct dai_hb_bar {
      char code;     /* use 'P' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int min;      /* lower limit */
      int max;      /* upper limit */
      int lngth;     /* length of slide bar */
      int *df;      /*default value; returns final value */
      char *prompt;  /* prompt string */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int val,int i,void *d);
      int bordr;
      int hide;   // =1 not shown
      int type;
      int sldclr;
      char Wid[50];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *sptr;
      void *D;
      int val;
      int direction;
      void *Bimg;
} DIHB;
typedef struct dai_brw {
      char code;     /* use 'w' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int size;      /* size of portion visible */
      int *df;      /*default value; returns final value */
      char *prompt;  /* prompt string */
      char **menu;   /* last item should be set as NULL */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      int hide;   // =1 not shown
      int width;
      int offset;
      int w;
      int itemhi;
      int bordr;
      int bkgr;
      char Wid[50];
      int nitems;
      int xb;
      int ptr;       /* to be filled in by dialog */
      int item;
      void *bwsr;
      void *D;
      int val;
      void **imgs;
      void *Bimg;
} DIW;
typedef struct dai_scrollmsg { //Menu
      char code;     /* use 's' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int size;      /* size of portion visible */
      int *df; /* Not used NULL */
      char *prompt;  /* not used NULL */
      char **menu;   /* last item should be set as NULL */
      void *arg;     /* not used */
      int (*Update)(int ret,int i,void *d); /* NULL not used */
      int width; // line width
      int offset; //offset from border ; not used now
      int w;  // scroll width
      int itemhi;
      int bordr;
      int bkgr;
      int hide;   // =1 not shown
      char Wid[50];
      int nitems;
      int ptr;       /* to be filled in by dialog */
      int item;
      void *bwsr;
      void *D;
      void *Bimg;
} DIS;
typedef struct dai_menu { //Menu
      char code;     /* use 'e' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int size;      /* size of portion visible */
      int *df;      /*default value; returns final value */
      char *prompt;  /* prompt string ; not used*/
      char **menu;   /* last item should be set as NULL */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      int width;
      int offset;
      int w;
      int itemhi;
      int bordr;
      int bkgr;
      int hide;   // =1 not shown
      char Wid[50];
      int nitems;
      int ptr;       /* to be filled in by dialog */
      int item;
      void *bwsr;
      int type;
      void *D;  // pointer to Dialog
      int val;
      void **imgs;
      void *Bimg;
} DIE;
typedef struct dai_inf {
      char code;     /* use 'i' */
      int x1;        /* top left corner */
      int y1;
      int x2;
      int y2;
      int linewidth;
      int bordr;
      int hide;   // =1 not shown
      int nchrs;     /* number of characters */
      int nlines;
      char Wid[50];
      void *twin;    /* be filled by dialog */
      void *D;
      int item;
      void *Bimg;
} DII;
typedef struct dai_pgbar {
// progress bar
      char code;     /* use 'o' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int hide;   // ==1 not visible
      int percent;      /*pecentage val */
      int bordr;  // border color -1 default
      int type;  
      int clr;  // progress color -1 default
      int direction;
      char Wid[50];
      void *D;  // pointer to Dialog
      int item;
      void *imgs; // internal
      int oldval;  // internal
      void *Bimg;
} DIO;
typedef struct dai_vsbar {
// Vertical scroll bar
      char code;     /* use 'v' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int hide;   // ==1 not visible
      double ds;   // size of visible portion; percentage
      double df;      /*default pos; returns final value */
      double mvmt;
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(double val,int i,void *d);
      char Wid[50];
      int bordr;
      int type;   // Not used
      int offset,w;
      int tbx1,tby1,tbx2,tby2;
      int bbx1,bby1,bbx2,bby2;
      int rsx1,rsy1,rsx2,rsy2;
      void *D;  // pointer to Dialog
      int item;
      void *Bimg;
} DIV;
typedef struct dai_hsbar {
// Horizontal scroll bar
      char code;     /* use 'z' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int hide;   // ==1 not visible
      double ds;   // size of visible portion; percentage
      double df;      /*default pos; returns final value */
      double mvmt;
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(double val,int i,void *d);
      char Wid[50];
      int bordr;
      int type;   // Not used
      int offset,w;
      int lbx1,lby1,lbx2,lby2;
      int rbx1,rby1,rbx2,rby2;
      int rsx1,rsy1,rsx2,rsy2;
      void *D;  // pointer to Dialog
      int item;
      void *Bimg;
} DIZ;
typedef struct dai_xbrw {
// used for new type browser with selection and image
      char code;     /* use 'x' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int nxsize;    /* presently nxsize=nx */
      int size;      /* size of portion visible */
      int *df;      /*default value; returns final value */
      char *prompt;  /* prompt string */
      void **list;   /* last item should be set as NULL */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      int offset; //border offset
      int w;    // width for scroller
      int type;  // Browser Type 0: mulit-select Otherwise select one
      int itemhi; // highlight keyboard attention
      int bordr;
      int bkgr;
      int hide;  // ==1 notvisible
      char Wid[50];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *bwsr;
      int nitems;
      int curpos;
      void *D;  // pointer to Dialog
      int val;
      void **pt; // may be NULL; if not null it will be freed by Cleanup Routine
      void **imgs;
      void *nimg,*himg;
      void *Bimg;
} DIX;
typedef struct dai_ybrw {
// used for new type browser with selection and image
      char code;     /* use 'y' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int nxsize;    /* presently nxsize=nx */
      int size;      /* size of portion visible */
      int *df;      /*default value; returns final value */
      char *prompt;  /* prompt string */
      void **list;   /* last item should be set as NULL */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      int offset; //border offset
      int w;    // width for scroller
      int type;  // Browser Type 0: mulit-select Otherwise select one
      int itemhi; // highlight keyboard attention
      int bordr;
      int bkgr;
      int hide;  // ==1 notvisible
      char Wid[50];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *bwsr;
      int nitems;
      int curpos;
      void *D;  // pointer to Dialog
      int val;
      void **pt; // may be NULL; if not null it will be freed by Cleanup Routine
      void **imgs;
      void *nimg,*himg;
      void *Bimg;
} DIY;
typedef struct dai_chkbut {
// used for new type browser with selection and image
      char code;     /* use 'c' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int nxsize;    /* presently nxsize=nx */
      int size;      /* size of portion visible */
      int *df;      /*default value; returns final value */
      char *prompt;  /* prompt string */
      void **list;   /* last item should be set as NULL */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      int offset; //border offset
      int w;    // width for scroller
      int type;  // Browser type
      int itemhi; // highlight keyboard attention
      int bordr;
      int bkgr;
      int hide;  // ==1 notvisible
      char Wid[50];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *bwsr;
      int nitems;
      int curpos;
      void *D;  // pointer to Dialog
      int val;
      void **imgs;
      void *nimg,*himg;
      void *Bimg;
} DICH;
typedef struct dai_radio {
// used for new type browser with selection and image
      char code;     /* use 'r' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      int xgap;      /* X dir. gap */
      int ygap;      /* Y dir. gap */
      int lngth;     /* length of each button */
      int width;     /* width of each button */
      int nx;        /* button distribution in x direction */
      int ny;        /* button distribution in y direction */
      int nxsize;    /* presently nxsize=nx */
      int size;      /* size of portion visible */
      int *df;      /*default value; returns final value */
      char *prompt;  /* prompt string */
      void **list;   /* last item should be set as NULL */
      void *arg;     /* Any extra information for Update to use*/
      int (*Update)(int ret,int i,void *d);
      int offset; //border offset
      int w;    // width for scroller
      int type;  // Broser type
      int itemhi; // highlight keyboard attention
      int bordr;
      int bkgr;
      int hide;  // ==1 notvisible
      char Wid[50];
      int ptr;       /* to be filled in by dialog */
      int item;
      void *bwsr;
      int nitems;
      int curpos;
      void *D;  // pointer to Dialog
      int val;
      void **imgs;
      void *nimg,*himg;
      void *Bimg;
} DIRA;
typedef struct dai_pix {
      char code;     /* use 'p' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      void *xpm;     /* pointer to image info */
      int bkgr_clr; /* -ve means back ground color is the fill color */
      int bordr;
      int hide;   // =1 not shown
      float transparency;
      char Wid[50];
      void *D;
      int item;
      void *Bimg;
} DIP;
typedef struct dai_grf {
      char code;     /* use 'g' */
      int x1;        /* top left corner */
      int y1;
      int x2;        /* bottom right corner */
      int y2;
      void *xpm;     /* pointer to xpm to root */
      int bkgr_clr; /* -ve means back ground color setting is ignored */
      void (*initgraph)(int i,void *D); /* for graphics initialisation */
      void *data;   /* for any data for graphics */
      int bordr;
      int hide;   // =1 not shown
      char Wid[50];
      int vx1,vy1,vx2,vy2; /* actual viewport data; filled by system */
      float vxmin,vymin,vxmax,vymax; /* orginal viewport data; filled by system */
      unsigned char *Rbuff;
      int  BACK_UP,OPEN,Byte,rbuf,D_ON,R_Byte,R_max,hbuf,B_min,cmdbuf,review,Obj_opn;
      float w[4];
      void *D; //Pointer to Parent Dialog;
      void *dc;
      void *wc; // pointer to WC
      void *img; // internal
      void *rzimg; // internal
      void *pixels; //internal
      int MAG;
      int item;
      void *glWindow; /* added for OpenGL; sub window */
      void *cMain;
      int glDouble;  // gl doublebuffer check
      void *Bimg;
} DIG;
typedef struct _GboxInfo {
      float vxmin,vymin,vxmax,vymax; /* orginal viewport data; filled by system */
      float FM[16],TM[16];
      float w[4];
      int ZBUFF;
      int SET3D;
      float xmin,ymin,zmin,xmax,ymax,zmax; // 3d box
} GBOXINFO;
typedef union dia_u { DIT *t;DIB *b;DIN *N;DIF *f;DID *d;DIBN *n;DIO *o;
                      DIV *v;DIZ *z; DIW *w;DIM *m;DIX *x;DIY *y;DIL *h;
                      DILN *H;DIP *p; DIG *g;DII *i;DIE *e;DIS *s;DIHB *B;
                      DICH *c;DIRA *r; 
} DIA;
typedef struct _kbevent {
 int event; /* 0:mouse movement,1:button press,2:button release,3:mouse move+button press,
               4: key press, 5:key release
            */
 int button;  /* button number */
 int button_state;  /* button state for mouse movement 0,1,2,4 (bit status)*/
 int key;    /* key number */
 int x, y; /* mouse position */
 int rootx,rooty; /* /*mouse posion in root window */
} KBEVENT;
typedef struct Dia_str {
  int xo, yo;      /* lower left corner */
  int xl,yl;       /* length and width */
  DIA *d;          /* DIA union */
  int bkup;        /* 1,0 to backup screen or not */
  int bor_type;    /* code for border type */
  int tw,lw,rw,bw; /* width of top,left,right and bottom border */
  int df;
  int (*Initfun)(void *); /* init function for new version */
  int kbattn;
  int butattn;
  Gclr gc;
  int Hsize;
  int evgax,evgay;
  KBEVENT kb;
  char name[300];
  int fullscreen;
  char *tmpdir;
  int Deco; /* 1 for window decorations ; 0 otherwise */
  int DrawBkgr; /* 1/0; 1 for drawing background; 0 otherwise */
  void *Bkpixmap; /* NULL FOR Nothing */
  int (*Callback) (void *,void *kbe); /* NULL FOR Nothing */
  float transparency;
  int Newwin; // 1 for newwindow and 0 for Sub dialog
  int Sticky;
  int Fixpos;
  int NoTaskBar;
  void * wc;
  void * parent;  // parent Dialog
  int entry;  // temp variable for box_cursor
//  int PON_X,PON_Y,KEY;
  int PON_X,PON_Y;int VerId;
  void *pt; // pointer to pass any info the user needs
  int StackPos; /* -1 below; 1:above; otherwise normal */
  int Resize;  /* 1 means resizable ; otherwise not */
  int MinWidth;
  int MinHeight;
  int (*ResizeCallback) (void *);  /* Callback on resize event */
  void *Shapexpm;
  void *SearchList;
  void *GrpList;
  int controls;
  int CurWid;
  int TotWid;
  int (*Cleanupfun) (void *);  /* Callback on finish */
  int (*WaitCallback) (void *);  /* Callback on resize event */
  pthread_mutex_t Lock;
  int KILL;  // internal if set to 1000 gui will be closed
  int SUBWIN; // = 1 means subwindow if parent==NULL; 0 pixmap is used
  int NoWinMngr; // if set to 1 it is assumed that no window manager is active
  int Maxl,Maxw; // for further development to include bigger size than xl and yl
  void *PWIN; // NULL, if not set as NULL it will be used as parent Window
  void *ThInfo;  // internal
  void *Kbrd; //for keyboard 
  int InputWid;
} DIALOG;

typedef struct _WidgetGroup {
  void *wlist;
  void *arg;
  int hide;
  int (*CleanupGrp)(void *);
} WIDGETGRP;
typedef struct _jpegimg {
  char Sign[4]; // Sign is "JPG"
  int incode;
  char flname[100];
  int row;
  int col;
  int colors;
  char **buf;
  int *matchcol;
} JPGIMG;
typedef struct _fmgimg {
  char Sign[4]; // Sign is "FMG"
  int incode;
  char flname[120];
  int size;
  unsigned char *image_data;
  void *image;
} FMGIMG;
typedef struct _PngImage {
  char Sign[4]; // Sign is "PNG"
  int incode;
  char flname[120];
  unsigned long image_width ,image_height ,image_rowbytes;
  int  image_channels;
  unsigned char *image_data;
  void *image; // not use for pure PNG, it is Image for gm converted
} PNGIMG;
typedef struct _gmImage {
  char Sign[4];  // Sign is "IMG" ; any file can be converted to IMG
  int incode;
  char flname[500];
  unsigned long image_width ,image_height ,image_rowbytes;
  int  image_channels;
  unsigned char *image_data; // Not used
  void *image;
  int xoffset,yoffset;
  int bkgrclr;
  float rzfac;
  void *info;
  void *exce;
} GMIMG;

void opntwin(DIALOG *D,int ix,int iy,int chrs,int lines);
void Opntwin(DIALOG *D,int ix,int iy,int chrs,int lines,int xbdr,int ybdr);
void clstwin(DIALOG *D);
void gprintf(DIALOG *D, char *s);
int  gscanf(void *D,void *unknown,...);
void twinmove(DIALOG *D,int col,int row);
void Print_gui_data(DIALOG *D,char *flname);
int  Runprinterdia(void *parent);
char **fontnames(void);

typedef int (*CALLBACK)(int,int,void *);

int  kgGetVersion(void);
int  kgUi(DIALOG *D);
void kgInitUi(void *Tmp);
void kgCleanUi(void *tmp);
int  kgOpenGrp(void *Tmp);
int  kgAddtoGrp(void *tmp,int grpid,void * Widget);
int  kgSetGrpCleanup(void *tmp,int grpid,int (*Cleanup)(void *));
int  kgSetGrpVisibility(void *Tmp,int grpid,int val);
int  kgUpdateGrp(void *Tmp,int grpid);
int  kgMoveGrp(void *Tmp,int grpid,int x1,int y1);
int  kgShiftGrp(void *Tmp,int grpid,int xs,int ys);
int  kgResizeGrp(void *Tmp,int grpid,int xl,int yl);
int  kgMoveWidget(void *Tmp,int item,int x1,int y1);
int  kgShiftWidget(void *Tmp,int item,int xsh,int ysh);
int  kgResizeWidget(void *Tmp,int item,int xl,int yl);
void *kgGetWidget(void *Tmp,int id);
void * kgGetNamedWidget(void *Tmp,char *name);
int  kgGetWidgetId(DIALOG *D,void *Widget);
void * kgGetWidgetName(void *Tmp,int id);
void * kgGetCurrentWidgetName(void *Tmp);
void * kgGetCurrentWidget(void *Tmp);
void * kgGetClickedWidget(void *Dtmp);
void * kgGetLocationWidget(void *Dtmp,int x1,int y1);
int  kgSetCurrentWidget(void *Tmp,int Wid);
int  kgSetDefaultWidget(void *Tmp,int Wid); // to be used only in Ui init function
int  kgSetWidgetVisibility(void *Widget,int vis);
int  kgGetWidgetVisibility(void *Widget);
int  kgUpdateTextBox(void *tmp,int no);
void kgUpdateWidget(void *widget);
int  kgDrawWidget(DIALOG *D,void *Widget);
WIDGETGRP * kgGetWidgetGrp(void *tmp,int grpid);
void kgLockUi(void *tmp);
void kgUnlockUi(void *tmp);
int kgSetExit(void *tmp);
void *kgUiFromWindow(void *Dsp,void * Win,void * cmap);
void kgFreeUifromWindow(void *D);

/* interacing with Widget */

float  kgGetfloat(void *Tmp,int tb,int item) ;
float  kgSetfloat(void *Tmp,int tb,int item,float val) ;
double kgGetdouble(void *Tmp,int tb,int item) ;
double kgSetdouble(void *Tmp,int tb,int item,double val);
int    kgGetint(void *Tmp,int tb,int item) ;
int    kgSetint(void *Tmp,int tb,int item,int val) ;
long   kgGetlong(void *Tmp,int tb,int item) ;
long   kgSetlong(void *Tmp,int tb,int item,long val) ;
char * kgGetstring(void *Tmp,int tb,int item) ;
char * kgSetstring(void *Tmp,int tb,int item,char * val) ;
float  kgGetFloat(void *Widget,int item) ;
float  kgSetFloat(void *Widget,int item,float val) ;
double kgGetDouble(void *Widget,int item) ;
double kgSetDouble(void *Widget,int item,double val);
int    kgGetInt(void *Widget,int item) ;
int    kgSetInt(void *Widget,int item,int val) ;
long   kgGetLong(void *Widget,int item) ;
long   kgSetLong(void *Widget,int item,long val) ;
char * kgGetString(void *Widget,int item) ;
char * kgSetString(void *Widget,int item,char * val) ;
void kgSetTextItemEcho(void *Tmp,int item, int echo);
int    kgWrite(void *Widget, char *str) ;
int    kgPrintf(void *Tmp, int infob,char *str) ;
int    kgSplash(void *Tmp,int item,char *msg);
//char * kgGetBrowserString(void *Tmp,int menu);
char   *kgGetSelectedString(void *Tmp);
int    kgGetSelection(void *tmp);
int    kgSetSelection(void *tmp,int val);
int    kgSetSwitch(void *Widget,int item, int val);
int    kgGetSwitch(void *Widget,int item);
void **kgSetList(void *Widget,void **list);
void **kgGetList(void *Widget);
void   kgSetProgressBar(void *Widget,int val);
int kgGetSlideValue(void *tmp);
int kgSetSlideValue(void *tmp,int val);
int kgChangeButtonColor(void *Widget,int butno,int red,int green,int blue) ;
void  kgChangeButtonTitle(void *Widget,int butno,char *title);
char *  kgGetButtonTitle(void *Widget,int butno);
void * kgChangeButtonNormalImage(void *Widget,int butno,void *img) ;
void * kgChangeButtonPressedImage(void *Widget,int butno,void *img) ;
void * kgChangeButtonHighImage(void *Widget,int butno,void *img) ;
void kgChangePixmapImage(void *Widget,void *img);
void kgFreePixmapImage(void *Widget);

/*************************************************************/

int  kgDisplaySize(int *xres,int *yres);
int  kgCheckTrueColor(void);
int  kgCheckCompositor(void);
int  kgMenu(void *parent,int xx1,int yy1,int pos,int df,char **menu,int size);
int  kgMenu1( void *parent,int xo,int yo,int df,char **menu,int size,int (*Fun)(int,void *),void *arg);
int  kgGoBackGround(void);
int  kgExtractBaseName (char *flname,char *basename) ;
void kgCleanBackground(void *D,int xo,int yo,int width,int height,float transparency) ;
void kgUpdateOn(void *D);
void kgUpdateOff(void *D);
int kgCheckAttention(void *D);
int kgMovePointer(void *Tmp,int x,int y);
void kgRaiseWindow(void *D);
void kgLowerWindow(void *D);
void kgMoveWindow(void *tmp,int x,int y);
void kgResizeWindow(void *tmp,int l,int w);
void kgSync(void *Tmp);
int kgSplashDia( int xo,int yo,int xl,int yl,char  *xpm ,char *message,int font,int fontcolor,unsigned long bkcolor);
void *RunkgSplashDia(void *arg);
int kgSplashMessage( void *Parent,int xo,int yo,int xl,int yl,char *message,int font,int fontcolor,int bkcolor) ;
void kgWindowStateAbove(void *Tmp);
void kgWindowStateBelow(void *Tmp);

void * kgRegionImage(void *Tmp,int xmin,int ymin,int xmax,int ymax);
void kgFreeXImage(void *Img);
int kgDragRegion(void *Tmp,int xmin,int ymin,int xmax,int ymax,int *x,int *y);
int kgGetThumbNailItem(DIY *Y,int x,int y);
int kgGetThumbNailItemRectangle(DIY *Y,int item,int *x1,int *y1,int *x2,int *y2);
int kgDragImage(void *Tmp,void *Itmp,int xl,int yl,int xmin,int ymin,int xmax,int ymax,int *x,int *y);
int kgScrollDownThumbNails (DIY *y);
int kgScrollUpThumbNails (DIY *y);
int kgDragThumbNail(DIY *Y,int item,int *x,int *y);
int kgPickImage( void *parent,int xo,int yo,void *pt);
int kgSelectImage( void *parent,int xo,int yo,int ThSize,void *pt);
int kgRedrawDialog(DIALOG *Dialog);
int kgDrawDialog(DIALOG *D);
ThumbNail ** kgMakeThumbNails(char *dir,int size);
ThumbNail **kgStringToThumbNails(char **menu);
ThumbNail ** kgFolderThumbNails(char *dir);
ThumbNail ** kgFileThumbNails(char *dir,char *filter);
void kgFreeThumbNails(ThumbNail **tb);
void * kgOpenBusy(void *arg,int xo,int yo);
void kgCloseBusy(void * id);
void kgAddSearchDir(void *Tmp,char *Dir);
void kgColorTheme(DIALOG *D,unsigned char red,unsigned char green, unsigned char blue);
void kgColorTheme1(DIALOG *D,unsigned char red,unsigned char green, unsigned char blue);
void kgResetColor(void *D,int no,int ir,int ig,int ib); // redefines a color; but will not affect hardcopy

/* Default definitions of color */
void kgDefineColor(int clr,unsigned char red,unsigned char green,unsigned char blue);
void kgGetDefaultRGB(int color,int *r,int *g,int *b);
/*
*/
/* 
  Cursor Related 
*/
void kgHideCursor(DIALOG *D);
void kgVisibleCursor(DIALOG *D);
void kgChangeCursor(DIALOG *D,int n);
/* 
  Event related 
*/
int  kgCheckButtonPress(void *D);
void kgSkipEvents(DIALOG *D);
void kgSkipButtonRelease(DIALOG *D);
KBEVENT  kgSkipMouseMove(DIALOG *D);
int   kgGetTimedEvent(DIALOG *D,KBEVENT *e);
void kgPushBackEvent(DIALOG *D);
int  kgCheckMousePressEvent(DIALOG *D,KBEVENT *kbe);
int  kgCheckEvent(DIALOG *D,KBEVENT *kbe);
int  kgCheckRootEvent(DIALOG *D,KBEVENT *kbe);
KBEVENT kgGetEvent(DIALOG *D);
int  kgCheckEscape(DIALOG *D);
int kgEscape(KBEVENT kb);
int kgLeftarrow(KBEVENT kb);
int kgRightarrow(KBEVENT kb);
int kgUparrow(KBEVENT kb);
int kgDownarrow(KBEVENT kb);
int kgEnter(KBEVENT kb);

int kgSendKeyEvent(void *Tmp,int ch) ;
int kgSendEscapeKeyEvent(void *Tmp) ;
int kgSendClearKeyEvent(void *Tmp);
int kgSendTabKeyEvent(void *Tmp) ;
int kgSendSpaceKeyEvent(void *Tmp) ;
int kgSendDeleteKeyEvent(void *Tmp) ;
int kgSendInsertKeyEvent(void *Tmp) ;
int kgSendPageupKeyEvent(void *Tmp) ;
int kgSendPagedownKeyEvent(void *Tmp) ;
int kgSendHomeKeyEvent(void *Tmp) ;
int kgSendEndKeyEvent(void *Tmp) ;
int kgSendBackspaceKeyEvent(void *Tmp) ;
int kgSendLeftKeyEvent(void *Tmp) ;
int kgSendRightKeyEvent(void *Tmp) ;
int kgSendUpKeyEvent(void *Tmp) ;
int kgSendDownKeyEvent(void *Tmp) ;
int kgSendEnterKeyEvent(void *Tmp) ;
int kgSendLinefeedKeyEvent(void *Tmp) ;
int kgSendControlKeyEvent(void *Tmp,int ch);

int kgSendKeyToWindow(void *Tmp,void *wtmp,int ch);
int kgSendTabKeyToWindow(void *Tmp,void *win) ;
int kgSendSpaceKeyToWindow(void *Tmp,void *win) ;
int kgSendDeleteKeyToWindow(void *Tmp,void *win) ;
int kgSendInsertKeyToWindow(void *Tmp,void *win) ;
int kgSendPageupKeyToWindow(void *Tmp,void *win) ;
int kgSendPagedownKeyToWindow(void *Tmp,void *win) ;
int kgSendEscapeKeyToWindow(void *Tmp,void *win) ;
int kgSendClearKeyToWindow(void *Tmp,void *win) ;
int kgSendHomeKeyToWindow(void *Tmp,void *win) ;
int kgSendEndKeyToWindow(void *Tmp,void *win) ;
int kgSendBackspaceKeyToWindow(void *Tmp,void *win) ;
int kgSendLeftKeyToWindow(void *Tmp,void *win) ;
int kgSendRightKeyToWindow(void *Tmp,void *win) ;
int kgSendUpKeyToWindow(void *Tmp,void *win) ;
int kgSendDownKeyToWindow(void *Tmp,void *win) ;
int kgSendEnterKeyToWindow(void *Tmp,void *win) ;
int kgSendLinefeedKeyToWindow(void *Tmp,void *win) ;
int kgSendControlKeyToWindow(void *Tmp,void *win,int ch);
/*
  Image related Calls
*/
void kgInitGm (void);
void *kgGetImageCopy(void *D,void *img);
void *kgGetProcessedImage(void *timg,int Bsize,float rfac,
                         int Btred,int Btgreen,int Btblue);
void kgImage(void *Dtmp,void *tmp,int x0,int y0,int width,int height,float transparency,float highfac);
void *kgGetImage(char *flname);
void *kgGetInlineImage(char *encode,int length);
void kgWriteImage(void *img,char *flname);
void *kgResizeImage(void *img,float fac);
void *kgChangeSizeImage(void *img,long width,long height);
void *kgFilterImage(void *img,long width,long height,int Fltr);
void *kgHalfSizeImage(void * img);
void kgFreeImage(void *img);
void *kgCropImage(void *img,int xl,int yl,int xu,int yu);
void *kgRotateImage(void *img,float angle) ;
void *kgRotateAboutImage(void *img,float angle,int xo,int yo); // Creates new rotated image
void *kgThumbNailImage(void *img,unsigned long w,unsigned long h);
void *kgShadeImage(void *img);
void *kgReduceNoiseImage(void *img);
void *kgBlurgmImage(void *img);
void *kgEmbossImage(void *img);
void *kgSharpenImage(void *img);
void *kgSpreadImage(void *img);
void *kgEnhanceImage(void *img);
void *kgShadowImage(void* img,int xoffset,int yoffset,void *shimg);
void *kgMergeImages(void  *img1,void  *img2,int Xshft,int Yshft); /* second on first */
void *kgMergeTransparentImage(void  *img1,void  *img2,int Xshft,int Yshft); /* second on first */
void *kgMaskImage(void *png,void *mask);
void *kgCopyImage(void *img);
void *kgCreateImage(int xzise,int ysize);
void *kgCleanImage(void *img);
void *kgFlipImage(void *img); // About X refledction overwrites img
void *kgFlopImage(void *img); // About Y refledction overwrites img
int   kgImagetoC(char *flname);
void *kgBorderedRectangle(int w, int h,int clr,float rfac);
void *kgPressedRectangle(int width,int height,int fillclr,float rfac);
void kgGphtoImagefile(char *Imagefile,char *gphfile,int xsize,int ysize,unsigned long bkcolor) ;
void *kgGphtoImage(char *gphfile,int xsize,int ysize,unsigned long bkcolor) ;
void *kgGphtoAntialiasedImage(char *gphfile,int xsize,int ysize,unsigned long bkcolor ,int Mag);
          // Same as kgGphtoImage; but can specify the Mag factor
void * kgStringToImage(char *Str,void *image,int xsize,int ysize,int font,int txtcolor,int justification,int charwidth,int bkcolor);
void  kgStringToImagefile(char *Imgfile,char *Str,int xsize,int ysize,int font,int txtcolor,int justification,int charwidth);
void * kgFilledStringToImage(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int justification,int width,float rfac,int state);
void * kgFilledStringToImage1(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int bodrclr,int justification,int width,float rfac,int state,float depthfac);
void * kgFilledStringToImage2(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int bodrclr,int justification,int width,float rfac,int state);
void * kgFilledStringToImage3(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int bodr,int justification,int width,float rfac,int state,float depthfac);
void * kgBoxedStringToImage(char *Str,void *image,int xsize,int ysize,int font,int border,int highli,int color,int justification,int width,float rfac,int state);
void * kgShadedStringToImage(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int justification,int width,float rfac,int state,int type);
void *kgFMGtogmImage(void *tmp);
void *kgGetBackground(void *Tmp,int x1,int y1,int x2,int y2);
int kgRestoreImage(DIALOG *D,void *tmp,int x0,int y0,int width,int height);
int kgRestoreImagePart(DIALOG *D,void *tmp,int x0,int y0,int xoff,int yoff,int width,int height);
void *kgAddTransparency(void *Img,float transp);
void *kgChangeBrightness(void *Img,float fac) ;
int kgRootImage(void *tmp);
void * kgGetRootImage(void );
void * kgGetRootRawImage(int xo,int yo,int wd,int ht );
void * kgMakeImageFromRaw(unsigned char *Imgdata,int wd,int ht );
/* built in images */
void * kgUndoImage(int size,int red,int green,int blue);
void * kgRedoImage(int size,int red,int green,int blue);
void * kgRightdirImage(int size,int red,int green,int blue);
void * kgLeftdirImage(int size,int red,int green,int blue);
void * kgUpdirImage(int size,int red,int green,int blue);
void * kgDowndirImage(int size,int red,int green,int blue);
void * kgRightImage(int size,int red,int green,int blue);
void * kgLeftImage(int size,int red,int green,int blue);
void * kgUpImage(int size,int red,int green,int blue);
void * kgDownImage(int size,int red,int green,int blue);
void * kgGoback1Image(int size,int red,int green,int blue);
void * kgGobackImage(int size,int red,int green,int blue);
void * kgPowerdownImage(int size,int red,int green,int blue);
void * kgHomeImage(int size,int red,int green,int blue);
int    kgFmgToFile(unsigned char *array,int sz,char * flname);
/* end  of built in images */
/*
   Graphics related
*/
/* opengl related */
void *kgGetglDisplay(void *Gtmp);
void *kgGetglWindow(void* Gtmp);
int  kgInitglWindow(void* Gtmp);
void kgCloseglWindow(void *Gtmp);
int  kgEnableglWindow(void *Gtmp);
int  kgSwapglBuffers(void *Gtmp);
/* end of Opengl related */
/* inbuilt graphics */
void kgViewport(DIG *G,float x1, float y1, float x2, float y2);
void kgUserFrame(DIG *G,float x1,float y1, float x2, float y2);
void kgGetViewport(DIG *G,float *x1,float *y1,float *x2,float *y2);
void kgGetWindow(DIG *G,float *x1,float *y1,float *x2,float *y2);
void kgSetAntialiasing(DIG *G,int val);
void kgAntialiasingOn(DIG *G,int val);
void kgAntialiasingOff(DIG *G);
void kgReview(DIG *G);
void kgAntialiasedReview(DIG *G);
void kgLineColor(DIG *G,int colr);
void kgClearView(DIG *G);
void kgClearBuffer(DIG *G);
void kgMove2f(DIG *G,float x,float y);
void kgMarkerType(DIG *G,int mtype);
void kgDraw2f(DIG *G,float x,float y);
void kgMarker2f(DIG *G,float x,float y);
void kgDefaultGuiTheme(Gclr *Gc);
void kgGrayGuiTheme(Gclr *Gc);
void kgSetGouraudParams(DIG *G,float p2, float p1, int n, int ib);
void kgGouraudFill(DIG *G,int n, float *x, float *y,float *v);
void kgWriteText( DIG *G,char *c);
void kgTextAngle(DIG *G,float ang);
float  kgStringLength(void *G,char *title);
void kgTextSize(DIG *G,float h,float w,float g);
void kgTextFont(DIG *G,int tf);
void kgTextColor(DIG *G,int tcolr);
void kgLineStyle(DIG *G,int style);
void kgLineWidth( DIG *G,int dw);
void kgPolyFill(DIG *G,int n, float *x, float *y, int flag,int color);
void kgBoxFill(DIG *G,float x1,float y1, float x2, float y2,int fl,int ib);
void kgArrowSize(DIG *G,int size);
void kgChangeColor(DIG *G,int no,int ir,int ig,int ib);
void kgSetClip(DIG *G,float x1,float y1,float x2,float y2);
void kgPopClip(DIG *G);
void kgDrawImage(DIG *G,void *imgfile,float x1,float y1,float x2, float y2);
void kgImportGphFile(DIG *G,char *flname,float gx1,float gy1,float gx2,float gy2);
void kgSetShadeParams(DIG *G,float p2, float p1,int startclr, int n, int ib);
int  kgSetShading(DIG *G,int startclr,int ncolors,int bodr,int basecolor);
int  kgSetHSVShading(DIG *G,int startclr,int ncolors,int bodr,float hue,float satu);
int  kgMixHsvColors(void * fid,int sclr,int nclrs,float h,float s,float vmin,float vmax) ;
void kgCircle2f(DIG *G,float x,float y,float r);
void kgPointerZoom(DIG *G);
void kgPointerAntialiasedZoom(DIG *G);
void kgBackupGph(DIG *G,char *flname);
void kgHardCopy(DIG *G,char *flname);
void kgA4Copy(DIG *G,char *flname);
void kgLandscapeCopy(DIG *G,char *flname);
void kgOpenObject(DIG *G,int obj);
void kgCloseObject(DIG *G);
void kgExtObject(DIG *G,int obj);
void kgCallObject(DIG *G,int obj,float sx,float sy,float fx,float fy);
void kgCopyObject(DIG *G,int obj);

int kgRoundedRectangleRing2(void * fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) ;
int kgRoundedRectangleRing3(void * fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) ;
int kgRoundedRectangleRing1(void * fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) ;
int kgRoundedRectangleRing(void * fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) ;
int kgRoundedRectangleFill(void * fid,float xo,float yo,float xl,float yl,int ib,int clr,float fac);
int kgRoundedRectangleShade(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax);
int kgRoundedRectangleShade0(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax);
int kgRoundedRectangleShade1(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax);
int kgRoundedRectangleShade4(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax);
int kgRoundedRectangle(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float bsize);
int kgArcFill(void * fid,float xo,float yo,float r,float ang1,float ang2,int flag,int color);
int kgRoundedRectanglePressed(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float bodrsize);
int kgRoundedRectangleRaised(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float bodrsize);

void kgSwapBuffers(DIG *G) ;
void kgDoubleBuffer(DIG *G); 
void kgSingleBuffer(DIG *G) ;

void  kgDrawingTool(DIG *G);
void kgDisplayGphFile(DIALOG *D,char *flname,int xo,int yo,int xl,int yl,unsigned long bkcolor);
/* 3D graphics related */
void kgUserFrame3D(DIG *G,float x_min,float y_min,float z_min,float x_max,float y_max,float z_max);
void kgRotate(DIG *G,unsigned char ch, float ang);
void kgTranslate(DIG *G,float tx, float ty, float tz);
void kgMove3f(DIG *G,float x, float y, float z);
void kgDraw3f(DIG *G,float x, float y, float z);
void kgPolyFill3f(DIG *G,int n, float *x, float *y, float *z, int flag, int color);
void kgBoxFill3f(DIG *G,float x1,float y1, float z1, float x2, float y2,float z2, int fl,int ib);
void kgGouraudFill3f(DIG *G,int n,float *x,float *y,float *z,float *v);
void kgShade3f(DIG *G,int n,float *x,float *y,float *z);
void kgColorShade3f(DIG *G,int n,float *x,float *y,float *z,int baseclr);
void kgSetZbuffer(DIG *G,int val);
void kgSetNearClip(DIG *G,int val);
void kgSetFarClip(DIG *G,int val);
int  kgAddLight(DIG *G,float x,float y,float z);
int  kgClearLights(DIG *G);
/* End of 3D Calls */
/* Graphics Cursors */
char kgCursor(DIG *G,float *xx,float *yy);
char kgRbrCursor(DIG *G,float *xx,float *yy,float *xbgn,float *ybgn);
char kgRectCursor(DIG *,float *xx,float *yy,float *xbgn,float *ybgn);
char kgBoxCursor(DIG *G,float *xx,float *yy,float *xbgn,float *ybgn);
char kgDblCursor(DIG *G,float *xx,float *yy,float *xbgn,float *ybgn);
char kgArcCursor(DIG *G,float *xx,float *yy,float *xbgn,float *ybgn);
char kgDstCursor(DIG *G,float *xx,float *yy,float *xbgn,float *ybgn);
char kgCrossCursor(DIG *G,float *xx,float *yy);
int  kgChangeCurFormat(DIG *G,char *xfmt,char *yfmt);
/* 
   Image related Calls to use graphics calls
*/
void *kgInitImage(int width,int height,int mag);
void *kgGetResizedImage(void *G);
void *kgGetSharpImage(void *G);
void *kgGetSmoothImage(void *G);
void kgCloseImage(void *Gtmp);
void *kgInitGph(int width,int height);
void kgCloseGph(void *Gtmp);
/* end of graphics calls */
/* wait related */
void kgWaitButtonRelease(DIALOG *D);
int  kgWait(int secs); // for threads to wait
void kgThreadSleep(int secs,int usecs);
int  kgThreadWaitPipe(int pipe,int secs,int usecs);


int  kgCheckMenu(void *parent,int x,int y,char *qst,int df);
int  kgWarnMenu(void *parent,int x,int y,char *qst,int df);
int  kgQstMenu(void *parent,int x,int y,char *qry,int df);
int kgFolderBrowser( void *parent,int x0,int y0,char *flname,char *fltr);
int kgFileBrowser(void *parent,int x0,int y0,  char *v0, char *v1 );
int kgGetFont(void *parent,int xo,int yo);
int kgGetColor(void *parent,int xo,int yo,int *r,int *g,int *b);
/* 
  utilities 
*/
void *kgFreeDouble(void **mem);
void kgCleanDir(char *folder);
void kgCheckAndRemoveParent(char *dir);
char *kgWhich(char *pgr);
int kgSearchString(char *s1,char *s2);
char *kgGetIcon(char *pgr,char *theme);
void *kgSearchIcon(char *IconName);
char *kgMakeTmpDir(void);
char **kgFontNames(void);
char **kgFileMenu( char *dir,char *filter);
char ** kgFolderMenu(char *d_name);
void kgFreeFontNames(char **pt);
int RGBtoHSV(float r,float g,float b,float *h,float *s,float *v);
int HSVtoRGB(float *r,float *g,float *b,float h,float s,float v);
int HLStoRGB(float *r,float *g,float *b,float h, float l,float s);
int RGBtoHLS(float r,float g, float b,float *h,float *l,float *s);

void kgGetWindowSize(DIALOG *D,int *length,int *height);
int kgGetRootPos( int *xp,int *yp);
int kgGetRootRect( int *x1,int *y1,int *x2,int *y2) ;
void * kgGetInputFocus (void *Tmp);
int kgSetInputFocus(void *Tmp,void *wtmp);
int kgCheckMyWindow(void *Tmp,void *wtmp);
void kgDropFocus(void *tmp);
/* Reading Widget data from file */
DIM * Read_data_message(FILE *fp);
DIO * Read_data_progressbar(FILE *fp);
DIV * Read_data_vertscroll(FILE *fp);
DIZ * Read_data_horizscroll(FILE *fp);
DIF * Read_data_floatslide(FILE *fp);
DID * Read_data_intslide(FILE *fp);
DIHB * Read_data_horislide(FILE *fp);
DIW * Read_data_browser(FILE *fp);
DIE * Read_data_scrollmenu(FILE *fp);
DIX *Read_data_selectmenu(FILE *fp);
DIY *Read_data_thumbnailbrowser(FILE *fp);
DIRA *Read_data_radiobutton(FILE *fp);
DICH *Read_data_checkbox(FILE *fp);
DIS * Read_data_msgscroll(FILE *fp);
DIP *Read_data_pixmapbox(FILE *fp);
DII *Read_data_infobox(FILE *fp);
DIG *Read_data_graphbox(FILE *fp);
DIT * Read_data_textbox(FILE *fp);
DIT * Read_data_tablebox(FILE *fp);
DIB * Read_data_buttonbox(FILE *fp);
DIN * Read_data_buttonboxn(FILE *fp);
DIL * Read_data_splbuttonbox(FILE *fp);
DIBN * Read_data_buttonboxnew(FILE *fp);
DIL * Read_data_horibar(FILE *fp);
DILN * Read_data_horibarnew(FILE *fp);
/* End of Widget data */
/* Create Ui and Widgets */
void *kgCreateUi(int length,int width,int (*Initfun)(void *),int (*CallBack)(void *,void *),int (*Cleanupfun) (void *),char *name) ;
void * kgGetGeometry(void *Tmp,int *xo,int *yp,int *l,int *h,int *borwidth);
int kgEventLoopUi(void *Dtmp);
int kgAddWidget(void *tmp,void * Widget);
int kgDeleteWidget(void *tmp,void * Widget);
DILN * kgCreateHButtons(int xo,int yo,int nx,int length,int height,char **titles,char *name);
DILN * kgCreateHButtonsn(int xo,int yo,int nx,int length,int height,char **titles,char *name);
DIB * kgCreateButtonsb(int xo,int yo,int nx,int ny,int length,int height,char **titles,char *name);
DIN * kgCreateButtons(int xo,int yo,int nx,int ny,int length,int height,char **titles,char *name);
DIL * kgCreateSplButtons(int xo,int yo,int nx,int ny,int length,int height,char **titles,char *name);
DII * kgCreateInfoBox(int xo,int yo,int nchars,int nlines,char *name);
DIP * kgCreateImageBox(int xo,int yo,int length,int width,char *image,int bordrtype,char * name);
DIM * kgCreateDisplayBox(int xo,int yo,int length,int width,char *name);
DIG * kgCreateDrawingBox(int xo,int yo,int xsize,int ysize,char *name);
DIO * kgCreateProgressBar(int xo,int yo,int length,int width,char *name);
DIV * kgCreateVertScroll(int xo,int yo,int height,int width,char *name);
DIZ * kgCreateHorizScroll(int xo,int yo,int length,int width,char *name);
DIM * kgCreateMessageBox(int xo,int yo,int length,int width,char *msg,int type,char *name);
DIT * kgCreateTextBox(int xo,int yo,char *formats,char *name);
DIT * kgCreateTable(int xo,int yo,int nrows,char **colformats,char *name);
DIW * kgCreatePulldownBrowser(int xo,int yo,char *prompt,char **menu,char *name);
DIE * kgCreateBrowser(int xo,int yo,char **menu,char *name);
DIX * kgCreateSelectMenu(int xo,int yo,int length,int width,int itenlength,int type,char *name);
DIY * kgCreateThumbnailBrowser(int xo,int yo,int length,int width,int nailsize,int type,char *name);
DIRA * kgCreateRadioButtons(int xo,int yo,int length,int width,int itemlength,char **list,char *name);
DICH * kgCreateCheckBox(int xo,int yo,int length,int width,int itemlength,char **list,char *name);
DIS * kgCreateMessageScroll(int xo,int yo,int nlines,int maxchar,char *name);
DIHB * kgCreateHorizSlide(int xo,int yo,int length,int min,int max,int val,char *name);
DID * kgCreateIntSlide(int xo,int yo,int length,int min,int max,int val,char *name);
DIF * kgCreateDoubleSlide(int xo,int yo,int length,double min,double max,double val,char *name);
/* end of create widgets */
void kgPrintWidgetData(void *W,FILE *fp1); // prints widget data
/***********************************/

/* String Manupulation       */

void kgTruncateString(char *m,int size);
void kgRestoreString(char *m,int size);
void gphUserFrame(int fid,float x1,float y1, float x2, float y2);
/*
   Version 2.1
   Dated 12/07/97
*/

#ifndef D_LINK
#define D_LINK
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
typedef  struct d_l {
         void  *bf;
         struct d_l *nx;
         struct d_l *pv;
} D_l;
typedef  struct l_L {
         D_l *st; 
         D_l *cr;
         D_l *en;
} Dlink;



Dlink * Dopen(void);
int Ddup( Dlink *S,Dlink *D);
int  Dappend(Dlink *F,void  *buf);
void Dempty(Dlink *F);
void Dfree(Dlink *F);
void Drewind(Dlink *F) ;
void Dend (Dlink *F) ;
void Dmove_back(Dlink *LN,int n) ;
void Dmove_forward(Dlink *LN,int n) ;
void Ddelete_items(Dlink *LN,int n) ;
void Ddelete(Dlink *LN) ;
void Dinsert(Dlink *F,void  *buf) ;
void Dadd(Dlink *F,void  *buf) ;
Dlink *Dcopy(Dlink *LN) ;
Dlink *Dsublist(Dlink *LN,void *s,int Dcondition(void *,void *)) ;
Dlink *Dnewlist(Dlink *LN,void * Dnewrule(void *)) ;
void Dsort(Dlink *LN , int Dcmpitems(void *,void *)) ;
int Dcount(Dlink *F) ;
void Dposition(Dlink *F,int n) ;
int Dsetcurpos(Dlink *LN,void *s,int Dcondition(void *,void *)) ;
Dlink *Djoin(Dlink *d,Dlink *s);
int Disum(Dlink *LN , int Dsumitem(void *)) ;
float Dfsum(Dlink *LN , float Dsumitem(void *)) ;
double Ddsum(Dlink *LN , double Dsumitem(void *)) ;
int comp_list(Dlink *L1,Dlink *L2,int comprecord(void *tmp1,void *tmp2)) ;
void Dcondition(Dlink *F,void *arg, int condition(void *,void *));
int Dread(Dlink **L,FILE *f,void * read_link(FILE *fp));
int Dwrite(Dlink *L,FILE *f,int write_link(FILE *fp,void *buf));
int Dreadauto(Dlink **L,char *flname,int size_of_rec);
int Dwriteauto(Dlink *L,char *flname,int size_of_rec);
int Drmvdup(Dlink *L,int size_of_rec);
int Drmvdup_cond(Dlink *L,int rule(void *,void *));
    /* if rule returns 1 records are same */
void *Resetlink(Dlink *L);
void *Getrecord(Dlink *L);
void *Getrecordrev(Dlink *L);
void* Dpick(Dlink *LN) ; /* like Ddelete but does not free */
#endif /* end of dlink.h */

typedef  struct f_L {
         Dlink *L;
         int C_pos;
         char Name[300];
} File;


File * Fopen(char *flname);
File * Fcreate(char *flname);
void Fclose(File *F);
void Empty(File *F);
void copy_to_file(File *F,char *flname);
void append_to_file(File *F,char *flname);
void insert_file(File *LN,char *flname) ;
void delete_lines(File *LN,int n) ;
void move_forward(File *LN,int n) ;
void move_back(File *LN,int n) ;
void Rewind(File *F) ;
void Eoffile(File *F) ;
int  Ungetc(int ch,File *F);
int  Getc(File *F);
int  Get_line(char *bf,File *F) ;
void rmv_right_blnk(File *F);
void Print_file(File *F);
int  append_line(File *F,char *buf);
void insert_line(File *F,char *buf);
void add_line(File *F,char *buf);
int  Fprintf(void *,...);
void Fview(File *F);
void view_file(char *f);
//  thread tool related
int getCores();
void *OpenThreads(int thds);
void DoInAnyThread(void *,void *(*threadFunc)(void *),void *arg);
void WaitThreads(void *);
void CloseThreads(void *);
// Clipboard routines
unsigned char *kgGetPrimary(void * Tmp);
unsigned char *kgGetClipBoard(void * Tmp);
int kgSetPrimary(void * Tmp,unsigned char *data);
int kgSetClipBoard(void * Tmp,unsigned char *data);
void * kgProcessClips(void *Tmp,void *kbtmp);
int kgEnableSelection(void *Tmp);
int kgDisableSelection(void *Tmp);
int kgClearHighlight(void *Tmp);
int kgCheckSelection(void *Tmp);
int kgSetClipbordCallback(void *Tmp,int *(cpCallback)(int,void *));
int kgSetClipMenu(void *Tmp,char **menu);
#ifndef D_KEYBRD
#define D_KEYBRD
#ifndef D_KBINFO
#define D_KBINFO
typedef struct _kb_info {
 int fillclr,butclr,charclr;
 int xl,yl,xg,yg;
 int btype;
 int font;
 float rfac,trans;
 int kbtype;
 int Bodr;
 float Brfac;
} KBINFO;
int kgInitKbinfo(KBINFO *ki);
#endif
typedef struct _keybrd {
  void *D;
  int GrpId;
  void *Coff,*Con,*Lon,*Loff;
  int sgrp,cgrp;
  int offgrp,ongrp;
  int CapsLock;
  int ShiftPress;
  int CurWid;
  int Vis;
  int Btype;
  int symgrp;
  int kbtype;
  char Sfac[5];
  int  Bfont,Bclr,ButClr;
  int  FillClr;
  float Rfac;
  int grp1,grp2,grp3,grp4,grp5,grp6; //extras
  int Hclr;
  void *Thds;
  int Bx,By,Xg,Yg, Boff;
  float trans;
  int Bodr;
  void *TargetWindow;
  float Brfac;
  int CntlGrp;
  void *XpmList; // for Xpms
  int ControlPress,AltPress; // for future
} KEYBRD;
int kgKeybrd(void *Tmp,int Vis,void *kbinfo);
int kgMakeKeybrd5(DIALOG *D,int Bx,int By,int Xg,int Yg,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency,int bodr,float Brfac);
int kgMakeKeybrd4(DIALOG *D,int Bx,int By,int Xg,int Yg,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency,int bodr,float Brfac);
int kgMakeKeybrd0(DIALOG *D,int Bx,int By,int Xg,int Yg,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency,int bodr,float Brfac);
int kgMakeKeybrd3(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency) ;
int kgMakeKeybrd2(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency) ;
int kgMakeSkeybrd(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency);
int kgMakeKeybrd1(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency);
int kgMakeDefaultKeybrd3(DIALOG *D,int xo,int yo,int vis);
int kgMakeDefaultKeybrd2(DIALOG *D,int xo,int yo,int vis);
int kgMakeDefaultSkeybrd(DIALOG *D,int xo,int yo,int vis);
int kgMakeDefaultKeybrd1(DIALOG *D,int xo,int yo,int vis);
int kgSetKeybrdWidget(void *Tmp,int curwid);
int kgHideKeybrd(void *Tmp);
int kgShowKeybrd(void *Tmp);
int kgGetKeybrdSize(void *Tmp,int *xl,int *yl);
int kgShiftKeybrd(void *Tmp,int xs,int ys);
int kgMakeKeybrd(void *Tmp,int Type,int Vis,int Btype,int Bfont,int Charclr,int Butclr,int Fillclr,float Rfac,float Trans) ;
int kgMakeDefaultKeybrd(void *Tmp,int Type,int Vis);
static void *Malloc(int size) {
   void *pt;
   pt = (void *) malloc(size);
   if(pt==NULL) {
     fprintf(stderr,"Failed in Malloc\n");
     exit(0);
   }
   return pt;
}
#endif
#ifdef __cplusplus
}
#endif
#endif /* enf of gl.h */ 
