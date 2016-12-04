#include <sys_msg.h>
#include <pthread.h>
#include <stdio.h>
#include <rest_com.h>
#include "gui.h"

typedef struct {
    REST_GENERIC_MSG_HEADER_T header;
} REST_GUI_MSG_T;

void gui_msg_handler(REST_GUI_MSG_T * Incoming)
{
    switch(Incoming->header.subType)
    {
        case 1:
            break;
        default:
            rest_log(ERR, "%s: Unknown message type %d\n", __FUNCTION__, Incoming->header.subType);
            break;
    }
}

void *gui_msg_task()
{
    REST_GUI_MSG_T gui_msg;
    int gui_msgid = create_msg(REST_GUI_MSGQ_KEY);
    if(gui_msgid < 0)
    {
        rest_log(ERR, "Failed to open GUI message\n");
        return;
    }

    while(1)
    {
        recv_msg(gui_msgid, (void *) &gui_msg, sizeof(REST_GUI_MSG_T), 5);
        gui_msg_handler((REST_GUI_MSG_T *) &gui_msg);
        sleep(1);
    }
}

void *gui_dog_bark_task()
{
    while(1) {
        send_dog_bark(REST_GUI_MODULE_ID);
        sleep(1);
    }
}

int rest_gui_init()
{
	int result;
	//start thread for module dog bark response
    pthread_t gui_dog_id = -1, gui_msg_id = -1;
    if(gui_dog_id == -1)
    {
        result = pthread_create(&gui_dog_id, NULL, gui_dog_bark_task, NULL);
        if(result == 0)
            rest_log(INFO, "Starting sensor dog bark thread.\n");
        else
            rest_log(ERR, "sensor thread launch failed\n");
    }

	// start thread for interprocess message
    if(gui_msg_id == -1)
    {
        result = pthread_create(&gui_msg_id, NULL, gui_msg_task, NULL);
        if(result == 0)
            rest_log(INFO, "Starting gui message thread.\n");
        else
            rest_log(ERR, "gui message thread launch failed\n");
    }
}


void *rest_gui_main_task()
{
	char gui_data[32];
	sleep(5); //Make sure watchdog ready
	rest_gui_init();
    rest_get_data(6, (char *) &gui_data);
	rest_log(INFO, "GUI get data item: %s\n", (char *) &gui_data);

	while(1)
	{
		//GUI business main loop 
		rest_log(DBG,"%s Do something then sleep\n", __FUNCTION__);
		sleep(TASK_SLEEP);	
	}

}

