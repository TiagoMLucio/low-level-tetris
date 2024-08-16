#
# Rules.mk
#
# USPi - An USB driver for Raspberry Pi written in C
# Copyright (C) 2014-2020  R. Stange <rsta2@o2online.de>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

ifeq ($(strip $(USPIHOME)), "./uspi")
USPIHOME = .
endif
ifeq ($(strip $(USPIHOME)), )
USPIHOME = ..
endif


-include $(USPIHOME)/Config.mk

RASPPI	?= 2
AARCH64 ?= 0
PREFIX	?= arm-none-eabi-
DEBUG	?= 0
LOGGING	?= 0

CC	= $(PREFIX)gcc
AS	= $(CC)
LD	= $(PREFIX)ld
AR	= $(PREFIX)ar

ifeq ($(strip $(RASPPI)),2)
ARCH	?= -march=armv7-a -mtune=cortex-a7
TARGET	?= kernel7
else
ARCH	?= -march=armv8-a -mtune=cortex-a53
TARGET	?= kernel8-32
endif

OPTIMIZE ?= -O2

AFLAGS	+= $(ARCH) -DRASPPI=$(RASPPI)
CFLAGS	+= $(ARCH) -Wall -Wno-psabi -fsigned-char -fno-builtin -nostdinc -nostdlib \
	   -std=gnu99 -undef -DRASPPI=$(RASPPI) -I $(USPIHOME)/include $(OPTIMIZE) \
	   -D__IEEE_LITTLE_ENDIAN -D__INT32_TYPE__=int \
	   -D__INTPTR_TYPE__=long\ int -D__UINTPTR_TYPE__=long\ unsigned\ int \
	   -DLOGGING=$(LOGGING)

ifeq ($(strip $(DEBUG)),0)
CFLAGS	+= -DNDEBUG
endif

%.o: %.S
	@echo "  AS    $@"
	@$(AS) $(AFLAGS) -c -o $@ $<

%.o: %.c
	@echo "  CC    $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET).img: $(OBJS) $(LIBS)
	@echo "  LD    $(TARGET).elf"
	@$(LD) -o $(TARGET).elf -Map $(TARGET).map -T $(USPIHOME)/env/uspienv.ld \
		$(USPIHOME)/env/lib/startup.o $(OBJS) $(LIBS)
	@echo "  DUMP  $(TARGET).lst"
	@$(PREFIX)objdump -D $(TARGET).elf > $(TARGET).lst
	@echo "  COPY  $(TARGET).img"
	@$(PREFIX)objcopy $(TARGET).elf -O binary $(TARGET).img
	@echo -n "  WC    $(TARGET).img => "
	@wc -c < $(TARGET).img

clean:
	rm -f *.o *.a *.elf *.lst *.img *.cir *.map *~ $(EXTRACLEAN)
