# Makefile for an A2F-Linux project

all: app/Makefile

# This project's name
SAMPLE		:= qtdemo

# Custom applcations subdirectories that this project
# needs to build. Edit this list as appropriate for your application.
# CUSTOM_APPS	:= app
CUSTOM_APPS	:= app

# Include Make rules applicable to all projects
include ../Rules.make

app/Makefile:
	if [ ! -e app ]; then \
		ln -s ${INSTALL_ROOT}/A2F/qt-4.7.3/demos/ranger ./app; \
	fi

	cd app && ${INSTALL_ROOT}/A2F/qt-4.7.3/bin/qmake
