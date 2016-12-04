
all: hello test_thread test_fork lcdtest.c tfork

hello:
	arm-uclinuxeabi-gcc -o hello hello.c -I \
		${INSTALL_ROOT}/A2F/root/usr/include -mcpu=cortex-m3 -mthumb -L \
		${INSTALL_ROOT}/A2F/root/usr/lib -pthread

test_thread:
	arm-uclinuxeabi-gcc -o test_thread test_thread.c -I \
		${INSTALL_ROOT}/A2F/root/usr/include -mcpu=cortex-m3 -mthumb -L \
		${INSTALL_ROOT}/A2F/root/usr/lib -pthread

test_fork:
	arm-uclinuxeabi-gcc -o test_fork test_fork.c -I \
		${INSTALL_ROOT}/A2F/root/usr/include -mcpu=cortex-m3 -mthumb -L \
		${INSTALL_ROOT}/A2F/root/usr/lib -pthread

lcdtest:
	arm-uclinuxeabi-gcc -mcpu=cortex-m3 -mthumb -o lcdtest lcdtest.c

tfork:
	arm-uclinuxeabi-gcc -o tfork tfork.c -I \
		${INSTALL_ROOT}/A2F/root/usr/include -mcpu=cortex-m3 -mthumb -L \
		${INSTALL_ROOT}/A2F/root/usr/lib -pthread

clean:
	rm -f hello test_thread test_fork lcdtest *.gdb
