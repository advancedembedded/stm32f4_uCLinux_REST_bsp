# Makefile for the LCD test user space program.
all		: apps
clean		: clean_apps

# Edit the line below to modify a set of user-space programs
# you need to build
APPS		= lcdtest
apps		: $(APPS)

# These are flags/tools used to build user-space programs
CFLAGS		:= -Os -mcpu=cortex-m3 -mthumb
LDFLAGS		:= -mcpu=cortex-m3 -mthumb
CC		= $(CROSS_COMPILE_APPS)gcc

# Clean-up after user-space programs
clean_apps	:
	-rm -f $(APPS) *.gdb *.o

