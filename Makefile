GCCDIR=../arm-linux-androideabi-4.6
NDKDIR=../ndk
MODULE=libsecurity.so
SRC=module_logging.c
INC=$(NDKDIR)/7/platforms/android-14/arch-arm/usr/include/
LIB=$(NDKDIR)/7/platforms/android-14/arch-arm/usr/lib/
BIN=$(NDKDIR)/7/platforms/android-14/arch-arm/usr/lib

$(MODULE): $(SRC)
	$(GCCDIR)/bin/arm-linux-androideabi-gcc -I$(INC) -L$(LIB) -B$(BIN) -fPIC -shared -o $(MODULE) $(SRC) 

clean:
	rm $(MODULE)

