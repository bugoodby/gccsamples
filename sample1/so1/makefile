CC=g++ -m32
WARNS=-Wall -Werror
COPTS=-O2
CDEFS=
CFLAGS=-c -fPIC

LINKERNAME=libtest.so
SONAME    =$(LINKERNAME).1
REALNAME  =$(LINKERNAME).1.0.2

INCLUDES=-I. \
         -I./src

SRCS=./src/test1.cpp \
     ./src/test2.cpp

OBJS=$(addsuffix .o,$(basename $(SRCS)))

LIBS=

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
all: $(SONAME)

$(SONAME): $(REALNAME)
	ln -sf $< $(SONAME)
	ln -sf $< $(LINKERNAME)

$(REALNAME): $(OBJS)
	$(CC) $(LDFLAGS) -shared -Wl,-soname,$(SONAME) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	-rm -f $(LINKERNAME)* $(OBJS) ./*~ ./src/*~ $(OBJS:.o=.gcno) $(OBJS:.o=.gcda)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ $<



