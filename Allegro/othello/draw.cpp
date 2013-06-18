#include <stdio.h>
#include "draw.h"
#include "stones.h"
#include "sound.h"
#include "eval.h"
#include "globals.h"
#include "lucida18.h"
#include "graphics.h"

int clear_message = 0;
int timer_var = 0;
LOCK_VARIABLE(clear_message);
LOCK_VARIABLE(timer_var);

void timerfunc()
{
    if (timer_var > 0)
    {
	timer_var--;
    }

    if (timer_var == 1)
    {
	clear_message = 1;
    }

    poll_keyboard();
    if (key[KEY_ESC])
    {
	abort_eval = 1;
    }
}
LOCK_FUNCTION(timerfunc);

Draw::Draw()
{
    board = 0;
    scratch = 0;
    sscratch = 0;
    msgscratch = 0;
    
    for (int i = PLAYER_1_STONE; i <= PLAYER_2_STONE; i++)
    {
	stones[i] = 0;
	shadow[i] = 0;
    }
    
    sounddat = 0;
    stonedat = 0;
    lucida18 = 0;
    smp_beep = 0;
}

Draw::~Draw()
{
    // don't call destroy_graphics from here, doesn't work nice with
    // allegro exit handlers since this object is global
}

void Draw::draw_background2(BITMAP *bmp)
{
    // simply blit the image from the datafile
    blit((BITMAP *)(graphicsdat[BOARD_BMP].dat), bmp,
	 0, 0, 0, 0, BOARD_SIZE, BOARD_SIZE);
}

// expect a 600x600 bitmap to draw the board onto.
// correction: expect a 2*MARGIN + 8*FIELD_SIZE square board.
// this is to make it a little more scalable, in case I decide to change
// the stone size again. Ugh.
void Draw::draw_background(BITMAP *bmp)
{
    int
	wood = makecol(180, 120, 45),
	light_wood_1 = makecol(195, 135, 55),
	light_wood_2 = makecol(185, 125, 45),
	dark_wood_1 = makecol(165, 105, 45),
	dark_wood_2 = makecol(155, 95, 35),
	dark_line = makecol(80,60,5),
	light_line = makecol(100,80,10);

    // paint the wood
    clear_to_color(bmp, wood);

    // paint the edges
    rectfill(bmp, 0, BOARD_EDGE, BOARD_EDGE - 1, BOARD_SIZE - BOARD_EDGE - 1, light_wood_1);
    rectfill(bmp, BOARD_EDGE, 0, BOARD_SIZE - BOARD_EDGE - 1, BOARD_EDGE - 1, light_wood_2);
    rectfill(bmp, BOARD_SIZE - BOARD_EDGE, BOARD_EDGE, BOARD_SIZE - 1, BOARD_SIZE - BOARD_EDGE - 1, dark_wood_2);
    rectfill(bmp, BOARD_EDGE, BOARD_SIZE - BOARD_EDGE, BOARD_SIZE - BOARD_EDGE - 1, BOARD_SIZE - 1, dark_wood_1);

    // make diagonal corners for cool 3d effects :-) seems to be a bug in
    // triangle that takes the bottom y coordinate 1 too low (ie stop
    // drawing too high) so use BOARD_EDGE instead of BOARD_EDGE - 1.
    
    triangle(bmp, 0, 0, 0, BOARD_EDGE, BOARD_EDGE - 1, BOARD_EDGE, light_wood_1);
    triangle(bmp, 0, BOARD_SIZE - BOARD_EDGE, 0, BOARD_SIZE, BOARD_EDGE - 1, BOARD_SIZE - BOARD_EDGE, light_wood_1);
    triangle(bmp, 0, 0, BOARD_EDGE - 1, BOARD_EDGE, BOARD_EDGE - 1, 0, light_wood_2);
    triangle(bmp, BOARD_SIZE - BOARD_EDGE, 0, BOARD_SIZE - 1, 0, BOARD_SIZE - BOARD_EDGE, BOARD_EDGE, light_wood_2);
    triangle(bmp, BOARD_SIZE - 1, 0, BOARD_SIZE - 1, BOARD_EDGE, BOARD_SIZE - BOARD_EDGE, BOARD_EDGE, dark_wood_2);
    triangle(bmp, BOARD_SIZE - BOARD_EDGE, BOARD_SIZE - BOARD_EDGE, BOARD_SIZE - 1, BOARD_SIZE - BOARD_EDGE, BOARD_SIZE - 1, BOARD_SIZE, dark_wood_2);
    triangle(bmp, 0, BOARD_SIZE, BOARD_EDGE - 1, BOARD_SIZE, BOARD_EDGE - 1, BOARD_SIZE - BOARD_EDGE, dark_wood_1);
    triangle(bmp, BOARD_SIZE - BOARD_EDGE, BOARD_SIZE - BOARD_EDGE, BOARD_SIZE - BOARD_EDGE, BOARD_SIZE, BOARD_SIZE - 1, BOARD_SIZE, dark_wood_1);
   
    
    // paint the lines
    
    for (int i = 0; i < 8; i++)
    {
	// draw horizontal line
	rectfill(bmp,
		 BOARD_MARGIN + BOARD_LINE_MARGIN,
		 BOARD_MARGIN + FIELD_SIZE/2 + FIELD_SIZE * i - 1,
		 BOARD_SIZE - BOARD_MARGIN - BOARD_LINE_MARGIN - 1,
		 BOARD_MARGIN + FIELD_SIZE/2 + FIELD_SIZE * i,
		 dark_line);
	rectfill(bmp,
		 BOARD_MARGIN + BOARD_LINE_MARGIN,
		 BOARD_MARGIN + FIELD_SIZE/2 + FIELD_SIZE * i+1,
		 BOARD_SIZE - BOARD_MARGIN - BOARD_LINE_MARGIN - 1,
		 BOARD_MARGIN + FIELD_SIZE/2 + FIELD_SIZE * i+2,
		 light_line);

	// draw vertical line
	rectfill(bmp,
		 BOARD_MARGIN + FIELD_SIZE/2 + FIELD_SIZE * i - 1,
		 BOARD_MARGIN + BOARD_LINE_MARGIN,
		 BOARD_MARGIN + FIELD_SIZE/2 + FIELD_SIZE * i,
		 BOARD_SIZE - BOARD_MARGIN - BOARD_LINE_MARGIN - 1,
		 dark_line);
	rectfill(bmp,
		 BOARD_MARGIN + FIELD_SIZE/2 + FIELD_SIZE * i+1,
		 BOARD_MARGIN + BOARD_LINE_MARGIN,
		 BOARD_MARGIN + FIELD_SIZE/2 + FIELD_SIZE * i+2,
		 BOARD_SIZE - BOARD_MARGIN - BOARD_LINE_MARGIN - 1,
		 light_line);
    }
}


