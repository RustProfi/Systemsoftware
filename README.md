1. Code runterladen:
Da labshell nicht funktioniert haben wir das nicht über nix runtergeladen sondern via
wget "https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.11.tar.xz"
und dann entpackt mit tar xf linux-4.11.tar.xz

weil wir einen coolen Error bekommen weil die Bib curses.h fehlt beim Aufruf von make
menuconfig müssen wir schauen dass wir ncurses selber installieren

Ok weil wir keine Lust haben auf make config, wo man Zeile für Zeile abgefragt wird und ncurses nicht installierbar ist, weil wir keine root Rechte haben,  machen wir die Config jetzt bei uns und hoffen dass es dann funktioniert. 
