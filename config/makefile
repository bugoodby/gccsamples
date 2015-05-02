CC=g++
WARNS=-Wall -Werror
COPTS=-O2
CDEFS=
CFLAGS=-c

TARGET=config

INCLUDES=-I. \
         -I./src \

SRCS=$(wildcard ./src/*.c) \
     $(wildcard ./src/*.cpp)

OBJS=$(addsuffix .o,$(basename $(SRCS)))

LIBS=

### for gcov ###
ifeq ($(GCOV),true)
$(warning *** GCOV MODE ***)
COPTS=-O0
CDEFS+=-DUSE_GCOV
CFLAGS+=-fprofile-arcs -ftest-coverage
LIBS+=-lgcov
endif

### for gdb ###
ifeq ($(GDB),true)
$(warning *** GDB MODE ***)
COPTS=-O0
CFLAGS+=-g
endif

CFLAGS+=$(WARNS) $(COPTS) $(CDEFS)
CXXFLAGS=$(CFLAGS)


.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	-rm -f $(TARGET) $(OBJS) ./*~ ./src/*~ $(OBJS:.o=.gcno) $(OBJS:.o=.gcda)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ $<



