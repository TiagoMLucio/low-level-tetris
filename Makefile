#
# Makefile
#

USPIHOME = ./uspi

OBJS = main.o

RASPPI ?= 2
AARCH64 ?= 0

ifeq ($(strip $(LOGGING)),1)
LIBUSPENV = $(USPIHOME)/lib/rpi$(RASPPI)/libuspienv.a
else
LIBUSPENV = $(USPIHOME)/lib/rpi$(RASPPI)/libuspienv-nolog.a
endif

LIBS = $(USPIHOME)/lib/rpi$(RASPPI)/libuspi.a \
	   $(LIBUSPENV)

CFLAGS	+= -I $(USPIHOME)/env/include -I $(USPIHOME)/include

include $(USPIHOME)/Rules.mk

clean:
	rm -f *.o *.elf *.lst *.img *.cir *.map *~
	rm -f $(USPIHOME)/env/lib/*.o 