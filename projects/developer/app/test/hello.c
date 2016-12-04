/*
 * This is a user-space application that reads /dev/sample
 * and prints the read characters to stdout
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

void test1_main_thread()
{
	int i = 0;
	while(1)
	{
		printf("%s: In loop: %d\n",__FUNCTION__, i++);
		sleep(10);
	}
}
		
void test1_main_task()
{
	pthread_t test1 = -1;
	pthread_create(&test1, NULL, test1_main_thread, NULL);
	
}

void test2_main_task()
{
	int i = 0;
	while(1)
	{
		printf("%s: In loop: %d\n",__FUNCTION__, i++);
		sleep(10);
	}
}
		

int main(int argc, char **argv)
{
	char * app_name = argv[0];

	pid_t t_id1 = -1, t_id2 = -1;
	
	t_id1 = vfork();
	if(t_id1 == 0)
	{
		printf("In vfork 1\n");
		test1_main_task();
	}
	t_id2 = vfork();
	if(t_id2 == 0)
	{
		printf("In vfork 2\n");
		test2_main_task();
	}
}
