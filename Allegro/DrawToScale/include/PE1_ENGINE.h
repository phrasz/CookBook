#include "PE1_DATA.h"
#include "PE1_ANIMATIONAUDIO.h"
#include "PE1_GAME.h"

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include "allegro5/allegro_font.h"
#include <allegro5/allegro_ttf.h>
//#include <allegro5/allegro_color.h>

// PE1_GameStates:
#define PE1_STATE_INTRO                   0
#define PE1_STATE_TITLE                   1
#define PE1_STATE_EXIT                  100
#define PE1_STATE_MAIN_MENU          100000
#define PE1_STATE_GAME_INITIALIZE    200010
#define PE1_STATE_GAME_PLAY          200100
#define PE1_STATE_GAME_MENU       	 210000 //MenuScreens: 2100XX - 2199XX; Submenu Options: 21YY00-21YY-99; 100 Menus w/ 100 sub options each! 
#define PE1_STATE_GAME_MENU_CONFIRM	 210001
#define PE1_STATE_GAME_MENU_SETTINGS 210002
#define PE1_STATE_GAME_PAUSE         220000

// PP MACROS
#define PE1_ENGINE_ALLEGRO_START()						\
		if (!al_init()){								\
			printf("\n[ERROR] Could not init Allegro.");\
			return 1;									\
		}

#define PE1_ENGINE_DISPLAY_INSTALL()					\
		ALLEGRO_DISPLAY *display;						\
		ALLEGRO_BITMAP *membitmap;						\
		ALLEGRO_MONITOR_INFO PE1_Monitor_info;			\
		float PE1_DISPLAY_SCALAR;						\
		float PE1_Monitor_Aspect_Ratio;					\
		float PE1_Display_Aspect_Ratio;					\
		float PE1_Aspect_Ratio_offset_y;				\
		float PE1_Display_Default_width=1920.0;			\
		float PE1_Display_Default_height=1080.0;		\
		float PE1_Display_width;						\
		float PE1_Display_height;
		
#define PE1_ENGINE_DISPLAY_DEFAULT(display_x1,display_y1)		\
		PE1_Display_Default_width=(float)display_x1;			\
		PE1_Display_Default_height=(float)display_y1;
		
// 1280, 720 ;;; 1920, 1080 ===> 1920/1280 > 1 ; == 1.5
#define PE1_ENGINE_DISPLAY_START(display_x2,display_y2)															\
	PE1_Display_width=(float)display_x2;																		\
	PE1_Display_height=(float)display_y2;																		\
	display = al_create_display((int)PE1_Display_width, (int)PE1_Display_height);								\
	if (!display){																								\
		printf("\n[ERROR] Error creating display");																\
		return 1;																								\
		}																										\
	al_get_monitor_info(0, &PE1_Monitor_info);																	\
	PE1_DISPLAY_SCALAR = PE1_Display_Default_width/PE1_Display_width;											\
	PE1_Monitor_Aspect_Ratio = (float)PE1_Monitor_info.x2/(float)PE1_Monitor_info.y2;							\
	PE1_Display_Aspect_Ratio = PE1_Display_width/PE1_Display_height;											\
																												\
	PE1_Aspect_Ratio_offset_y=0;																				\
	if(PE1_DISPLAY_SCALAR<1.0){																					\
		if(PE1_Monitor_Aspect_Ratio!=PE1_Display_Aspect_Ratio){													\
			PE1_Aspect_Ratio_offset_y=(PE1_Display_height-(float)PE1_Monitor_info.y2*PE1_DISPLAY_SCALAR)/2.0;	\
		}																										\
	}																											\
	else if(PE1_DISPLAY_SCALAR>=1.0){																			\
		PE1_DISPLAY_SCALAR=1.0/PE1_DISPLAY_SCALAR;																\
		if(PE1_Monitor_Aspect_Ratio!=PE1_Display_Aspect_Ratio){													\
			PE1_Aspect_Ratio_offset_y=(PE1_Display_height-(float)PE1_Monitor_info.y2*PE1_DISPLAY_SCALAR)/2.0;	\
		}																										\
	}																										

