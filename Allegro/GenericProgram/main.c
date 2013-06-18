// gcc -I"E:\My Programing\Libraries\Allegro\allegro-5.0.8-mingw-4.6.2\include" -L"E:\My Programing\Libraries\Allegro\allegro-5.0.8-mingw-4.6.2\lib" -o GenericProgram main.c -lallegro-5.0.8-monolith-md
// gcc -I"C:\Users\Mike\Desktop\My programming\Libraries\Allegro\include" -L"C:\Users\Mike\Desktop\My programming\Libraries\Allegro\bin" -o GenericProgram main.c -lallegro-5.0.8-monolith-md

#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
//#include "allegro5/allegro_audio.h"
//#include "allegro5/allegro_acodec.h"
//#include "allegro5/allegro_font.h"
//#include <allegro5/allegro_ttf.h>

//Constructor(s)
//pe1_paint_2d_gameboard_icon(float x, float y, ALLEGRO_BITMAP *bitmap);

int main(){
	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//
	//                 Allegro Variables
		int x, y, i, j, runthisgame, redraw=0;
		//float dx, dy;
		float scale_ratio = 0.10; // 1080 --> 720 == 720/1080 = .666666
		int tile1_w, tile1_h;
		float tile2_w, tile2_h =240.0;
		int tilecounter, row_counter;
		int gameboard_tile_columns, gameboard_tile_rows;
		float board_offset_y, board_offset_x;
		int tile_placement_counter;
		
		ALLEGRO_DISPLAY *display;
		ALLEGRO_BITMAP *membitmap;//, *bitmap;
		ALLEGRO_BITMAP *black_icon, *white_icon, *bg, *tile1, *tile2; //, *opt2, *opt3, *credits, *cred1, *cred2, *cred3 //Main menu and Credits

		ALLEGRO_TIMER *timer;
		ALLEGRO_EVENT_QUEUE *queue;
	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//

	
	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//
	//                 Allegro Bootup
	//Return if allegro cannot start
		if (!al_init()){
			printf("\n[ERROR] Could not init Allegro.");
			return 1;
			// abort_example("Could not init Allegro.\n");
		}
	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//


	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//
	//                  I/O Devices Installation
		al_install_mouse();
		al_install_keyboard();
		al_init_image_addon();
		//al_init_font_addon();
		//al_init_ttf_addon();
	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//

	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//
	//                  Display Creation

	display = al_create_display(1280, 720);
	if (!display){
		printf("\n[ERROR] Error creating display");
		return 1;
	}
	x=al_get_display_width(display)/2; // should be floats and have int's typecasted
	y=al_get_display_height(display)/2; // should be floats and have int's typecasted
	printf("\n[DEBUGGING] This is the Display Width:%i and Height:%i \n",x,y);

	al_set_window_title(display, "2d Icon Painter Example");
	al_set_window_position(display, 0, 0);
	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//

	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//
	//                      BITMAP LAODING
	/* We load the bitmap into a memory bitmap, because creating a
	* display bitmap could fail if the bitmap is too big to fit into a
	* single texture.
	* FIXME: Or should A5 automatically created multiple display bitmaps?
	*/
	//ALLEGRO_BITMAP *black, *powered, *PPlogo, *gimp, *presents, *menuscreen;

	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	membitmap = al_load_bitmap("images/bg.png");
	if (!membitmap) {
		//abort_example("%s not found or failed to load\n");
		printf("\n[ERROR] bg.png Failed to Load!!!");
	}
	//else
		//al_convert_mask_to_alpha( membitmap, al_map_rgb(255,0,255) );

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	bg =al_clone_bitmap(membitmap);
	al_destroy_bitmap(membitmap);
	
	membitmap = al_load_bitmap("images/blk.png");
	if (!membitmap) {
		//abort_example("%s not found or failed to load\n");
		printf("\n[ERROR] blk.png Failed to Load!!!");
	}
	else
		al_convert_mask_to_alpha( membitmap, al_map_rgb(255,0,255) );

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	black_icon =al_clone_bitmap(membitmap);
	al_destroy_bitmap(membitmap);

	membitmap = al_load_bitmap("images/wht.png");
	if (!membitmap) {
		//abort_example("%s not found or failed to load\n");
		printf("\n[ERROR] wht.png Failed to Load!!!");
	}
	else
		al_convert_mask_to_alpha( membitmap, al_map_rgb(255,0,255) );

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	white_icon =al_clone_bitmap(membitmap);
	al_destroy_bitmap(membitmap);
	
	
	membitmap = al_load_bitmap("images/tile1.png");
	if (!membitmap) {
		//abort_example("%s not found or failed to load\n");
		printf("\n[ERROR] wht.png Failed to Load!!!");
	}
	else
		al_convert_mask_to_alpha( membitmap, al_map_rgb(255,0,255) );

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	tile1 =al_clone_bitmap(membitmap);
	al_destroy_bitmap(membitmap);
	
		membitmap = al_load_bitmap("images/tile2.png");
	if (!membitmap) {
		//abort_example("%s not found or failed to load\n");
		printf("\n[ERROR] wht.png Failed to Load!!!");
	}
	else
		al_convert_mask_to_alpha( membitmap, al_map_rgb(255,0,255) );

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	tile2 =al_clone_bitmap(membitmap);
	al_destroy_bitmap(membitmap);
	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//


	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//
	//                  Timer Function Variables
	timer = al_create_timer(1.0 / 60); //60 FPS
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//

	
	
	///Intilize Varaiables;
	tile1_w = 240.0;
	tile1_h = 240.0;
	board_offset_y= 200.0;
	board_offset_x=200.0;
	tilecounter =0;
	gameboard_tile_columns = 8;
	gameboard_tile_rows = 8;
	tile_placement_counter=0;
	///-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~///
	///                  THE MAGIC WHILE LOOP
	/// Here's the start of the execution of the game
	// x=al_get_display_width(display)/2;
	// y=al_get_display_height(display)/2;
	//dx=640.0;
	//dy=360.0;
	//printf("\nX=%i, Y=%i", x, y);
	//x=1280;
	//y=720;
	//gamestate=1; startload=1; song_start=0; round=1;
	
	runthisgame=1;
	printf("\n[DEBUGGING] Getting Ready to start the game!");
	while (runthisgame==1){//runthisgame) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //User closes the window
			break;
		if (event.type == ALLEGRO_EVENT_KEY_CHAR){
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				runthisgame=0;
			}
		}

		if(event.type == ALLEGRO_EVENT_KEY_UP){
			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
			}
		}

		if(event.type == ALLEGRO_EVENT_TIMER)
			redraw = true;

		if (redraw && al_is_event_queue_empty(queue)){
			redraw = false;
			
			//al_clear_to_color(al_map_rgb_f(0, 0, 0));
			//MAIN PROGRAM LOGIC!!!!
			printf("\n[DEBUGGING] In the While loop!");
			al_draw_bitmap(bg,0,0,0);
			
			//al_draw_bitmap(tile1,200,200,0);
			//printf("\n[DEBUGGING] Tile1_w: %f, Tile1_h: %f", tile1_w, tile1_h);
			
			//tilecoutner == 0.0
			
			tile_placement_counter=0; // Either/Or ... Even/Odd
			row_counter=0;
			for(i=0; i<(gameboard_tile_columns*gameboard_tile_rows); i++){
				if(tile_placement_counter%2 == 0 ){//Even or 0
					al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*i, board_offset_y+(tile1_h*scale_ratio)*row_counter, tile1_w*scale_ratio, tile1_h*scale_ratio, 0);
				}
				else{ //Odd
					al_draw_scaled_bitmap(tile2, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*i, board_offset_y+(tile1_h*scale_ratio)*row_counter, tile1_w*scale_ratio, tile1_h*scale_ratio, 0);
				}
				tile_placement_counter++;
				if(i%gameboard_tile_columns == 0){ //after tile_placement_counter++; ==> skip very first tile
					row_counter++;
				}
			}
			
			// al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*0, board_offset_y, tile1_w*scale_ratio, tile1_h*scale_ratio, 0);
			// al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*1, board_offset_y, tile1_w*scale_ratio, tile1_h*scale_ratio, 0); //go right 1 tile	
			// al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*2, board_offset_y, tile1_w*scale_ratio, tile1_h*scale_ratio, 0); //go right 1 tile	
			// al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*3, board_offset_y, tile1_w*scale_ratio, tile1_h*scale_ratio, 0); //go right 1 tile	
			// al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*4, board_offset_y, tile1_w*scale_ratio, tile1_h*scale_ratio, 0); //go right 1 tile	
			// al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*5, board_offset_y, tile1_w*scale_ratio, tile1_h*scale_ratio, 0); //go right 1 tile	
			// al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*6, board_offset_y, tile1_w*scale_ratio, tile1_h*scale_ratio, 0); //go right 1 tile	
			// al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,board_offset_x+(tile1_w*scale_ratio)*7, board_offset_y, tile1_w*scale_ratio, tile1_h*scale_ratio, 0); //go right 1 tile	



			
			//al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,200, 200, tile1_w*scale_ratio, tile1_h*scale_ratio, 0);
			//al_draw_scaled_bitmap(tile1, 0, 0, 240, 240,200+240*scale_ratio, 200, tile1_w*scale_ratio, tile1_h*scale_ratio, 0); //go right 1 tile
			
			//al_draw_bitmap(gbg,0,0,0); //draw first screen (current values/words/strikes)
			al_flip_display();
		}
	}

	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//
	//                  DATA DESTRUCTION
	//al_destroy_bitmap(membitmap);
	// al_destroy_bitmap(bitmap);

	//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~//


	return 0; //Program Completed Successfully


}



//Functions