// set a graphics mode and allocate the necessary bitmaps
int Draw::init_graphics()
{
//    set_color_depth(16);
    set_color_depth(32);
    if (set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) < 0)
    {
	allegro_message("Could not set 800x600x24 graphics mode.\n");
	return 0;
    }

    // show the mouse
    show_mouse(screen);
    
    // load font
    lucida18 = load_datafile("lucida18.dat");
    if (!lucida18)
    {
	allegro_message("Could not load font 'lucida18.dat'\n");
	return 0;
    }
    font = (FONT *)(lucida18[LUCIDA18].dat);

    // load the board bitmap from datafile
    graphicsdat = load_datafile("graphics.dat");
    if (!graphicsdat)
    {
	allegro_message("Could not load datafile 'graphics.dat'.\n");
	return 0;
    }
    
    // board bitmap
    board = create_bitmap(BOARD_SIZE, BOARD_SIZE);
    draw_background2(board);

    // scratch bitmap
    scratch = create_bitmap(800, 600);

    // small scratch bitmaps
    sscratch = create_bitmap(FIELD_SIZE, FIELD_SIZE);

    // text area
    msgscratch = create_bitmap(BOARD_SIZE, 600 - BOARD_Y - BOARD_SIZE);
    
    // this does not belong here, but oh well.
    // sound samples (from datafile)
    sounddat = load_datafile("sound.dat");
    if (!sounddat)
    {
	allegro_message("Could not load datafile 'sound.dat'.\n");
	return 0;
    }
    smp_beep = (SAMPLE *)(sounddat[BEEP].dat);

    // these are 32 bit bitmaps, and I want to keep it that way
    set_color_conversion(COLORCONV_NONE);
    
    // stone bitmaps (from datafile)
    stonedat = load_datafile("stones.dat");
    if (!stonedat)
    {
	allegro_message("Could not load datafile 'stones.dat'.\n");
	return 0;
    }
    for (int i = PLAYER_1_STONE; i <= LAST_STONE; i++)
	stones[i] = (BITMAP *)(stonedat[i].dat);

    // pause and other command buttons
    pause_bmp = (BITMAP *)(stonedat[STONE_PAUSE].dat);
    play_bmp = (BITMAP *)(stonedat[STONE_PLAY].dat);
    undo_bmp = (BITMAP *)(stonedat[STONE_UNDO].dat);
    redo_bmp = (BITMAP *)(stonedat[STONE_REDO].dat);
    hint_bmp = (BITMAP *)(stonedat[STONE_HINT].dat);
    green_bmp = (BITMAP *)(stonedat[STONE_GREEN].dat);
    
    // make shadow bitmaps
    for (int b = PLAYER_1_STONE; b <= LAST_STONE; b++)
    {
	// allocate
	shadow[b] = create_bitmap_ex(32, stones[b]->w, stones[b]->h);

	// make it black if it is colored
	// keep it transparent where it is
	for (int x = 0; x < shadow[b]->w; x++)
	{
	    for (int y = 0; y < shadow[b]->h; y++)
	    {
		// we changed to alpha bitmaps.  shadow bitmap is generated
		// by looking at the alpha channel of the original: 0 for
		// background and 255 for stone. If 255, give shadow 70
		// alpha, if 0 give shadow 0 alpha.
		// the color of the shadow is black.

		int c = getpixel(stones[b], x, y);
		int a = makeacol_depth(32, 0, 0, 0, 70*geta32(c)/255);
		
		_putpixel32(shadow[b], x, y, a);
	    }
	}

//	char buf[80];
//	sprintf(buf, "bitmaps/shadow-%d.tga", b);
//	save_tga(buf, shadow[b], 0);
//	sprintf(buf, "bitmaps/stone-%d.tga", b);
//	save_tga(buf, stones[b], 0);
    }

    // restore color conversion
    set_color_conversion(COLORCONV_TOTAL);
    
    // set the transparency for draw_trans_sprite (shadow drawing)
    // set_trans_blender(0, 0, 0, 70);

    // set the alpha blender now that we are using alpha sprites
    set_alpha_blender();

    // set text mode (deprecated, for support of 4.0 series)
    text_mode(-1);

    // install a timer for clearing the message field (100 Hz)
    install_int(timerfunc, 10);
    
    return 1;
}
	  

