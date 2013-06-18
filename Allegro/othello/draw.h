#ifndef _DRAW_H_
#define _DRAW_H_

#include <stdarg.h>
#include <allegro.h>
#include "board.h"

class Draw
{
public:
    enum player_stones
    {
	PLAYER_1_STONE = 0,
	PLAYER_2_STONE = 20,
	LAST_STONE = 40,
	FIELD_SIZE = 65,
	STONE_SIZE = 56,
	BOARD_SIZE = 560,
	BOARD_MARGIN = 20,
	BOARD_EDGE = 10,
	BOARD_X = (800 - BOARD_SIZE) / 2,
	BOARD_Y = 4,
	BOARD_LINE_MARGIN = 20,
	STONE_MARGIN = 4,
	SHADOW_X = 4,
	SHADOW_Y = 4,
	V_BUTTON_SPACE = 80
    };

    Draw();
    ~Draw();
    
    int  init_graphics();
    void shutdown_graphics();
    void draw_background(BITMAP *bmp);
    void draw_background2(BITMAP *bmp);
    void draw_screen(Board const &b);
    void pancake(Board const &b);
    void remember(Board const &b);
    void beep();
    void message(char const *fmt, ...);
    void clear_message();
    void set_hint(int p);
    
private:
    SAMPLE *smp_beep;
    BITMAP *board;
    BITMAP *scratch;
    BITMAP *sscratch;
    BITMAP *stones[41];
    BITMAP *shadow[41];
    BITMAP *msgscratch;
    BITMAP *undo_bmp;
    BITMAP *redo_bmp;
    BITMAP *play_bmp;
    BITMAP *pause_bmp;
    BITMAP *hint_bmp;
    BITMAP *green_bmp;
    DATAFILE *sounddat;
    DATAFILE *stonedat;
    DATAFILE *lucida18;
    DATAFILE *graphicsdat;
    Board lastboard;
    int hintpos;
};
    
#endif
