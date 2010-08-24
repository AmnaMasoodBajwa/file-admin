QTDIR = /usr/local/Trolltech/Qt-4.6.3
MOC = $(QTDIR)/bin/moc

CC = gcc
CXX = g++
CPPFLAGS = -Wall -g -I include -I $(QTDIR)/include/QtGui -I $(QTDIR)/include/QtCore -I $(QTDIR)/include
LD = ld
LDFLAGS =  -L src -L $(QTDIR)/lib -lQtCore -lQtGui

vpath %.h include
vpath %.cpp src

META_HEADERS = window.h FileSystemModel.h
MOC_SOURCES = $(patsubst %.h, moc_%.cpp, $(META_HEADERS))

HEADERS = window.h FileSystemModel.h utility.h
SOURCES = window.cpp main.cpp FileSystemModel.cpp utility.cpp

OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES) $(MOC_SOURCES))

PROGRAM = file-manager

$(PROGRAM):$(OBJECTS) 
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJECTS):%.o:%.cpp
	$(CXX) -c $(CPPFLAGS) -o $@ $<

$(MOC_SOURCES):moc_%.cpp:%.h
	$(MOC) -o $@ $<

%.d:%.cpp
	rm -f $@
	$(CXX) -MM $(CPPFLAGS) $< >$@
	sed -i -e 's/\($*\)\.o[ :]*/\1.o $@ : /g' $@

DEPENDS = $(patsubst %.o, %.d, $(OBJECTS))
-include $(DEPENDS)

.PHONY:clean

clean:
	rm -rf $(PROGRAM) *.o $(MOC_SOURCES) $(DEPENDS)