//	PE_Monitor_scale_y = (float)PE_Monitor_info.y2/(float)display_y;	!!! CHECK FOR USAGE !!!
	
#define	PE1_ENGINE_DISPLAY_WINDOW_FULLSCREEN(DisplayName)			\
		al_set_display_flag(DisplayName, ALLEGRO_FULLSCREEN_WINDOW, 1);
	
#define	PE1_ENGINE_DISPLAY_WINDOW_START(WindowName)		\
		al_set_window_title(display, WindowName);		\
		al_set_window_position(display, 0, 0);
//Depends on: PE1_INSTALL_DISPLAY()

#define PE1_ENGINE_DISPLAY_ICON(DisplayName,IconBitmap)	\
		al_set_display_icon(DisplayName,IconBitmap);
		

#define PE1_ENGINE_VARIABLES_INSTALL()							\
		int PE1_GameState;								\
		int PE1_Run_Magic_Loop;							\
		int x, y, i, j,redraw =0;						\
		int PE1_Timer_Counter;							\
		int	PE1_ENGINE_CONSOLE_ENABLED;					\
		char PE1_ENGINE_CONSOLE_OPTIONS[50];			\
		time_t current_time;							\
		char PE1_ENGINE_LocalTime[20];
	
#define PE1_ENGINE_VARIABLES_START()						\
		PE1_Run_Magic_Loop=1;								\
		PE1_GameState = PE1_STATE_GAME_INITIALIZE;			\
		PE1_Timer_Counter=0;								\
		PE1_ENGINE_CONSOLE_ENABLED=0;						\
		for(i=0;i<50;i++){									\
		PE1_ENGINE_CONSOLE_OPTIONS[i]=0;				\
		}	

#define	PE1_ENGINE_CONSOLE_TOGGLE()									\
		PE1_ENGINE_CONSOLE_ENABLED=PE1_ENGINE_CONSOLE_ENABLED^1;

#define	PE1_ENGINE_CONSOLE_START	if(PE1_ENGINE_CONSOLE_ENABLED){

#define	PE1_ENGINE_CONSOLE_STOP	}

#define	PE1_ENGINE_CONSOLE_OPTION_TOGGLE(Value)		\
		PE1_ENGINE_CONSOLE_OPTIONS[Value]=PE1_ENGINE_CONSOLE_OPTIONS[Value]^1;

