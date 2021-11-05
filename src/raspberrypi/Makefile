# Makefile

TARGETS     = send_wifi_info receive_string

CC          = gcc
CFLAGS      = -Wall -I/usr/local/include -L/usr/local/lib -lwiringPi 

# For old compilers
# CFLAGS      = -Wall -march=native -O3 -std=c99

# If not available perf command
# PERF        =

SRCS        := $(wildcard *.c)

.PHONY: all clean build

.c.o:
	gcc $(CFLAGS) -c $<

all: $(TARGETS)

setup:
	git clone git://git.drogon.net/wiringPi &&\
	cd ~/wiringPi &&\
  ./build

build: $(TARGETS)
	$(foreach src,$(TARGETS),src.c)
