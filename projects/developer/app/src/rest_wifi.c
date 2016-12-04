#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "rest_http.h"

#define SEND_READY 1
#define RECV_READY !SEND_READY 
char time_buf[20];


char *t_log()
{
	time_t now = time(NULL);
    struct tm ts;
    strftime(time_buf, 17, "(%Y%m%d%H%M%S)", localtime_r(&now, &ts));
	return (char *) &time_buf;
}

int wifi_seq = 0, wifi_status = SEND_READY;
void *send_main_task();
void *recv_main_task();

int wifi_serial_init(int serial, int baud)
{
	struct termios options;
	printf("Setting old serial port\n");

    tcgetattr(serial, &options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_oflag &= ~OPOST;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 10;
    options.c_iflag &= ~INPCK;  /* Enable parity checking */
    options.c_iflag |= IGNPAR;
    options.c_cflag &= ~PARENB; /* Clear parity enable */
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;
    //options.c_iflag &= ~(IXON | IXOFF | IXANY); /* no flow control */
    options.c_oflag &= ~(IXON | IXOFF | IXANY); /* no flow control */
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;  /* No output processing */
    options.c_iflag &= ~INLCR;  /* Don't convert linefeeds */
    options.c_iflag &= ~ICRNL;  /* Don't convert linefeeds */
    options.c_cflag |= (CLOCAL | CREAD);    /* Enable receiver, set*/
	options.c_cflag |= (IXON | IXOFF);  /* Software flow control */
    options.c_lflag = 0;    /* no local flags */
    options.c_cflag |= HUPCL;   /* Drop DTR on close */

    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);
    tcflush(serial, TCIFLUSH);

	//set attribute
	if(-1 == tcsetattr(serial, TCSANOW, &options))
    {
        printf("Error set attribute failed: %d\n", errno);
        return -1;
    }
    tcflush(serial, TCIFLUSH);
	return 1;
}

int wifi_fd = -1;

void open_serial(char *wifi_port)
{
    printf("Start open\n");
    wifi_fd = open(wifi_port, O_RDWR | O_NOCTTY);
    if(wifi_fd < 0)
    {
        printf("Open wifi port: %s error: %d\n", wifi_port, errno);
        exit(0);
    }
	fcntl(wifi_fd, F_SETFL, 0);
}


int main(int argc, char **argv)
{
	char cmd[32];

	open_serial(argv[1]);
	wifi_serial_init(wifi_fd, B115200);

	pthread_t t_read, t_write;
	pthread_create((pthread_t *) &t_write, NULL, send_main_task, NULL);
	pthread_create((pthread_t *) &t_read, NULL, recv_main_task, NULL);
    pthread_join(t_write, NULL);
    pthread_join(t_read, NULL);

}

void *send_main_task()
{
	printf("Entering: %s\n", __FUNCTION__);
	char c;
	int input;
    char get_token[1024];
    memset((char *) &get_token, 0, 1024);
	sprintf((char *) &get_token,"%s%s%s%s%s%s", HTTP_AUTHOR,
            HTTP_AGENT, HTTP_CONTENT, HTTP_ACCEPT,HTTP_LENGTH, REST_LOGIN);
	while(1)
	{
		c = getchar();
		if(c == '.')
		{
			printf("Terminated !!!\n");
			close(wifi_fd);
			exit(0);
		}
	    else if(c == '1')
        {
            write(wifi_fd, AT_CMD, strlen(AT_CMD));
        }
	    else if(c == '2')
        {
            write(wifi_fd, AT_JAP, strlen(AT_JAP));
        }
	    else if(c == '7')
        {
            write(wifi_fd, AT_HTTP_START, strlen(AT_HTTP_START));
        }
        else if(c == '8')
        {
            char send_cmd[32];
            sprintf((char *) &send_cmd,"AT+CIPSEND=%d\r\n",strlen(get_token));
            write(wifi_fd, &send_cmd, strlen(send_cmd));
        }
        else if(c == '9')
        {
            write(wifi_fd, &get_token, strlen(get_token));
        }
        else if(c == '?' || c == 'h')
        {
            printf("List available commands\n");
            printf("a-AT        b-List APs  c-Join AP   d-Version\n");
            printf("e-ListIP    f-Reset     g-HTTP  h-CIPSEND   i-USER\n");
        }

	}
} 

void *recv_main_task()
{
	int read_byte;
	char read_buf[1024];
	printf("Entering: %s\n", __FUNCTION__);
    fd_set ReadSet;
    struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;

	while(1)
	{
        sleep(1);
        if(wifi_fd > 0)
            FD_SET(wifi_fd, &ReadSet);

        //listen of file descriptor
        if(0 == select(wifi_fd+1, &ReadSet, NULL, NULL, &timeout))
        {
            continue;
        }

        //Check to see file descriptor ready
        if(wifi_fd > 0 && FD_ISSET(wifi_fd, &ReadSet))// && wifi_status == RECV_READY)
        {
            memset((char *) &read_buf, 0, 1024);
            read_byte = read(wifi_fd, &read_buf, 1024);
            if(read_byte > 1)
            {
                //printf("ECHO %d bytes content: %s ",read_byte,(char *) &read_buf);
                printf("%s",(char *) &read_buf);
                if(strstr((char *) &read_buf, "OK"))
                {
                    sleep(1);
                    wifi_status = SEND_READY;
                }
            }
		}
	}
}	
