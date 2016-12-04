include ../Make.rules

QT_TOP = ../usr/local
DEFINES = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB

INCPATH = -I./ -I$(QT_TOP)/mkspecs/qws/linux-arm-uclinuxeabi-g++ \
			-I$(QT_TOP)/include/ -I$(QT_TOP)/include/QtCore \
			-I$(QT_TOP)/include/QtGui -I../include

AM_CXXFLAGS = $(INCPATH) $(DEFINES) -static

LINK = arm-uclinuxeabi-g++
RANLIB = arm-uclinuxeabi-ranlib

L_LIBS = -L$(QT_TOP)/lib -lQtCore -lQtGui -lpthread
lib_LTLIBRARIES = libQtApp.a

libQtApp_a_CFLAGS = -static
libQtApp_a_SOURCES = ranger.cpp gui.c

libQtApp_a_LIBADD = -lm
