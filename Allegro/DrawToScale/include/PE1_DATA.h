//Default Defines
#define PE1_DATA_IMAGE	ALLEGRO_BITMAP

#define	PE1_DATA_FONT	ALLEGRO_FONT

#define PE1_DATA_IMAGE_REMOVE(ImageName)	al_destroy_bitmap(ImageName);

#define PE1_DATA_LOAD_INSTALL()	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

//Default alpha: Magic Pink
#define PE1_DATA_LOAD_IMAGE_FROM_FILE_WITH_ALPHA(ImageName,filename)  		\
		membitmap = al_load_bitmap(filename);								\
		if (!membitmap) { 													\
			printf("\n[ERROR] filename Failed to Load!!!");					\
		} 																	\
		else																\
			al_convert_mask_to_alpha( membitmap, al_map_rgb(255,0,255) );	\
                                                                            \
		al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);						\
		ImageName =al_clone_bitmap(membitmap); 								\
		al_destroy_bitmap(membitmap); 										
//Depends on: ALLEGRO_BITMAP *membitmap; && al_init_image_addon(); && One call of: al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
//Notes: ADD the declaration of the bitmap aswell: ALLEGRO_BITMAP *<NAME>;
//Notes: ENSURE YOU DESTROY/CLEAN UP with: al_destroy_bitmap(bitmap);

//Default alpha: PNG Transparency
#define PE1_DATA_LOAD_IMAGE_FROM_FILE(ImageName,filename)  					\
		membitmap = al_load_bitmap(filename);								\
		if (!membitmap) { 													\
			printf("\n[ERROR] filename Failed to Load!!!");					\
		} 																	\
																			\
		al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);						\
		ImageName = al_create_bitmap(PE1_Display_width,PE1_Display_height);	\
																			\
		al_set_target_bitmap(ImageName);									\
		al_draw_scaled_bitmap(membitmap , 0, 0, PE1_Display_Default_width, PE1_Display_Default_height, 0, PE1_Aspect_Ratio_offset_y, PE1_Display_width, PE1_Display_Default_height*PE1_DISPLAY_SCALAR, 0);	\
										  \
		al_set_target_backbuffer(display);\
		al_destroy_bitmap(membitmap); 										
//Depends on: ALLEGRO_BITMAP *membitmap; && al_init_image_addon(); && One call of: al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
//Notes: ADD the declaration of the bitmap aswell: ALLEGRO_BITMAP *<NAME>;
//Notes: ENSURE YOU DESTROY/CLEAN UP with: al_destroy_bitmap(bitmap);

#define PE1_DATA_FONT_TTF_LOAD_FROM_FILE(fontName,fontSize,fontPath)	\
		fontName = al_load_ttf_font(fontPath, fontSize*PE1_DISPLAY_SCALAR, 0);				\
		if (!fontName){													\
			printf("\n[ERROR] FAILED TO LOAD FONT fontName!");			\
			return 1;													\
		}

#define	PE1_DATA_STRUCT_PLAYER_INSTALL()					\
		struct PE1_PlayerData{								\
		  int location;										\
		  int last_frame_shown;								\
		  int state;										\
		  char *name;										\
		  char *ip_address;									\
		  int color;										\
		  int DirectionFacing;								\
		  int AIControlled;									\
		};

/////////////////////////////////////VALID ^^-------------------
//BUG? ----v
#define	PE1_DATA_2D_ARRAY_INSTALL(PE1_2D_Array,PE1_2D_Array_rows,PE1_2D_Array_columns)	\
		int **PE1_2D_Array;																\
		int	PE1_2D_Array_rows;															\
		int	PE1_2D_Array_columns;					
		
#define	PE1_DATA_2D_ARRAY_START(PE1_2D_array,PE1_2D_Array_rows,nrows,PE1_2D_Array_columns,ncolumns,default_val)	\
		PE1_2D_Array_rows=nrows;											\
		PE1_2D_Array_columns=ncolumns;										\
		PE1_2D_array = malloc(PE1_2D_Array_rows * sizeof(int *));			\
		if(PE1_2D_array == NULL){											\
			printf("\n[ERROR] out of memory");								\
			exit;															\
		}																	\
		for(i = 0; i < PE1_2D_Array_rows; i++){								\
			PE1_2D_array[i] = malloc(PE1_2D_Array_columns * sizeof(int));	\
			if(PE1_2D_array[i] == NULL){									\
				printf("\n[ERROR] out of memory\n");						\
				exit;														\
			}																\
		}																	\
		for(i = 0; i < PE1_2D_Array_rows; i++){								\
			for(j = 0; j < PE1_2D_Array_columns; j++){						\
				PE1_2D_array[i][j] = -1;									\
			}																\
		}

#define	PE1_DATA_2D_ARRAY_DEBUGGING(PE1_2D_array)											\
		printf("\n\n[DEBUGGING] Updated Array with \"Default Number\" Values:");			\
		for(i = 0; i < PE1_2D_Array_rows; i++){												\
			for(j = 0; j < PE1_2D_Array_columns; j++){										\
				printf("\n[DEBUGGING] (PE1_2D_array)[%i][%i]: %i",i,j,PE1_2D_array[i][j]);	\
			}																				\
		}
		
#define	PE1_DATA_2D_ARRAY_REMOVE(PE1_2D_Array,PE1_2D_Array_rows,PE1_2D_Array_columns)				\
		for(i = 0; i < PE1_2D_Array_rows; i++){				\
			free(PE1_2D_Array[i]);							\
		}													\
		free(PE1_2D_Array);
//Depends: ON PE1_DATA_2D_ARRAY_START() -- Crashes program IF not previous called.

#define	PE1_DATA_AUDIO_SONG_INSTALL(StreamName)			\
		ALLEGRO_AUDIO_STREAM *StreamName;

#define	PE1_DATA_AUDIO_SONG_LOAD_FROM_FILE(StreamName,PathName)	\
		StreamName = al_load_audio_stream(PathName, 4, 2048);					
		
#define	PE1_DATA_AUDIO_SONG_REMOVE(StreamName)				\
		al_destroy_audio_stream(StreamName);

#define	PE1_DATA_AUDIO_EFFECT_INSTALL(InstanceName)			\
		ALLEGRO_SAMPLE			*InstanceName;				
		
#define	PE1_DATA_AUDIO_EFFECT_LOAD_FROM_FILE(InstanceName,PathName)	\
		InstanceName = al_load_sample(PathName);
		//Instead: 	InstanceName_data = al_load_sample(PathName);	
		
#define	PE1_DATA_AUDIO_EFFECT_REMOVE(InstanceName)				\
         al_destroy_sample(InstanceName);
		 
