#include <stdio.h>
#include <libintl.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Evas.h>
#include <Edje.h>

int main(int argc, char **argv)
{
	ecore_init();
	ecore_evas_init();
	edje_init();

	setlocale(LC_ALL, "");
	textdomain("eshutdown");

	Ecore_Evas *main_win = ecore_evas_software_x11_new(0, 0, 0, 0, 600, 300);
	ecore_evas_borderless_set(main_win, 0);
	ecore_evas_shaped_set(main_win, 0);
	ecore_evas_title_set(main_win, "eshutdown");
	ecore_evas_name_class_set(main_win, "eshutdown", "eshutdown");
	ecore_evas_show(main_win);

	Evas *main_canvas = ecore_evas_get(main_win);

	Evas_Object *edje = edje_object_add(main_canvas);
	evas_object_name_set(edje, "edje");
	edje_object_file_set(edje, "themes/eshutdown.edj", "eshutdown");
	evas_object_move(edje, 0, 0);
	evas_object_resize(edje, 600, 300);
	evas_object_show(edje);

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
	ecore_shutdown();

	return 0;
}
