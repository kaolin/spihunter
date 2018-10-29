all: bin/wrmac bin/rdmac bin/pimac bin/rdreg bin/wrreg

bin/rdreg:
	g++ src/rdreg.c -o bin/rdreg -lwiringPi

bin/wrreg:
	g++ src/wrreg.c -o bin/wrreg -lwiringPi

bin/rdmac:
	g++ src/rdmac.c -o bin/rdmac -lwiringPi

bin/wrmac:
	g++ src/wrmac.c -o bin/wrmac -lwiringPi

bin/pimac:
	g++ src/pimac.c -o bin/pimac

clean:
	rm -f bin/*

.PHONY: clean all
