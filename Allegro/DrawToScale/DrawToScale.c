// gcc -I"E:\My Programing\Libraries\Allegro\allegro-5.0.8-mingw-4.6.2\include" -L"E:\My Programing\Libraries\Allegro\allegro-5.0.8-mingw-4.6.2\lib" -o DrawToScale DrawToScale.c -lallegro-5.0.8-monolith-md

#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "include/PE1_ENGINE.h"
//#include "common.c"

int main(){
	PE1_ENGINE_VARIABLES_INSTALL();
    PE1_ENGINE_DISPLAY_INSTALL();
	PE1_ENGINE_TIMER_INSTALL();
	
    PE1_DATA_IMAGE *bitmap;

    //bool redraw = true;
    double zoom = 1;
    double t0;
    double t1;

    PE1_ENGINE_ALLEGRO_START();
	PE1_ENGINE_IO_DEFAULT_INSTALL();

    PE1_ENGINE_DISPLAY_START(1920,1080);
    PE1_ENGINE_DISPLAY_WINDOW_START("DrawToScale");

	printf("\n\n\nDisplay Stats:\n===================================\n");
	printf("[DEBUGGING] This is the Display Width:%i and Height:%i \n",(int)PE1_Display_width,(int)PE1_Display_height);
	printf("\n[DEBUGGING] This is display 0's info: (%d, %d) - (%d, %d)", PE1_Monitor_info.x1, PE1_Monitor_info.y1, PE1_Monitor_info.x2, PE1_Monitor_info.y2);
	printf("\n[DEBUGGING] This is Display Scalar: %f", PE1_DISPLAY_SCALAR);
	printf("\n[DEBUGGING] This is the Monitor aspect ratio: %f", PE1_Monitor_Aspect_Ratio);
	printf("\n[DEBUGGING] This is the Display aspect ratio: %f", PE1_Display_Aspect_Ratio);
	printf("\n[DEBUGGING] This is the Aspect Ratio Y Offset: %f",PE1_Aspect_Ratio_offset_y);
	printf("\n[DEBUGGING] This is the Display Height: %f , Y2*Scalar: %f, Y2-DH*S: %f",PE1_Display_height, (float)PE1_Monitor_info.y2*PE1_DISPLAY_SCALAR,PE1_Display_height-(float)PE1_Monitor_info.y2*PE1_DISPLAY_SCALAR);
				
    /* We load the bitmap into a memory bitmap, because creating a
     * display bitmap could fail if the bitmap is too big to fit into a
     * single texture.
     * FIXME: Or should A5 automatically created multiple display bitmaps?
     */
	 
	PE1_DATA_LOAD_INSTALL();
    t0 = al_get_time();
	PE1_DATA_LOAD_IMAGE_FROM_FILE(bitmap,"images/bg.jpeg");
    printf("\n[DEBUGGING]Loading took %.4f seconds\n", t1 - t0);
    
    // FIXME: 
    // Now try to split the memory bitmap into display bitmaps?
    /*
	
	bitmap = al_clone_bitmap(membitmap);
    if (!bitmap)
        bitmap = membitmap;
		
	*/
	
    PE1_ENGINE_TIMER_FPS_START(30);
	PE1_ENGINE_VARIABLES_START();
	
    PE1_ENGINE_LOOP_START
        if (event.type == ALLEGRO_EVENT_DISPLAY_ORIENTATION) {
            int o = event.display.orientation;
            if (o == ALLEGRO_DISPLAY_ORIENTATION_0_DEGREES) {
                printf("0 degrees\n");
            }
            else if (o == ALLEGRO_DISPLAY_ORIENTATION_90_DEGREES) {
                printf("90 degrees\n");
            }
            else if (o == ALLEGRO_DISPLAY_ORIENTATION_180_DEGREES) {
                printf("180 degrees\n");
            }
            else if (o == ALLEGRO_DISPLAY_ORIENTATION_270_DEGREES) {
                printf("270 degrees\n");
            }
            else if (o == ALLEGRO_DISPLAY_ORIENTATION_FACE_UP) {
                printf("Face up\n");
            }
            else if (o == ALLEGRO_DISPLAY_ORIENTATION_FACE_DOWN) {
                printf("Face down\n");
            }
        }

        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                PE1_Run_Magic_Loop=0;
            if (event.keyboard.unichar == '1')
                zoom = 1;
            if (event.keyboard.unichar == '+')
                zoom *= 1.1;
            if (event.keyboard.unichar == '-')
                zoom /= 1.1;
            if (event.keyboard.unichar == 'f')
                zoom = (double)al_get_display_width(display) /
                    al_get_bitmap_width(bitmap);
        }
        if (event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
            
        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb_f(0, 0, 0));
            if (zoom == 1)
                al_draw_bitmap(bitmap, 0, 0, 0);
            else
                al_draw_scaled_rotated_bitmap(
                    bitmap, 0, 0, 0, 0, zoom, zoom, 0, 0);
            al_flip_display();
        }
    PE1_ENGINE_LOOP_STOP

    al_destroy_bitmap(bitmap);

    return 0;
}

/* vim: set sts=4 sw=4 et: */
