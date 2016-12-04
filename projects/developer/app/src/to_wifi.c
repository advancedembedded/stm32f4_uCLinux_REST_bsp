#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define SEND_READY 1
#define RECV_READY !SEND_READY 

int wifi_seq = 0, wifi_status = SEND_READY;
void *send_main_task();
void *recv_main_task();
#define MAX_CMD 5 
char *ATCMD[] = {	"AT\r\n",
					"AT+GMR\r\n", 	
					"ATE1\r\n",	
					"ATE0\r\n",	
					"AT+CWMODE=0\r\n",
					"AT+CWLAP\r\n",
					"AT+CWJAP=\"%s\",\"%s\"\r\n",
					"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",
					"AT+CWQAP\r\n",
					"AT+RST\r\n",
					"AT+RESTORE\r\n",
					"AT+CIFSR\r\n",
					"AT+CWMAC\r\n",
					"AT+CIPSTATUS\r\n",
					"AT+CIPSTART %s\r\n",
					"AT+CIPSEND %s\r\n",
					"+IDP\r\n"
									};

typedef enum {
	AT_CMD = 0,
	AT_VER,
	AT_ECHO_ON,
	AT_ECHO_OFF,
	AT_MODE,
	AT_LAP,
	AT_JAP,
	AT_JAP_CUR,
	AT_QAP,
	AT_RST,
	AT_RESTORE,
	AT_LIP,
	AT_MAX_PW,
	AT_GET_MAC,
	AT_NET_STATUS,
	AT_TCP_CON,
	AT_DATA_SEND,
	AT_DATA_RECV
} WIFI_AT_CMD;
	
	

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
	setting.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
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

int wifi_fd = -1;

void open_serial()
{
    printf("Start open\n");
    wifi_fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(wifi_fd < 0)
    {
        printf("Open wifi serial port error: %d\n", errno);
        exit(0);
    }
}


int main(int argc, char **argv)
{
	char cmd[32];

	open_serial();
	wifi_serial_init(wifi_fd, B115200);

	pthread_t t_read, t_write;
	pthread_create((pthread_t *) &t_write, NULL, send_main_task, NULL);
	pthread_create((pthread_t *) &t_read, NULL, recv_main_task, NULL);
    pthread_join(t_write, NULL);
    pthread_join(t_read, NULL);

}

void *send_main_task()
{
	int i;
	char cmd[32];
	static int write_seq = 0;
	printf("Entering: %s\n", __FUNCTION__);

	while(1)	
	{

		if(wifi_seq >= MAX_CMD)
		{
			printf("End of sending sequence\n");
			sleep(5);
			continue;
		}
		if(wifi_status == SEND_READY)
		{
	        printf("send %s to wifi serial", AT_cmd[wifi_seq]);
	        write(wifi_fd, AT_cmd[wifi_seq], strlen(AT_cmd[wifi_seq]));
		    sleep(1);
			wifi_status = RECV_READY;
		}
		sleep(1);
	}
} 

void *recv_main_task()
{
	int read_byte;
	char read_buf[1024];
	printf("Entering: %s\n", __FUNCTION__);
	while(1)
	{
		if(wifi_status == RECV_READY)
		{
			read_byte = read(wifi_fd, &read_buf, 1024);
			if(read_byte > 0)
			{
				printf("WIFI response: %s\n", &read_buf);
				if(strstr(&read_buf, "OK"))
				{
					wifi_seq++;
					wifi_status = SEND_READY;
				}
			}
		}
		sleep(1);
	}
}	
