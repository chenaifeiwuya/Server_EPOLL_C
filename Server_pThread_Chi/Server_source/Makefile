.PHONY:Server clean

INCLUDE_PATH=../INCLUDE/
INSTALL_PATH=/user/bin/
LIBRARY=../library/
CC=gcc
CFLAGS=-I$(INCLUDE_PATH) -g -Wall -c
LDFLAGS=-lpthread
TARGET=Server

SFILE=$(wildcard *.c)
DFILE=$(patsubst %.c,%.o,$(SFILE))

$(TARGET):$(DFILE)
	$(CC) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $<



clean:
	rm -rf $(DFILE)
	rm -rf $(TARGET)
