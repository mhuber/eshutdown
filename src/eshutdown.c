#include <stdio.h>
#include <libintl.h>

#include <Ecore.h>
#include <Ecore_Con.h>
#include <Ecore_Evas.h>
#include <Evas.h>
#include <Edje.h>

#ifndef DATADIR
#define DATADIR "."
#endif

#define POWER "Power"

Ecore_Evas *main_win;

typedef struct
{
    char* msg;
    int size;
} client_data_t;

static void shutdown()
{
	printf("shutdown\n");
}

static void
key_handler(void *data, Evas *evas, Evas_Object *obj, void *event_info)
{
	Evas_Event_Key_Up* e = (Evas_Event_Key_Up*)event_info;

	const char* k = e->keyname;

	printf("keypress: %s\n", k);

	if(!strcmp(k, "XF86PowerOff") || !strcmp(k, "Return"))
		shutdown();
	else if(!strcmp(k, "Escape"))
		ecore_evas_hide(main_win);
}


static int _client_add(void* param, int ev_type, void* ev)
{
    Ecore_Con_Event_Client_Add* e = ev;
    client_data_t* msg = malloc(sizeof(client_data_t));
    msg->msg = strdup("");
    msg->size = 0;
    ecore_con_client_data_set(e->client, msg);
    return 0;
}

static int _client_del(void* param, int ev_type, void* ev)
{
    Ecore_Con_Event_Client_Del* e = ev;
    client_data_t* msg = ecore_con_client_data_get(e->client);

    /* Handle */
	if(strlen(POWER) == msg->size && !strncmp(POWER, msg->msg, msg->size))
		if(ecore_evas_visibility_get(main_win))
			shutdown();
		else
			ecore_evas_show(main_win);

    //printf(": %.*s(%d)\n", msg->size, msg->msg, msg->size);


    free(msg->msg);
    free(msg);
    return 0;
}

static int _client_data(void* param, int ev_type, void* ev)
{
    Ecore_Con_Event_Client_Data* e = ev;
    client_data_t* msg = ecore_con_client_data_get(e->client);
    msg->msg = realloc(msg->msg, msg->size + e->size);
    memcpy(msg->msg + msg->size, e->data, e->size);
    msg->size += e->size;
    return 0;
}

int main(int argc, char **argv)
{
	ecore_init();
	ecore_con_init();
	ecore_evas_init();
	edje_init();

	setlocale(LC_ALL, "");
	textdomain("eshutdown");

	ecore_con_server_add(ECORE_CON_LOCAL_USER, "eshutdown", 0, NULL);

	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD, _client_add, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA, _client_data, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL, _client_del, NULL);

	main_win = ecore_evas_software_x11_new(0, 0, 0, 0, 600, 300);
	ecore_evas_borderless_set(main_win, 0);
	ecore_evas_shaped_set(main_win, 0);
	ecore_evas_title_set(main_win, "eshutdown");
	ecore_evas_name_class_set(main_win, "eshutdown", "eshutdown");

	Evas *main_canvas = ecore_evas_get(main_win);

	Evas_Object *edje = edje_object_add(main_canvas);
	evas_object_name_set(edje, "edje");
	edje_object_file_set(edje, DATADIR "/eshutdown/themes/eshutdown.edj", "eshutdown");
	evas_object_move(edje, 0, 0);
	evas_object_resize(edje, 600, 300);
	evas_object_show(edje);
	evas_object_focus_set(edje, 1);
	evas_object_event_callback_add(edje, EVAS_CALLBACK_KEY_DOWN, &key_handler, NULL);

	edje_object_part_text_set(edje, "eshutdown/text/ok/label2", "OFF");
	edje_object_part_text_set(edje, "eshutdown/text/cancel/label2", "Cancel");

	char *t1 = gettext("OFF"), 
		 *t2 = gettext("Cancel");

	if(!strcmp("OFF", t1) && !strcmp("Cancel", t2)) {
		edje_object_signal_emit(edje, "set_noloc", "");
	} else {
		edje_object_signal_emit(edje, "set_loc", "");
		edje_object_part_text_set(edje, "eshutdown/text/ok/label1", t1);
		edje_object_part_text_set(edje, "eshutdown/text/cancel/label1", t2);
	}

	ecore_evas_show(main_win);
	ecore_main_loop_begin();

	edje_shutdown();
	ecore_evas_shutdown();
	ecore_con_shutdown();
	ecore_shutdown();

	return 0;
}
