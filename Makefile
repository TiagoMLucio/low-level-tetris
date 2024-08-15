#
# Makefile
#

USPIHOME = ./uspi

OBJS = queue.o graphics.o game_screen.o tile.o main.o  syscalls.o uptime.o

RASPPI ?= 2
AARCH64 ?= 0

NEWLIBHOME = ./newlib
LIBGCCHOME = ./libgcc

LIBUSPI = $(USPIHOME)/lib/rpi$(RASPPI)/libuspi.a
LIB_NEWLIB = $(NEWLIBHOME)/lib/libc.a
LIB_LIBGCC = $(LIBGCCHOME)/lib/libgcc.a
LIBUSPENV = $(USPIHOME)/lib/rpi$(RASPPI)/libuspienv.a

LIBS = $(LIBUSPI) $(LIBUSPENV) $(LIB_NEWLIB) $(LIB_LIBGCC)

CFLAGS	+= -I $(USPIHOME)/env/include -I $(USPIHOME)/include \
	   	   -I $(NEWLIBHOME)/include -I $(LIBGCCHOME)/include \
		   -I $(LIBGCCHOME)/plugin/include

include $(USPIHOME)/Rules.mk

clean:
	rm -f *.o *.elf *.lst *.img *.cir *.map *~
	rm -f $(USPIHOME)/env/lib/*.o 