# Makefile

TARGETS     = send_wifi_info receive_string

CC          = gcc
CFLAGS      = -Wall -lwiringPi

# For old compilers
# CFLAGS      = -Wall -march=native -O3 -std=c99

# If not available perf command
# PERF        =

SRCS        := $(wildcard *.c)
OBJS        := $(SRCS:.c=.o)
COMMON_OBJS := $(filter-out m-%.o,$(OBJS))

.PHONY: all clean test test-perf allow-perf test-e2e

.c.o:
	gcc $(CFLAGS) -c $<

all: $(TARGETS)

$(foreach EXE,$(TARGETS),$(eval $(EXE): $(COMMON_OBJS) $(EXE).o))

# Remove backslash cf. https://koturn.hatenablog.com/entry/2016/06/26/020000
$(foreach SRC,$(SRCS),$(eval $(subst \,,$(shell $(CC) -MM $(SRC)))))

clean:
	rm -f *.o
	rm -f $(TARGETS)
