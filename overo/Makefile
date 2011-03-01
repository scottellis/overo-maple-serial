ifeq ($(strip $(OETMP)),)
	OETMP = ${OVEROTOP}/tmp
endif

TOOLDIR = $(OETMP)/sysroots/`uname -m`-linux/usr/armv7a/bin
STAGEDIR = ${OETMP}/sysroots/armv7a-angstrom-linux-gnueabi/usr

CC = ${TOOLDIR}/arm-angstrom-linux-gnueabi-gcc
CFLAGS = -Wall

LIBDIR = $(STAGEDIR)/lib
INCDIR = $(STAGEDIR)/include

SERIAL_C_OBJS = serial_c.o

serial_c : $(SERIAL_C_OBJS)
	$(CC) $(CFLAGS) -L $(LIBDIR) $(SERIAL_C_OBJS) -o serial_c

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@


install:
	sudo cp serial_c /exports/overo/home/root


install_ssh:
	scp serial_c root@earth:/home/root


clean :
	rm -f *.o serial_c