// delete bitmaps that were allocated
void Draw::shutdown_graphics()
{
    destroy_bitmap(board);
    board = 0;

    destroy_bitmap(scratch);
    scratch = 0;

    destroy_bitmap(sscratch);
    sscratch = 0;

    destroy_bitmap(msgscratch);
    msgscratch = 0;
    
    for (int i = PLAYER_1_STONE; i <= LAST_STONE; i++)
    {
	destroy_bitmap(shadow[i]);
	shadow[i] = 0;
	stones[i] = 0;
    }

    undo_bmp = 0;
    redo_bmp = 0;
    play_bmp = 0;
    pause_bmp = 0;
    hint_bmp = 0;
    green_bmp = 0;
    
    unload_datafile(stonedat);
    stonedat = 0;

    font = 0;
    unload_datafile(lucida18);
    lucida18 = 0;
}

// draw the screen on the scratch bitmap, then blit it to the screen.
void Draw::draw_screen(Board const &b)
{
    int
	s1, s2;

    // get scores from the board
    b.count_stones(s1, s2);
    
    // clear scratch bitmap
    clear_to_color(scratch, makecol(0, 0, 0));

    // copy the board
    blit(board, scratch, 0, 0, BOARD_X, BOARD_Y, BOARD_SIZE, BOARD_SIZE);

    // draw the stones
    for (int j = 0; j < 8; j++)
    {
	for (int i = 0; i < 8; i++)
	{
	    // use a 600x600 rectangle in the center of the screen
	    // 64x64 bitmaps on a 70x70 square: 3 margin on both sides
	    //
	    
	    BITMAP
		*stone = 0,
		*shade = 0;
	    
	    if (b.getstone(j*8+i) == 1)
	    {
		stone = stones[PLAYER_1_STONE];
		shade = shadow[PLAYER_1_STONE];
	    }
	    if (b.getstone(j*8+i) == 2)
	    {
		stone = stones[PLAYER_2_STONE];
		shade = shadow[PLAYER_2_STONE];
	    }
	    if (hint == 3 && j*8+i == hintpos)
	    {
		stone = green_bmp;
		shade = shadow[PLAYER_1_STONE];
	    }
	    
	    // set a trans blender with low alpha value, and start with
	    // blitting the shadow bitmaps with a slight offset to the
	    // lower right. For sprite blitting, the color values of
	    // set_trans_blender are ignored, only the alpha channel is
	    // used to determine opacity. draw_trans_sprite uses different
	    // argument order than blit!

	    // we now use the alpha blender with 32 bpp sprites. This
	    // should work the same way with drawing, except our normal
	    // stone image is now also 32 bpp, so use draw_trans_sprite
	    // there too.
	    
	    if (stone && shade)
	    {
		draw_trans_sprite(scratch, shade,
				  BOARD_X + BOARD_MARGIN + STONE_MARGIN +
				  SHADOW_X + FIELD_SIZE*i,
				  BOARD_Y + BOARD_MARGIN + STONE_MARGIN +
				  SHADOW_Y + FIELD_SIZE * j);
		
		draw_trans_sprite(scratch, stone,
				  BOARD_X + BOARD_MARGIN + STONE_MARGIN +
				  FIELD_SIZE*i,
				  BOARD_Y + BOARD_MARGIN + STONE_MARGIN +
				  FIELD_SIZE * j);
	    }
	}
    }

    // the side screen

    // plot the stone showing who's turn it is
    if (who == 1)
	draw_trans_sprite(scratch, stones[PLAYER_1_STONE],
			  BOARD_X / 2 - STONE_SIZE / 2, 90);
    else
	draw_trans_sprite(scratch, stones[PLAYER_2_STONE],
			  BOARD_X / 2 - STONE_SIZE / 2, 90);

    // if it's a computer player, show some dots in it
    if (computer_player[who - 1] || hint == 2)
	textprintf_centre(scratch, font, BOARD_X / 2,
			  90 + STONE_SIZE / 2 - text_height(font) + 3,
			  makecol(0,0,0), "....");
    
    // plot 2 stones at the bottom with the scores in them
    draw_trans_sprite(scratch, stones[PLAYER_1_STONE],
		      BOARD_X / 2 - STONE_SIZE / 2, 400);
    draw_trans_sprite(scratch, stones[PLAYER_2_STONE],
		      BOARD_X / 2 - STONE_SIZE / 2, 480);
    
    textprintf_centre(scratch, font, BOARD_X / 2,
			 400 + STONE_SIZE / 2 - text_height(font)/2,
			 makecol(0,0,0), "%d", s1);
    textprintf_centre(scratch, font, BOARD_X / 2,
			 480 + STONE_SIZE / 2 - text_height(font)/2,
			 makecol(0,0,0), "%d", s2);

    int margin = (V_BUTTON_SPACE-STONE_SIZE)/2;
    
    // plot 4 command buttons on the right side
    if (paused)
	draw_trans_sprite(scratch, play_bmp,
			  3 * BOARD_X / 2 + BOARD_SIZE - STONE_SIZE / 2,
			  BOARD_Y + BOARD_SIZE/2 - 2*V_BUTTON_SPACE + margin);
    else
	draw_trans_sprite(scratch, pause_bmp,
			  3 * BOARD_X / 2 + BOARD_SIZE - STONE_SIZE / 2,
			  BOARD_Y + BOARD_SIZE/2 - 2*V_BUTTON_SPACE + margin);

    draw_trans_sprite(scratch, undo_bmp,
		      3 * BOARD_X / 2 + BOARD_SIZE - STONE_SIZE / 2,
		      BOARD_Y + BOARD_SIZE/2 - V_BUTTON_SPACE + margin);
    draw_trans_sprite(scratch, redo_bmp,
		      3 * BOARD_X / 2 + BOARD_SIZE - STONE_SIZE / 2,
		      BOARD_Y + BOARD_SIZE/2 + margin);
    draw_trans_sprite(scratch, hint_bmp,
		      3 * BOARD_X / 2 + BOARD_SIZE - STONE_SIZE / 2,
		      BOARD_Y + BOARD_SIZE/2 + V_BUTTON_SPACE + margin);
    
    // the text area
    blit(msgscratch, scratch, 0, 0, BOARD_X, BOARD_Y + BOARD_SIZE,
	 msgscratch->w, msgscratch->h);
    
    // blit the scratch bitmap to the screen
    scare_mouse();
    blit(scratch, screen, 0, 0, 0, 0, 800, 600);
    unscare_mouse();
}

