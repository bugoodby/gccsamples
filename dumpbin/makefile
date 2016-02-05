CC=g++
WARNS=-Wall -Werror
COPTS=-O2
CDEFS=
CFLAGS=-c

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


##################################################

TARGET1=dumpbin

INCLUDES=-I. 

SRCS1=$(wildcard ./src/dumpbin/*.c) \
      $(wildcard ./src/dumpbin/*.cpp)

OBJS1=$(addsuffix .o,$(basename $(SRCS1)))

LIBS1=


##################################################

TARGET2=packbin

INCLUDES=-I. 

SRCS2=$(wildcard ./src/packbin/*.c) \
      $(wildcard ./src/packbin/*.cpp)

OBJS2=$(addsuffix .o,$(basename $(SRCS2)))

LIBS2=


##################################################

.PHONY: all
all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS1)

$(TARGET2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS2)

.PHONY: clean
clean:
	-rm -f $(TARGET1) $(TARGET2) $(OBJS) ./*~ ./src/dumpbin/*~ ./src/packbin/*~ $(OBJS:.o=.gcno) $(OBJS:.o=.gcda)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -I$(dir $@) -o $@ $<

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -I$(dir $@) -o $@ $<



