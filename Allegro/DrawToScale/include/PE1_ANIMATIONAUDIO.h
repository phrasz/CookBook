//Default Defines
	//HERE
		
#define	PE1_AA_DRAW_FONT_TEXT(fontName,R,G,B,Xval,Yval,TheText)					\
		al_draw_textf(fontName, al_map_rgb(R, G, B), Xval*PE1_DISPLAY_SCALAR, Yval*PE1_DISPLAY_SCALAR+PE1_Aspect_Ratio_offset_y, 0, TheText);	

#define	PE1_AA_DRAW_SHADOWED_FONT_TEXT(fontName,R,G,B,Xval,Yval,SR,SG,SB,SXval,SYval,TheText)	\
		al_draw_textf(fontName, al_map_rgb(SR, SG, SB), SXval*PE1_DISPLAY_SCALAR, SYval*PE1_DISPLAY_SCALAR+PE1_Aspect_Ratio_offset_y, 0, TheText);				\
		al_draw_textf(fontName, al_map_rgb(R, G, B), Xval*PE1_DISPLAY_SCALAR, Yval*PE1_DISPLAY_SCALAR+PE1_Aspect_Ratio_offset_y, 0, TheText);

#define PE1_AA_VARIABLES_2D_GAMEBOARD_INSTALL()								\
		float tile_w, tile_h;												\
		int PE1_DATA_2D_gameboard_columns, PE1_DATA_2D_gameboard_rows;		\
		float board_offset_y, board_offset_x;								
		
#define PE1_AA_VARIABLES_2D_GAMEBOARD_START(val_board_offset_x,val_board_offset_y,val_PE1_DATA_2D_gameboard_columns,val_PE1_DATA_2D_gameboard_rows,val_tile_w,val_tile_h) \
	PE1_DATA_2D_gameboard_columns = val_PE1_DATA_2D_gameboard_columns;					\
	PE1_DATA_2D_gameboard_rows = val_PE1_DATA_2D_gameboard_rows;							\
	tile_w = val_tile_w;													\
	tile_h = val_tile_h;													\
	board_offset_y= val_board_offset_y;										\
	board_offset_x= val_board_offset_x;

	
#define PE1_AA_DRAW_2D_BOARD_2_TILES(board_offset_x,board_offset_y,PE1_DATA_2D_gameboard_columns,PE1_DATA_2D_gameboard_rows,tile1,tile2,tile_w,tile_h)    \
		int tile_placement_counter=0;															\
		int row_counter=0;																		\
			for(i=0; i<(PE1_DATA_2D_gameboard_columns*PE1_DATA_2D_gameboard_rows); i++){						\
				if(i%PE1_DATA_2D_gameboard_columns == 0 && i!=0){ 										\
					row_counter++;																\
					if(PE1_DATA_2D_gameboard_columns%2==0){tile_placement_counter++;}					\
				}																				\
				if(tile_placement_counter%2 == 0 ){												\
					al_draw_scaled_bitmap(tile1, 0, 0, tile_w, tile_h,PE1_DISPLAY_SCALAR*(board_offset_x+tile_w*(i%PE1_DATA_2D_gameboard_columns)), PE1_Aspect_Ratio_offset_y+PE1_DISPLAY_SCALAR*(board_offset_y+tile_h*row_counter), tile_w*PE1_DISPLAY_SCALAR, tile_h*PE1_DISPLAY_SCALAR, 0);	\
				}																				\
				else{																			\
					al_draw_scaled_bitmap(tile2, 0, 0, tile_w, tile_h,PE1_DISPLAY_SCALAR*(board_offset_x+tile_w*(i%PE1_DATA_2D_gameboard_columns)), PE1_Aspect_Ratio_offset_y+PE1_DISPLAY_SCALAR*(board_offset_y+tile_h*row_counter), tile_w*PE1_DISPLAY_SCALAR, tile_h*PE1_DISPLAY_SCALAR, 0);	\
				}																				\
				tile_placement_counter++;														\
			}

#define PE1_AA_DRAW_2D_BOARD_1_TILE(board_offset_x,board_offset_y,PE1_DATA_2D_gameboard_columns,PE1_DATA_2D_gameboard_rows,tile1,tile_w,tile_h)	\
		int row_counter=0;																		\
			for(i=0; i<(PE1_DATA_2D_gameboard_columns*PE1_DATA_2D_gameboard_rows); i++){						\
				if(i%PE1_DATA_2D_gameboard_columns == 0 && i!=0){ 										\
					row_counter++;																\
				}																				\
					al_draw_scaled_bitmap(tile1, 0, 0, tile_w, tile_h,PE1_DISPLAY_SCALAR*(board_offset_x+tile_w*(i%PE1_DATA_2D_gameboard_columns)), PE1_Aspect_Ratio_offset_y+PE1_DISPLAY_SCALAR*(board_offset_y+tile_h*row_counter), tile_w*PE1_DISPLAY_SCALAR, tile_h*PE1_DISPLAY_SCALAR, 0);	\
			}

#define PE1_AA_DRAW_2D_BOARD_TILE(tile1,x_val,y_val)	al_draw_scaled_bitmap(tile1, 0, 0, tile_w, tile_h,PE1_DISPLAY_SCALAR*(board_offset_x+tile_w*(x_val)), PE1_Aspect_Ratio_offset_y+PE1_DISPLAY_SCALAR*(board_offset_y+tile_h*y_val), tile_w*PE1_DISPLAY_SCALAR, tile_h*PE1_DISPLAY_SCALAR, 0);

