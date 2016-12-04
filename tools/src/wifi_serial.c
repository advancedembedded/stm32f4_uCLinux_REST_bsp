#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


int wifi_serial_init(int wifi_fd, int baud)
{
	struct termios setting;

	tcflush(wifi_fd, TCIOFLUSH);
	//get attribute
	if(-1 == tcgetattr(wifi_fd, &setting))
	{
		printf("Error get attribute failed: %d\n", errno);
		return -1;
	}
	setting.c_cflag |= (CLOCAL | CREAD);

	//set baudrate
	if(-1 == cfsetospeed(&setting, baud))
	{
		printf("Error set baudrate failed: %d\n", errno);
		return -1;
	}
	//set 8N1
	setting.c_cflag &= ~PARENB;
	setting.c_cflag &= ~CSTOPB;
	setting.c_cflag &= ~CSIZE;
	setting.c_cflag |= CS8;
	//no HW control	
	setting.c_cflag &= ~CRTSCTS;
	//raw input
	setting.c_cflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	//no SW control
	setting.c_cflag &= ~(IXON | IXOFF | IXANY | ICRNL);
	//raw output
	setting.c_cflag &= ~OPOST;

	//set attribute
	if(-1 == tcsetattr(wifi_fd, TCSANOW, &setting))
    {
        printf("Error set attribute failed: %d\n", errno);
        return -1;
    }
	return 1;
}

#define READ_BUF_SZ	256
int main(int argc, char **argv)
{
	int wifi_fd = -1, read_byte = 0;
	char read_buf[READ_BUF_SZ];
	fd_set ReadSet;
	struct timeval timeout;

	wifi_fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(wifi_fd < 0)
	{
		printf("Open wifi serial port error: %d\n", errno);
		return -1;
	}

	// Set up serial port
	wifi_serial_init(wifi_fd, B115200);

	while(1)
	{
		if(wifi_fd > 0)
			FD_SET(wifi_fd, &ReadSet);

		//listen of file descriptor
		if(0 == select(wifi_fd +1, &ReadSet, NULL, NULL, &timeout))
			continue;
	
		//Check to see file descriptor ready
		if(wifi_fd > 0 && FD_ISSET(wifi_fd, &ReadSet))
		{
			read_byte = read(wifi_fd, &read_buf, READ_BUF_SZ);
			if(read_byte > 0)
			{
				printf("Read %d bytes\n", read_byte);
				printf("contend %s\n",(char *) &read_buf);
			}
		}	
		sleep(1);
	}
}
	


