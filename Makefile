CC = g++
CFLAG = -m32 -O3 -Wall -ansi -fPIC -fno-strict-aliasing -Wno-write-strings -fshort-wchar
DFLAG = -DLINUX
INCDIR = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux \
	-I/usr/include
LIBDIR = -L$(JAVA_HOME)/jre/lib/i386 -L$(JAVA_HOME)/jre/lib/i386/client
WrapperDIR = Source/Wrapper/
JNIDIR = Source/JNI/
samppluginDIR = Dependencies/sampplugin/
SHELL = /bin/bash

OBJS = $(JNIDIR)encoding.o $(JNIDIR)jni_functions.o $(JNIDIR)jni_core.o \
	$(JNIDIR)samp_core.o $(JNIDIR)linux.o $(WrapperDIR)callback.o \
	$(WrapperDIR)core.o $(WrapperDIR)function.o \
	$(samppluginDIR)amxplugin.o $(samppluginDIR)amx/getch.o

.SUFFIXES: .o

all: Shoebill
ifndef JAVA_HOME
	echo "Please define the evironment variable."
endif
	@if [ -f Binary/$< ]; \
		then \
			echo "Make successful."; \
	else \
		echo "Make failed."; \
	fi

Shoebill: $(OBJS)
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
