all: bin/wrmac bin/rdmac bin/pimac

bin/rdmac:
	g++ src/rdmac.c -o bin/rdmac -lwiringPi

bin/wrmac:
	g++ src/wrmac.c -o bin/wrmac -lwiringPi

bin/pimac:
	g++ src/pimac.c -o bin/pimac

clean:
	rm -f bin/*

.PHONY: clean all
