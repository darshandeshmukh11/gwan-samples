CFLAGS = -Wall -DXP_UNIX=1 -DMOZ_X11=1 -fPIC -g

gwan-browser-plugin: gwan-browser-plugin.o
	cc $(CFLAGS) -shared gwan-browser-plugin.o -o gwan-browser-plugin.so

gwan-browser-plugin.o: gwan-browser-plugin.c gwan-browser-plugin.h
	cc $(CFLAGS) -c gwan-browser-plugin.c

clean:
	rm gwan-browser-plugin.so gwan-browser-plugin.o

install:
	cp gwan-browser-plugin.so /usr/lib/mozilla/plugins/
	
uninstall:
	rm /usr/lib/mozilla/plugins/gwan-browser-plugin.so
