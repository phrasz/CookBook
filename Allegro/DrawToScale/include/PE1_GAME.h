//GAME Entries
#define	PE1_GAME_SETUP_REVERSI_VARIABLES()			\
	int 	Reversi_Redraw_Board =0;				\
	int		Reversi_Next_Move_Found=0;				\
	int 	Reversi_Paths_Valid=0;					\
	int		Reversi_Paths_to_Check;					\
	int		Reversi_Paths_Loop_Counter;				\
	char	ReversiPaths[8][2];						\
	int		ReversiNewPlayerColor;					\
	int		ReversiNewPlayerLocation;				\
	char	ReversiScore[4][1];						\
	int		ReversiCurrentMove=0;					\
	int		ReversiLastLocation=0;					\
	int		ReversiCurrentPlayer=0;					\
	struct PE1_PlayerData	ReversiPlayers[4];
//at 4 for scores to allow up to 4 player

#define	PE1_GAME_STRUCT_PLAYER_ReversiPlayers_START()	\
		ReversiPlayers[0].location=0;					\
		ReversiPlayers[0].last_frame_shown=0;			\
		ReversiPlayers[0].state=0;						\
		ReversiPlayers[0].AIControlled=0;				\
														\
		ReversiPlayers[1].location=0;					\
		ReversiPlayers[1].last_frame_shown=0;			\
		ReversiPlayers[1].state=1;						\
		ReversiPlayers[1].AIControlled=0;				\
														\
		ReversiPlayers[2].location=0;					\
		ReversiPlayers[2].last_frame_shown=0;			\
		ReversiPlayers[2].state=2;						\
		ReversiPlayers[2].AIControlled=0;				\
														\
		ReversiPlayers[3].location=0;					\
		ReversiPlayers[3].last_frame_shown=0;			\
		ReversiPlayers[3].state=3;						\
		ReversiPlayers[3].AIControlled=0;				

#define	PE1_GAME_STRUCT_PLAYER_ReversiPlayers_DEBUG()													\
		for(i=0;i<4;i++){																				\
			printf("\n\n[CONSOLE] Player %i location: %i",i,ReversiPlayers[i].location);				\
			printf("\n[CONSOLE] Player %i last frame shown: %i",i,ReversiPlayers[i].last_frame_shown);	\
			printf("\n[CONSOLE] Player %i state: %i",i,ReversiPlayers[i].state);						\
			printf("\n[CONSOLE] Player %i AIControlled: %i",i,ReversiPlayers[i].AIControlled);			\
		}
		
#define PE1_GAME_SETUP_2D_BOARD_REVERSI_START(PE1_2D_array,tile1_name,tile2_name,icon1_name,icon2_name)		\
	PE1_AA_DRAW_2D_BOARD_2_TILES(board_offset_x,board_offset_y,PE1_DATA_2D_gameboard_columns,PE1_DATA_2D_gameboard_rows,tile1_name,tile2_name,tile_w,tile_h); \
	PE1_AA_DRAW_2D_BOARD_TILE(icon1_name,3,3);	\
	PE1_AA_DRAW_2D_BOARD_TILE(icon1_name,4,4);	\
	PE1_AA_DRAW_2D_BOARD_TILE(icon2_name,3,4);	\
	PE1_AA_DRAW_2D_BOARD_TILE(icon2_name,4,3);	\
	PE1_2D_array[27][0]=0;						\
	PE1_2D_array[28][0]=1;						\
	PE1_2D_array[35][0]=1;						\
	PE1_2D_array[36][0]=0;						\
	ReversiCurrentMove=0;

#define PE1_GAME_REVERSI_MOVE_NEXT_RIGHT(PlayerLocation,PE1_2D_array)	\
	Reversi_Next_Move_Found=0;											\
	for(i=PlayerLocation;i<64;i++){										\
		if(PE1_2D_array[i][0]==-1){										\
			Reversi_Next_Move_Found=i;									\
		}																\
	}																	\
	if(!Reversi_Next_Move_Found){										\
		for(i=0;i<PlayerLocation;i++){									\
			if(PE1_2D_array[i][0]==-1){									\
				Reversi_Next_Move_Found=i;								\
			}															\
		}																\
	}																	\
	if(Reversi_Next_Move_Found!=0){PlayerLocation=i;}

	
	