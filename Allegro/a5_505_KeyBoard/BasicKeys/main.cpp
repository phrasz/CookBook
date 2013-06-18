/*
 *    Example program for the Allegro library, by Peter Wang.
 *    Updated by Ryan Dickie.
 *
 *    This program tests keyboard events.
 */
#include <stdio.h>
#include <allegro5/allegro.h>
//#include "common.c"

/* globals */
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_DISPLAY     *display;
ALLEGRO_EVENT event;
/* main_loop:
 *  The main loop of the program.  Here we wait for events to come in from
 *  any one of the event sources and react to each one accordingly.  While
 *  there are no events to react to the program sleeps and consumes very
 *  little CPU time.  See main() to see how the event sources and event queue
 *  are set up.
 */

int main(void){
   if (!al_init()) {
      //abort_example("Could not init Allegro.\n");
      return 1;
   }

   display = al_create_display(600, 480);
   if (!display) {
      //abort_example("al_create_display failed\n");
      return 1;
   }

   if (!al_install_keyboard()) {
      //abort_example("al_install_keyboard failed\n");
      return 1;
   }

   event_queue = al_create_event_queue();
   if (!event_queue) {
      //abort_example("al_create_event_queue failed\n");
      return 1;
   }

   al_register_event_source(event_queue, al_get_keyboard_event_source());
   al_register_event_source(event_queue, al_get_display_event_source(display));

   while (true) {
      /* Take the next event out of the event queue, and store it in `event'. */
      al_wait_for_event(event_queue, &event);

      /* Check what type of event we got and act accordingly.  ALLEGRO_EVENT
       * is a union type and interpretation of its contents is dependent on
       * the event type, which is given by the 'type' field.
       *
       * Each event also comes from an event source and has a timestamp.
       * These are accessible through the 'any.source' and 'any.timestamp'
       * fields respectively, e.g. 'event.any.timestamp'
       */
      switch (event.type) {

         /* ALLEGRO_EVENT_KEY_DOWN - a keyboard key was pressed.
          */
         case ALLEGRO_EVENT_KEY_DOWN:
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                printf("\nEsc Pressed");
               return 0;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W){
                printf("\nUp/W Pressed");
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S){
                printf("\nDown/S Pressed");
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT || event.keyboard.keycode == ALLEGRO_KEY_A){
                printf("\nLeft/A Pressed");
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_D){
                printf("\nRight/D Pressed");
            }
            //log_key("KEY_DOWN", event.keyboard.keycode, 0, 0);
            break;

         /* ALLEGRO_EVENT_KEY_UP - a keyboard key was released.
          */
         case ALLEGRO_EVENT_KEY_UP:
            //log_key("KEY_UP", event.keyboard.keycode, 0, 0);
            break;

         /* ALLEGRO_EVENT_KEY_CHAR - a character was typed or repeated.
          */
//         case ALLEGRO_EVENT_KEY_CHAR: {
//            char const *label = (event.keyboard.repeat ? "repeat" : "KEY_CHAR");
//            log_key(label,
//               event.keyboard.keycode,
//               event.keyboard.unichar,
//               event.keyboard.modifiers);
//            break;
//         }

         /* ALLEGRO_EVENT_DISPLAY_CLOSE - the window close button was pressed.
          */
         case ALLEGRO_EVENT_DISPLAY_CLOSE:
            return 1;

         /* We received an event of some type we don't know about.
          * Just ignore it.
          */
         default:
            break;
      }
   }


   //close_log(false);

   return 0;
}

/* vim: set ts=8 sts=3 sw=3 et: */
