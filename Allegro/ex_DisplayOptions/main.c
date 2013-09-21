/* Test retrieving and settings possible modes. */
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "common.c"

ALLEGRO_FONT *font;
ALLEGRO_COLOR white;
int font_h;
int modes_count;
int options_count;
char status[256];
int flags, old_flags;

int visible_rows;
int first_visible_row;

int selected_mode;

static void load_font(void){
	font = al_load_font("data/fixed_font.tga", 0, 0);
	if (!font) {
	abort_example("data/fixed_font.tga not found\n");
	exit(1);
	}
	font_h = al_get_font_line_height(font);
}

static void display_options(ALLEGRO_DISPLAY *display){
	int i, y = 10;
	int x = 10;
	int n = options_count;
	int dw = al_get_display_width(display);
	int dh = al_get_display_height(display);
	ALLEGRO_COLOR c;

	modes_count = al_get_num_display_modes();

	for (i = first_visible_row; i < modes_count + 2 &&	i < first_visible_row + visible_rows; i++){
			ALLEGRO_DISPLAY_MODE mode;
			if (i > 1) {
				al_get_display_mode(i - 2, &mode);
			}
			else if (i == 1){ //FS Window
				mode.width = 1280;
				mode.height = 720;
				mode.format = 0;
				mode.refresh_rate = 0;
			}
			else { //Default Window
				mode.width = 800;
				mode.height = 600;
				mode.format = 0;
				mode.refresh_rate = 0;
			}
			if (selected_mode == i){ //Selected Value
				c = al_map_rgb_f(1, 1, 0);
				al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
				al_draw_filled_rectangle(x, y, x + 300, y + font_h, c);
			}
			c = al_map_rgb_f(0, 0, 0);
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
			
			
			if ((i == first_visible_row && i > 0) || (i == first_visible_row + visible_rows - 1 && i < modes_count + 1)) {
				al_draw_textf(font, c, x, y, 0, "...");
			}
			else {
				al_draw_textf(font, c, x, y, 0, "%s %d x %d (fmt: %x, %d Hz)", i > 1 ? "Fullscreen" : i == 0 ? "Windowed" : "FS Window", mode.width, mode.height, mode.format, mode.refresh_rate);
			}
			y += font_h;
	}

}

static void update_ui(void){
	int h = al_get_display_height(al_get_current_display());
	visible_rows = h / font_h - 10;
}

int main(void){
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	bool redraw = false;

	if (!al_init()) {
		abort_example("Could not init Allegro.\n");
		return 1;
	}
	//init_flags();
	al_init_primitives_addon();

	white = al_map_rgba_f(1, 1, 1, 1);

	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();

	display = al_create_display(1280, 720);
	if (!display){
		abort_example("Could not create display.\n");
		return 1;
	}

	load_font();

	timer = al_create_timer(1.0 / 60);
	
	al_set_new_display_refresh_rate(60);
	modes_count = al_get_num_display_modes();

	update_ui();

	al_clear_to_color(al_map_rgb_f(1, 1, 1));
	display_options(display);
	al_flip_display();

	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, 
	al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (1){
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			if (event.mouse.button == 1){
				int dw = al_get_display_width(display);
				int y = 10;
				int row = (event.mouse.y - y) / font_h - 1;
				int column = event.mouse.x / (dw / 2);
				
				if (row >= 0 && row <= modes_count) {
					selected_mode = row;
					redraw = true;
				}
			}
		}
		if (event.type == ALLEGRO_EVENT_TIMER) {
			int f = al_get_display_flags(display);
			if (f != flags) {
				redraw = true;
				flags = f;
				old_flags |= f;
			}
		}
		if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				break;
			if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
				selected_mode -= 1;
				redraw = true;
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				selected_mode += 1;
				redraw = true;
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
				ALLEGRO_DISPLAY_MODE mode;
				ALLEGRO_DISPLAY *new_display;
				if (selected_mode > 1){ //Set new Display
					al_get_display_mode(selected_mode - 2, &mode);
					al_set_new_display_flags(ALLEGRO_FULLSCREEN);
				}
				else if (selected_mode == 1) { //
					mode.width = 800;
					mode.height = 600;
					al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
				}
				else { //catch @ 0
					mode.width = 800;
					mode.height = 600;
					al_set_new_display_flags(ALLEGRO_WINDOWED);
				}

				al_destroy_font(font);
				font = NULL;

				new_display = al_create_display(mode.width, mode.height);
				if (new_display){
					al_destroy_display(display);
					display = new_display;
					al_set_target_backbuffer(display);
					al_register_event_source(queue,al_get_display_event_source(display));
					update_ui();
					sprintf(status, "Display creation succeeded.");
				}
				else {
					sprintf(status, "Display creation failed.");
				}

				load_font();
				
				redraw = true;
			}
		}

		 // Up exception handle
		if (selected_mode < 0)
			selected_mode = 0;
		if (selected_mode < first_visible_row)
			first_visible_row = selected_mode;
		
		// Down exception handle
		if (selected_mode > modes_count + 1) 
			selected_mode = modes_count + 1;
		if (selected_mode > first_visible_row + visible_rows - 1)
			first_visible_row = selected_mode - visible_rows + 1;

		if (redraw && al_is_event_queue_empty(queue)) { //Redraw queue 
			redraw = false; //toggle
			al_clear_to_color(al_map_rgb_f(1, 1, 1)); // whiteout screen
			display_options(display); // print list
			al_flip_display(); //push to screen
		}
	}

	//Clean up
	al_destroy_font(font); 

	return 0;
}

