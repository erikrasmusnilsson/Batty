build:
	gcc batty.c -o batty -lpthread

install:
	cp batty /usr/local/bin
	mkdir /usr/share/batty
	cp batty.conf /usr/share/batty/batty.conf
	cp hdr.txt /usr/share/batty/hdr.txt

uninstall:
	rm /usr/local/bin/batty
	rm -r /usr/share/batty
