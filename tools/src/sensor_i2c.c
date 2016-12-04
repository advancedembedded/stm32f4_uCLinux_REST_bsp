#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_BUF_SZ 265

int main(int argc, char **argv)
{
	int i2c_fd;
	char i2c_buf[I2C_BUF_SZ];

	i2c_fd = open("/dev/i2c-2", O_RDWR);
	if(i2c_fd < 0)
	{
		printf("Failed to open i2c-2 device: %d\n", errno);
		return -1;
	}	
	
	//Set control state
	ioctl(i2c_fd, I2C_SLAVE, 0x05);

}
