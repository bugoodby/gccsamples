CC=g++
WARNS=-Wall -Werror
COPTS=-O2
CDEFS=
CFLAGS=-c

TARGET=main

INCLUDES=-I. \
         -I./src \
         -I../so1/src

SRCS=./src/main.cpp \
     ./src/foo.c \
     ./src/bar.c

OBJS=$(addsuffix .o,$(basename $(SRCS)))

LIBS=-L../so1 -ltest

### for gcov ###
ifeq ($(gcov),true)
COPTS=-O0
CDEFS+=-DUSE_GCOV
CFLAGS+=-fprofile-arcs -ftest-coverage
LIBS+=-lgcov
endif

### for gdb ###
ifeq ($(gdb),true)
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



