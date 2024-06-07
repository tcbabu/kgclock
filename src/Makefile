KULINA=/usr
CC	=cc  -pthread -I$(KULINA)/include -L$(KULINA)/lib
kgclock	: clockpgr.o  SetAlertDia.o notify.o setnotify.o  InstallDia.o ShowDateDia.o clockMenu.o clockpic.o makeimage.o clocktypeCallbacks.o clocktype.o 
	 $(CC) -O3 -o kgclock clockpgr.o SetAlertDia.o notify.o \
	 setnotify.o  InstallDia.o ShowDateDia.o clockMenu.o clockpic.o \
	 clocktypeCallbacks.o clocktype.o \
	 makeimage.o  ../flite/main/flite_main.o \
	../flite/main/flite_voice_list.o \
	../flite/main/flite_lang_list.o -Wl,-rpath=/usr/share/kgclock/lib \
	 -Wl,-Bstatic   -L../lib  \
	 -lflite_cmu_us_slt -lflite_cmulex -lflite_usenglish \
	 -lflite_cmu_indic_lex -lflite_cmu_indic_lang \
	 -lflite_cmu_grapheme_lang -lflite_cmu_grapheme_lex \
	 ../lib/libflite.a  -lkulina -lgm -static-libgcc -Wl,-Bdynamic -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread
install	: kgclock
		install -m 755 -s  kgclock  /usr/bin/kgclock
		mkdir -p /usr/share/kgclock
		install -m 644  kgclock.png /usr/share/kgclock/kgclock.png
		install -m 644  kgbell1.wav  /usr/share/kgclock
		install -m 644  kgbell2.wav  /usr/share/kgclock
		install -m 644  bell.wav  /usr/share/kgclock
		install -m 644  beep.wav  /usr/share/kgclock
		install -m 644  kgclock.desktop /usr/share/applications
clean	:
	 rm -f *.o kgclock
clockpgr.o	: clockpgr.c images.c 
	 $(CC) -O3 -c clockpgr.c
notify.o	: notify.c  
	 $(CC) -O3 -c notify.c
setnotify.o	: setnotify.c  
	 $(CC) -O3 -c setnotify.c
setAlertDia.o	: setAlertDia.c  
	 $(CC) -O3 -c setAlertDia.c
InstallDia.o	: InstallDia.c  
	 $(CC) -O3 -c InstallDia.c
ShowDateDia.o	: ShowDateDia.c  
	 $(CC) -O3 -c ShowDateDia.c
clockMenu.o	: clockMenu.c  
	 $(CC) -O3 -c clockMenu.c
