# Makefile

BINARY          = mac_test
OBJS     	= crc8.o crc16.o main.o
DIST            = mac_test

CC              = gcc
CFLAGS		= -Wall -O3
LIBS            =
LDFLAGS         =
INCLUDES	=

$(BINARY):      $(OBJS)
		$(CC) $(LDFLAGS) -o $(BINARY) $(OBJS) $(LIBS)

.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
		rm -rf $(DIST)-*.txz $(OBJS) $(BINARY) *~ 

dist:		clean
		cd ..; \
		tar cfvJ $(DIST)/$(DIST)-`date -u "+%Y%m%d%H%M00"`.txz \
			$(DIST)/*
