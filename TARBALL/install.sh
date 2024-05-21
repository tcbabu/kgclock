install -m 755   kgclock  /usr/bin/kgclock
mkdir -p /usr/share/kgclock
install -m 644  kgclock.png /usr/share/kgclock/kgclock.png
install -m 644  beep.wav /usr/share/kgclock
install -m 644  bell.wav /usr/share/kgclock
install -m 644  kgbell1.wav /usr/share/kgclock
install -m 644  kgbell2.wav /usr/share/kgclock
install -m 644  kgclock.desktop /usr/share/applications
install -m 755 -d voices  /usr/share/kgclock