#define	PE1_AA_DRAW_TINTED_IMAGE(NameImage,Xval,Yval,Value)			\
		al_draw_tinted_bitmap(NameImage,al_map_rgba_f(Value,Value,Value,Value),Xval,Yval,0);
		
#define	PE1_AA_DRAW_TARGET_TO_IMAGE(Image_name)	al_set_target_bitmap(Image_name);

#define PE1_AA_DRAW_TARGET_TO_DISPLAY(Display_name)	al_set_target_backbuffer(Display_name);

#define	PE1_AA_DRAW_SEND_TO_DISPLAY()	al_flip_display();

#define	PE1_AA_AUDIO_VOLUME_CHANGE()						\
		al_set_mixer_gain(PE1_AUDIO_mixer,PE1_AUDIO_SONG_GAIN*PE1_AUDIO_VOLUME_MUTE);

#define	PE1_AA_AUDIO_SONG_PLAY(StreamName)							\
		al_attach_audio_stream_to_mixer(StreamName, PE1_AUDIO_mixer);	

#define	PE1_AA_AUDIO_SONG_LOOP_PLAY(StreamName)															\
		al_set_audio_stream_loop_secs(StreamName, 0.0, al_get_audio_stream_length_secs(StreamName));	\
		al_set_audio_stream_playmode(StreamName, ALLEGRO_PLAYMODE_LOOP);								\
		al_attach_audio_stream_to_mixer(StreamName, PE1_AUDIO_mixer);	
		
#define	PE1_AA_AUDIO_SONG_STOP(StreamName)					\
		al_detach_audio_stream(StreamName);
		
#define	PE1_AA_AUDIO_EFFECT_PLAY(InstanceName)								\
		al_play_sample(InstanceName, PE1_AUDIO_EFFECT_GAIN*PE1_AUDIO_VOLUME_MUTE, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		
#define	PE1_AA_AUDIO_EFFECT_STOP(InstanceName)								\
		al_stop_sample(InstanceName);

#define	PE1_AA_AUDIO_VOLUME_3MODE_DOWN()					\
		if(PE1_AUDIO_VOLUME_MODE==0){						\
			if(PE1_AUDIO_SONG_GAIN>0){						\
				PE1_AUDIO_SONG_GAIN-=0.1;					\
				PE1_AUDIO_EFFECT_GAIN=PE1_AUDIO_SONG_GAIN;	\
				PE1_AA_AUDIO_VOLUME_CHANGE();				\
			}												\
		}													\
		else if(PE1_AUDIO_VOLUME_MODE==1){ 					\
			if(PE1_AUDIO_SONG_GAIN>0){						\
				PE1_AUDIO_SONG_GAIN-=0.1;					\
				PE1_AA_AUDIO_VOLUME_CHANGE();				\
			}												\
		}													\
		else if(PE1_AUDIO_VOLUME_MODE==2){					\
			if(PE1_AUDIO_EFFECT_GAIN>0){					\
				PE1_AUDIO_EFFECT_GAIN-=0.1;					\
				PE1_AA_AUDIO_VOLUME_CHANGE();				\
			}												\
		}

#define	PE1_AA_AUDIO_VOLUME_3MODE_UP()						\
		if(PE1_AUDIO_VOLUME_MODE==0){						\
			if(PE1_AUDIO_SONG_GAIN<2.0){					\
				PE1_AUDIO_SONG_GAIN+=0.1;					\
				PE1_AUDIO_EFFECT_GAIN=PE1_AUDIO_SONG_GAIN;	\
				PE1_AA_AUDIO_VOLUME_CHANGE();				\
			}												\
		}													\
		else if(PE1_AUDIO_VOLUME_MODE==1){ 					\
			if(PE1_AUDIO_SONG_GAIN<2.0){					\
				PE1_AUDIO_SONG_GAIN+=0.1;					\
				PE1_AA_AUDIO_VOLUME_CHANGE();				\
			}												\
		}													\
		else if(PE1_AUDIO_VOLUME_MODE==2){					\
			if(PE1_AUDIO_EFFECT_GAIN<2.0){					\
				PE1_AUDIO_EFFECT_GAIN+=0.1;					\
				PE1_AA_AUDIO_VOLUME_CHANGE();				\
			}												\
		}

#define	PE1_AA_AUDIO_VOLUME_3MODE_CHANGE()		\
		PE1_AUDIO_VOLUME_MODE++;				\
		if(PE1_AUDIO_VOLUME_MODE>2){			\
			PE1_AUDIO_VOLUME_MODE=0;			\
		}
#define	PE1_AA_AUDIO_VOLUME_3MODE_MUTE()		\
		if(PE1_AUDIO_VOLUME_MUTE==1.0){			\
			PE1_AUDIO_VOLUME_MUTE=0.0;			\
		}										\
		else{									\
			PE1_AUDIO_VOLUME_MUTE=1.0;			\
		}										\
		PE1_AA_AUDIO_VOLUME_CHANGE();

///#define	PE1_AA_AUDIO_SYSTEM_PAUSE_INSTALL()			
	//The Idea would be to create a way to "watch" elapsed time of an audio stream being played.
	// 		Use PE1_Elapsed_Stream_Playtime
	//
	//	1. Stop incrementing
	//	2. Pushs playing sounds onto stack
	//		al_get_sample_instance_playing(const ALLEGRO_SAMPLE_INSTANCE *spl) -- Check relavent instances
	//		bool al_get_audio_stream_playing(const ALLEGRO_AUDIO_STREAM *stream) -- Cjeck relavent samples
	//	3. When unpaused: queue up music and resume...
	//
	//	Needs Still: _PAUSE(); _PAUSE_REMOVE(); Data Structure to hold "paused" songs.