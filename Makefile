all: bin/wrmac bin/rdmac bin/pimac bin/rdreg bin/wrreg bin/dpmib bin/rdmib bin/rgrst bin/swrgrst bin/swrst

bin/rdreg: src/rdreg.c src/reg.c
	g++ src/rdreg.c src/reg.c -o bin/rdreg -lwiringPi

bin/wrreg: src/wrreg.c src/reg.c
	g++ src/wrreg.c src/reg.c -o bin/wrreg -lwiringPi

bin/rdmac: src/rdmac.c
	g++ src/rdmac.c -o bin/rdmac -lwiringPi

bin/dpmib: src/dpmib.c
	g++ src/dpmib.c -o bin/dpmib -lwiringPi

bin/rdmib: src/rdmib.c
	g++ src/rdmib.c -o bin/rdmib -lwiringPi

bin/rgrst: src/rgrst.c
	g++ src/rgrst.c -o bin/rgrst -lwiringPi

bin/swrgrst: src/swrgrst.c
	g++ src/swrgrst.c -o bin/swrgrst -lwiringPi

bin/swrst: src/swrst.c
	g++ src/swrst.c -o bin/swrst -lwiringPi

bin/wrmac: src/wrmac.c
	g++ src/wrmac.c -o bin/wrmac -lwiringPi

bin/pimac: src/pimac.c
	g++ src/pimac.c -o bin/pimac
