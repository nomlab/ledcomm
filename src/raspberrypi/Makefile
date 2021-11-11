# Makefile

TARGETS     = send_wifi_info send_string receive_string_from_android

CC          = gcc
CFLAGS      = -Wall $(LIBS)
LIBS        = -l wiringPi
# For old compilers
# CFLAGS      = -Wall -march=native -O3 -std=c99

# If not available perf command
# PERF        =

.PHONY: all build

all: $(TARGETS)

build: $(TARGETS)
