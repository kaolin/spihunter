all: ../bin/wrmac ../bin/rdmac ../bin/pimac

../bin/rdmac:
	g++ rdmac.c -o ../bin/rdmac -lwiringPi

../bin/wrmac:
	g++ wrmac.c -o ../bin/wrmac -lwiringPi

../bin/pimac:
	g++ pimac.c -o ../bin/pimac

clean:
	rm -f ../bin/*

install:
	sudo cp -f ../bin/* /usr/bin/
	sudo systemctl enable spihunter
	sudo systemctl start spihunter
