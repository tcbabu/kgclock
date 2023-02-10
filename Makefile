HOMEDIR=$(PWD)
FLITE=$(PWD)/flite
kgclock	: src/kgclock
	  mv src/kgclock kgclock
src/kgclock	: lib/libflite.a
	  make -C src
lib/libflite.a	:buildflite
	         ./buildflite
buildflite     :  
	echo "export HOMEDIR=$(PWD)" >buildflite
	echo "export FLITE=$(PWD)/flite" >>buildflite
	echo "cd $(FLITE)" >>buildflite
	echo "./configure prefix=$(HOMEDIR)" --with-audio=linux >> buildflite
	echo "make">> buildflite
	echo "make install">> buildflite
	chmod +x buildflite

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
	 make -C src clean
	 make -C flite clean
	 rm -rf bin lib include buildflite
