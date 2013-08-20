#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"

int main(){	
	int Run_Magic_Loop=1;		
	int redraw=0;
	int tick_counter=0;
	int seconds_counter=0;
	float seconds = 1.0;
	float FPS = 500.0;
	ALLEGRO_EVENT_QUEUE *queue;						
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT event;
		
	if (!al_init()){								
		printf("\n[ERROR] Could not init Allegro.");
		return 1;									
	}												
	queue = al_create_event_queue();

	timer = al_create_timer(seconds / FPS);
	al_start_timer(timer);
	
	//al_install_mouse();								
	al_install_keyboard();
	al_init_image_addon();
	
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	
	while(Run_Magic_Loop){
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
			break;
		if (event.type == ALLEGRO_EVENT_KEY_CHAR){
			switch(event.keyboard.keycode){
				case ALLEGRO_KEY_ESCAPE:
					printf("\nESC Key pressed!");
					Run_Magic_Loop=0;
					break;
				default:
					// Nothing
					break;
			}
		}

		if(event.type == ALLEGRO_EVENT_KEY_UP){
			switch(event.keyboard.keycode){
				//case ALLEGRO_KEY_ENTER:
				//	break;
				default:
					// Nothing
					break;
			}
		}
		if(event.type == ALLEGRO_EVENT_TIMER){
			redraw = true;
		}
		
		if (redraw && al_is_event_queue_empty(queue)){	
			redraw = false;
			printf("\n IN Timer Loop!");
			printf("\n\ttick_counter:%i , seconds_counter:%i",tick_counter,seconds_counter);
			tick_counter++;
			if(tick_counter%(int)FPS==0)
				seconds_counter++;
		}
	}
	return 0; //Program Completed Successfully
}