void Draw::clear_message()
{
    // if the message timeout is up, clear the message area.
    clear_to_color(msgscratch, makecol(0, 0, 0));
    ::clear_message = 0;
}

void Draw::pancake(Board const &b)
{
    if (!animations)
	return;

    // b is the new board. compute differences using lastboard.
    uint64_t diff = b.diff(lastboard);
    uint64_t flipped = b.flipped(lastboard);
    
    if (!diff)
	return;

    // hide the mouse
    scare_mouse();
    
    // loop over 21 timesteps.
    for (int t = PLAYER_1_STONE; t <= PLAYER_2_STONE; t++)
    {
	// loop over all squares
	for (int p = 0; p < 64; p++)
	{
	    int
		w,
		bmp,
		newstone = 0;
	    
	    int
		i = p % 8,
		j = p / 8;
	    
            // no change? continue
	    if (!get(diff, p))
		continue;
	    
	    // differentiate between new and flipped stones
	    if (!get(flipped, p))
	    {
		newstone = 1;
	    }
		
	    // get the new stone
	    w = b.getstone(p);
	    ASSERT(w);

	    if (newstone)
	    {
		if (w == 1)
		    // we go from 2 to 1
		    bmp = PLAYER_1_STONE;
		else
		    // we go from 1 to 2
		    bmp = PLAYER_2_STONE;
	    }
	    else
	    {
		// now that we have 41 bitmaps, we can always add t to the
		// starting bitmap.
		
		if (w == 1)
		    // we go from 2 to 1
		    // this used to be - t
		    bmp = PLAYER_2_STONE + t;
		else
		    // we go from 1 to 2
		    bmp = PLAYER_1_STONE + t;
	    }

	    // new stones only have to be drawn once
	    if (!(t && newstone))
	    {
		// restore background
		blit(board, sscratch,
		     BOARD_MARGIN + FIELD_SIZE * i,
		     BOARD_MARGIN + FIELD_SIZE * j,
		     0, 0, FIELD_SIZE, FIELD_SIZE);
		
		// blit shadow and stone
		draw_trans_sprite(sscratch, shadow[bmp],
				  STONE_MARGIN + SHADOW_X,
				  STONE_MARGIN + SHADOW_Y);
		draw_trans_sprite(sscratch, stones[bmp],
				  STONE_MARGIN, STONE_MARGIN);
		
		// blit the scratch bitmap to screen
		blit(sscratch, screen,
		     0, 0,
		     BOARD_X + BOARD_MARGIN + FIELD_SIZE * i,
		     BOARD_Y + BOARD_MARGIN + FIELD_SIZE * j,
		     FIELD_SIZE, FIELD_SIZE);
	    }
	}

	// do the sideboard stone (who indicator)
	clear_to_color(sscratch, makecol(0,0,0));
	if (who == 1)    // move from 2 to 1
	{
	    // used to be - t
//	    masked_blit(stones[PLAYER_2_STONE + t], sscratch,
//			0, 0, 0, 0, 64, 64);
	    draw_trans_sprite(sscratch, stones[PLAYER_2_STONE + t], 0, 0);
	}
	else             // move from 1 to 2
	{
//	    masked_blit(stones[PLAYER_1_STONE + t], sscratch,
//			0, 0, 0, 0, 64, 64);
	    draw_trans_sprite(sscratch, stones[PLAYER_1_STONE + t], 0, 0);
	}
	blit(sscratch, screen, 0, 0, BOARD_X / 2 - STONE_SIZE / 2,
	     90, STONE_SIZE, STONE_SIZE);
	
	// pause a little while
	rest(20);
    }

    // show the mouse
    unscare_mouse();
}

