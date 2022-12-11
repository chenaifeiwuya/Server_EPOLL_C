.PHONY:Client clean

INSTALL_PATH=/user/bin/
LIBRARY=../library/
CC=gcc
CFLAGS=-g -Wall -c
TARGET=Client

SFILE=$(wildcard *.c)
DFILE=$(patsubst %.c,%.o,$(SFILE))

$(TARGET):$(DFILE)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) $<



clean:
	rm -rf $(DFILE)
	rm -rf $(TARGET)
