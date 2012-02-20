CC = g++
CFLAG = -O3 -Wall -ansi -fPIC -fno-strict-aliasing -Wno-write-strings
DFLAG = -DLINUX
INCDIR = -I$(JDK_HOME)/include -I$(JDK_HOME)/include/linux \
	-I/usr/include
LIBDIR = -L$(JDK_HOME)/jre/lib/i386 -L$(JDK_HOME)/jre/lib/i386/client
WrapperDIR = Source/Wrapper/
JNIDIR = Source/JNI/
samppluginDIR = Dependencies/sampplugin/

OBJS = $(JNIDIR)encoding.o $(JNIDIR)jni_functions.o $(JNIDIR)jni_core.o \
	$(JNIDIR)samp_core.o $(JNIDIR)linux.o $(WrapperDIR)callback.o \
	$(WrapperDIR)core.o $(WrapperDIR)function.o \
	$(samppluginDIR)amxplugin.o $(samppluginDIR)amx/getch.o

.SUFFIXES: .o

all: libShoebill.so

libShoebill.so: $(OBJS)
	$(CC) -shared $(OBJS) $(LIBDIR) -ljvm -liconv $(CFLAG) -o $@
	-mkdir Binary
	mv $@ Binary/$@

%.o: %.cpp
	$(CC) $< $(CFLAG) $(DFLAG) $(INCDIR) $(LIBDIR) -c
	mv *.o $@

.PHONY: clean
clean:
	-rm -rf Binary
	-rm $(OBJS)