void Draw::remember(Board const &b)
{
    lastboard = b;
}

// I'm too lazy to define a separate class for this now.
void Draw::beep()
{
    play_sample(smp_beep, 255, 128, 1000, 0);
}

void Draw::message(char const *fmt, ...)
{
    // allegro message and printf don't have v... versions, so
    // use vsprintf to print into a (rediculously big) string buffer,
    // and send that buffer to textout instead of textprintf.

    char too_big[10000];
    
    va_list args;
    va_start(args, fmt);

    // format into string buffer
    vsprintf(too_big, fmt, args);

//    vprintf(fmt, args);
//    allegro_message(fmt, args);

    va_end(args);
    
    clear_to_color(msgscratch, makecol(0,0,0));

    // print centered at the bottom of the screen
    textout_centre(msgscratch, font, too_big,
		   msgscratch->w/2,
		   (msgscratch->h - text_height(font))/2,
		   makecol(200, 200, 200));
    

    // blit the bitmap to screen
    blit(msgscratch, screen, 0, 0, BOARD_X, BOARD_Y + BOARD_SIZE,
	 msgscratch->w, msgscratch->h);

    // set a message timeout: if it expires the message will be erased.
    ::clear_message = 0;
    timer_var = 300;
}

void Draw::set_hint(int p)
{
    hintpos = p;
}