#define	PE1_ENGINE_CONSOLE_OPTION_START(Value)		\
		if(PE1_ENGINE_CONSOLE_OPTIONS[Value]==1){
		
#define	PE1_ENGINE_CONSOLE_OPTION_STOP	}

#define	PE1_ENGINE_CONSOLE_DEBUGGING_ON(DebugVal)		\
		PE1_ENGINE_CONSOLE_START						\
		PE1_ENGINE_CONSOLE_OPTION_START(DebugVal)

#define	PE1_ENGINE_CONSOLE_DEBUGGING_OFF				\
		PE1_ENGINE_CONSOLE_OPTION_STOP				\
		PE1_ENGINE_CONSOLE_STOP
			
#define PE1_ENGINE_IO_DEFAULT_INSTALL()						\
		al_install_mouse();								\
		al_install_keyboard();							\
		al_init_image_addon();							\
		
#define PE1_ENGINE_IO_FONTS_INSTALL()					\
		al_init_font_addon();							\
		al_init_ttf_addon();
				
#define PE1_ENGINE_TIMER_INSTALL()	\
		ALLEGRO_TIMER *timer;		\
		ALLEGRO_EVENT_QUEUE *queue;
		
#define PE1_ENGINE_GET_LOCAL_TIME()												\
		current_time = time(NULL);												\
		strftime (PE1_ENGINE_LocalTime, 50,"%I:%M:%S",localtime(&current_time));	


		
#define PE1_ENGINE_TIMER_FPS_START(FPS)											\
		timer = al_create_timer(1.0 / FPS);										\
		queue = al_create_event_queue();										\
		al_register_event_source(queue, al_get_keyboard_event_source());		\
		al_register_event_source(queue, al_get_display_event_source(display));	\
		al_register_event_source(queue, al_get_timer_event_source(timer));		\
		al_start_timer(timer);
		
//Depends on: PE1_INSTALL_DISPLAY(); PE1_INSTALL_TIMER();


#define PE1_ENGINE_MONITOR_INFO_DEBUG()											\
	ALLEGRO_MONITOR_INFO PE1_Monitor_info;										\
	int num_adapters;															\
	num_adapters = al_get_num_video_adapters();									\
	printf("\n[DEBUGGING] System Currently Has %i Monitors", num_adapters);		\
	for (i = 0; i < num_adapters; i++) {										\
      al_get_monitor_info(i, &PE1_Monitor_info);									\
      printf("\n[DEBUGGING] Adapter %d: ", i);									\
      printf("\n[DEBUGGING] (%d, %d) - (%d, %d)\n", PE1_Monitor_info.x1, PE1_Monitor_info.y1, PE1_Monitor_info.x2, PE1_Monitor_info.y2);\
      al_set_new_display_adapter(i);											\
      printf("\n[DEBUGGING]\t Available fullscreen display modes:\n");			\
      for (j = 0; j < al_get_num_display_modes(); j++) {						\
         ALLEGRO_DISPLAY_MODE mode;												\
         al_get_display_mode(j, &mode);											\
         printf("\n[DEBUGGING]\t    Mode %3d: %4d x %4d, %d Hz\n",j, mode.width, mode.height, mode.refresh_rate);	\
      }																			\
   }

#define PE1_ENGINE_LOOP_START							\
	while (PE1_Run_Magic_Loop){							\
		ALLEGRO_EVENT event;							\
		al_wait_for_event(queue, &event);				\
														\
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 	\
			break;										
			
#define PE1_ENGINE_LOOP_STOP	}

#define PE1_ENGINE_AUDIO_SYSTEM_INSTALL()																			\
		ALLEGRO_VOICE* PE1_AUDIO_voice;																	\
		ALLEGRO_MIXER* PE1_AUDIO_mixer;																	\
		float	PE1_AUDIO_EFFECT_GAIN = 1.0;															\
		float	PE1_AUDIO_SONG_GAIN = 1.0;																\
		int		PE1_AUDIO_VOLUME_MODE=0;																\
		float	PE1_AUDIO_VOLUME_MUTE=1;																\
																										\
		al_init_acodec_addon();																			\
																										\
		if (!al_install_audio()) {																		\
			printf("\n[ERROR] Could not init sound!");													\
			return 1;																					\
		}																								\
		if (!al_reserve_samples(1)) {																	\
			printf("\n[ERROR] Could not set up voice and mixer!");										\
			return 1;																					\
		}																								\
		PE1_AUDIO_voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);	\
		if (!PE1_AUDIO_voice) {																					\
			printf("\n[ERROR] Could not create ALLEGRO_VOICE!"); 										\
			return 1;																					\
		}																								\
		PE1_AUDIO_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);	\
		if (!PE1_AUDIO_mixer) {																					\
			printf("\n[ERROR] Could not create ALLEGRO_MIXER!");										\
			return 1;																					\
		}																								\
		if (!al_attach_mixer_to_voice(PE1_AUDIO_mixer, PE1_AUDIO_voice)){								\
			printf("\n[ERROR] al_attach_mixer_to_voice failed!");										\
			return 1;																					\
		}																								\
		al_reserve_samples(20);

		//		float PE1_Elapsed_Stream_Playtime;
		
#define	PE1_ENGINE_AUDIO_SYSTEM_REMOVE()		\
		al_destroy_mixer(PE1_AUDIO_mixer);	\
		al_destroy_voice(PE1_AUDIO_voice);	\
		al_uninstall_audio();
