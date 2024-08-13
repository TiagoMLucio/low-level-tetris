#
# Makefile
#

USPIHOME = ./uspi

OBJS = main.o

RASPPI ?= 2
AARCH64 ?= 0

LIBS = $(USPIHOME)/lib/rpi$(RASPPI)/libuspi.a \
	  $(USPIHOME)/lib/rpi$(RASPPI)/libuspienv.a

CFLAGS	+= -I $(USPIHOME)/env/include -I $(USPIHOME)/include

include $(USPIHOME)/Rules.mk

clean:
	rm -f *.o *.elf *.lst *.img *.cir *.map *~
	rm -f $(USPIHOME)/env/lib/*.o 