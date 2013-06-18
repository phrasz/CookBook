#include <allegro.h>
#include <stdlib.h>
#include "file.h"
#include "log.h"
#include "globals.h"
#include "setup.h"
#include "kbd.h"

// setup.cc: provide a function that shows a dialog to change the game
// settings, both before and during the game.
// dialogs are the easiest (I think) if I use global variables, but I may
// come back on that later.

// setup dialog needs the following:
// - ply setting checkboxes
// - computer player checkboxes/toggle for player 1 and 2
// - toggle to determine which player starts
// - optional: name textfields for both players
// - 3 buttons:  OK,  resume  and  restart   (gray out resume/restart first)
// - optional: a save-game button or a separate dialog
// - optional: a load-game button or dialog

// I will attempt to make the dialog sized 600x400
// true colors have to be set afterwards, I'm afraid

DIALOG options_dialog[] =
{
    // (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)
    // (d1)         (d2)      (dp)                      (dp2) (dp3)
    {  d_clear_proc,     0,    0,    0,    0,    1,    0,    0,    0,
       0,           0,        0,                        0,    0 },
    
    {  d_shadow_box_proc, 100, 50,   600,  500,  1,    2,    0,    0,
       0,           0,        0,                       0,    0 },
    
    {  d_radio_proc,     450,  150,   100,  20,   1,    2,    '1',  0,
       1,           0,        (void *)"ply 1",          0,    0 },

    {  d_radio_proc,     450,  190,  100,  20,   1,    2,    '2',  0,
       1,           0,        (void *)"ply 2",          0,    0 },

    {  d_radio_proc,     450,  230,  100,  20,   1,    2,    '3',  0,
       1,           0,        (void *)"ply 3",          0,    0 },

    {  d_radio_proc,     450,  270,  100,  20,   1,    2,    '4',  D_SELECTED,
       1,           0,        (void *)"ply 4",          0,    0 },

    {  d_radio_proc,     450,  310,  100,  20,   1,    2,    '6',  0,
       1,           0,        (void *)"ply 6",          0,    0 },

    {  d_radio_proc,     450,  350,  100,  20,   1,    2,    '8',  0,
       1,           0,        (void *)"ply 8",          0,    0 },

    {  d_text_proc,      450,  100,  150,  20,   1,    2,    0,    0,
       0,           0,        (void *)"Difficulty",     0,    0 },
    
    {  d_button_proc,    450,  450,  100,  50,   1,    0,    0,    D_EXIT | D_GOTFOCUS | D_GOTMOUSE,
       0,           0,        (void *)"Ok",          0,    0 },

    {  d_button_proc,    250,  450,  100,  50,   1,    0,    0,    D_EXIT,
       0,           0,        (void *)"Cancel",          0,    0 },

    {  d_text_proc,      250,  100,  150,  20,   1,    2,    0,    0,
       0,           0,        (void *)"Player 1",     0,    0 },

    {  d_radio_proc,     250,  150,  100,  20,   1,    2,    0,    D_SELECTED,
       2,           0,        (void *)"Human",          0,    0 },

    {  d_radio_proc,     250,  190,  100,  20,   1,    2,    0,    0,
       2,           0,        (void *)"Computer",       0,    0 },

    {  d_text_proc,      250,  260,  150,  20,   1,    2,    0,    0,
       0,           0,        (void *)"Player 2",     0,    0 },

    {  d_radio_proc,     250,  310,  100,  20,   1,    2,    0,    0,
       3,           0,        (void *)"Human",          0,    0 },

    {  d_radio_proc,     250,  350,  100,  20,   1,    2,    0,    D_SELECTED,
       3,           0,        (void *)"Computer",       0,    0 },
    
    {  0,           0,    0,     0,    0,    0,    0,    0,    0,
       0,           0,        0,                        0,    0 }
};


#define FIRST_RADIO_PLY 2
#define LAST_RADIO_PLY  7
#define FIRST_RADIO_PL1 12
#define FIRST_RADIO_PL2 15
#define SHADOW_BOX      1
#define OK_BUTTON       9
#define CANCEL_BUTTON   10

int do_options_dialog()
{
    int
	i,
	ret;

    static bool init_colors = true;
    
    // initialize dialog

    int
	ply_settings[6] = {1, 2, 3, 4, 6, 8};
    
    // colors

    if (init_colors)
    {
	int
	    colors[3];
	
	colors[0] = makecol(0,0,0);
	colors[1] = makecol(200, 200, 200);
	colors[2] = makecol(50, 50, 50);
    	
	for (i = 0; options_dialog[i].proc; i++)
	{
	    options_dialog[i].fg = colors[options_dialog[i].fg];
	    options_dialog[i].bg = colors[options_dialog[i].bg];
	}

	init_colors = false;
    }

    // initialize dialog from global variables
    for (i = FIRST_RADIO_PLY; i <= LAST_RADIO_PLY; i++)
    {
	if (ply == ply_settings[i - FIRST_RADIO_PLY])
	{
	    options_dialog[i].flags |= D_SELECTED;
	}
	else
	    options_dialog[i].flags &= ~D_SELECTED;
    }

    if (computer_player[0])
    {
	options_dialog[FIRST_RADIO_PL1].flags &= ~D_SELECTED;
	options_dialog[FIRST_RADIO_PL1 + 1].flags |= D_SELECTED;
    }
    else
    {
	options_dialog[FIRST_RADIO_PL1].flags |= D_SELECTED;
	options_dialog[FIRST_RADIO_PL1 + 1].flags &= ~D_SELECTED;
    }
    
    if (computer_player[1])
    {
	options_dialog[FIRST_RADIO_PL2].flags &= ~D_SELECTED;
	options_dialog[FIRST_RADIO_PL2 + 1].flags |= D_SELECTED;
    }
    else
    {
	options_dialog[FIRST_RADIO_PL2].flags |= D_SELECTED;
	options_dialog[FIRST_RADIO_PL2 + 1].flags &= ~D_SELECTED;
    }
    
    // if any keys pressed, read it out
    clear_kbd(KEY_ESC);
    ret = do_dialog(options_dialog, -1);
    clear_kbd(KEY_ESC);

    // check out if the ok or cancel button was clicked
    if (ret == OK_BUTTON)
    {
	// set all options
	
	// find out which radio button was selected
	for (i = FIRST_RADIO_PLY; i <= LAST_RADIO_PLY ; i++)
	{
	    if (options_dialog[i].flags & D_SELECTED)
	    {
		// set ply setting,
		ply = ply_settings[i - FIRST_RADIO_PLY];
//		allegro_message("Difficulty set to ply %d\n", ply);
		break;
	    }
	}

	// find out player/computer settings
	computer_player[0] =
	    !(options_dialog[FIRST_RADIO_PL1].flags & D_SELECTED);
    
	computer_player[1] =
	    !(options_dialog[FIRST_RADIO_PL2].flags & D_SELECTED);

	// save settings
	file.save_settings();
    }

    // if we leave this dialog, go back to the game dialog.
    // this can be recursive, but hopefully that will be ok.
    return do_game_dialog();
}


char edit_buf[1024];

DIALOG game_dialog[] =
{
    // (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)
    // (d1)         (d2)      (dp)                      (dp2) (dp3)
    {  d_clear_proc,     0,    0,    0,    0,    1,    0,    0,    0,
       0,           0,        0,                        0,    0 },
    
    {  d_shadow_box_proc, 200, 50,   400,  500,  1,    2,    0,    0,
       0,           0,        0,                       0,    0 },
    
    {  d_button_proc,    300,  100,  200,  50,   1,    0,    0,    D_EXIT,
       0,           0,        (void *)"New Game",          0,    0 },

    {  d_button_proc,    300,  187,  200,  50,   1,    0,    0,    D_EXIT | D_DISABLED,
       0,           0,        (void *)"Resume Game",          0,    0 },

    {  d_button_proc,    300,  362,  200,  50,   1,    0,    0,    D_EXIT,
       0,           0,        (void *)"Options",          0,    0 },

    {  d_button_proc,    300,  450,  200,  50,   1,    0,    0,    D_EXIT,
       0,           0,        (void *)"Quit",          0,    0 },

    {  d_button_proc,    300,  275,   95,  50,   1,    0,    0,    D_EXIT,
       0,           0,        (void *)"Save",          0,    0 },

    {  d_button_proc,    405,  275,   95,  50,   1,    0,    0,    D_EXIT,
       0,           0,        (void *)"Load",          0,    0 },

    {  0,           0,    0,     0,    0,    0,    0,    0,    0,
       0,           0,        0,                        0,    0 }
};

#define GAME_NEW      2
#define GAME_RESUME   GAME_NEW + 1
#define GAME_OPTIONS  GAME_NEW + 2
#define GAME_QUIT     GAME_NEW + 3
#define GAME_SAVE     GAME_NEW + 4
#define GAME_LOAD     GAME_NEW + 5

int do_game_dialog()
{
    int
	i,
	ret;

    static bool first_time   = true;
    static bool init_colors  = true;
    
    // initialize dialog

//    allegro_message("do_game_dialog: %d %d\n", first_time, init_colors);
    
    // colors
    if (init_colors)
    {
	int
	    colors[3];

	colors[0] = makecol(0,0,0);
	colors[1] = makecol(200, 200, 200);
	colors[2] = makecol(50, 50, 50);
	
	for (i = 0; game_dialog[i].proc; i++)
	{
	    game_dialog[i].fg = colors[game_dialog[i].fg];
	    game_dialog[i].bg = colors[game_dialog[i].bg];
	}
	init_colors = false;

	// also initialize the colors of the 'standard' allegro dialogs
	gui_fg_color = colors[1];
	gui_bg_color = colors[0];
    }

    // if any keys pressed, read it out
    clear_kbd(KEY_ESC);
    ret = do_dialog(game_dialog, -1);
    clear_kbd(KEY_ESC);

    // if user exited by escape, determine what to do.
    if (ret < 0)
    {
	if (game_dialog[GAME_RESUME].flags & D_DISABLED)
	    ret = GAME_QUIT;
	else
	    ret = GAME_RESUME;
    }
    
//    allegro_message("end game_dialog, return value %d\n", ret);
    
    // reset quit flag, used to request this menu
    quit = 0;
    
    // check out if the resume game button was clicked
    if (ret == GAME_RESUME)
    {
	return ret;
    }

    // check out if the new game button was clicked
    if (ret == GAME_NEW)
    {
	// first time? enable resume game button
	if (first_time)
	{
	    game_dialog[GAME_RESUME].flags ^= D_DISABLED;
	    first_time = false;   // we just started our first game
	}
    
	aap.init();           // reset the game board
	draw.remember(aap);   // don't flip stones upon first draw
	logfile.init();           // reset the log
	
	// load settings
	// they are saved each time you leave the options menu,
	// so it should be safe to load them for each new game too.
	file.load_settings();

	who = who_starts;
	win_message = 0;
	
	// leave other options as they are
	
	return ret;
    }

    // check out if the resume game button was clicked
    if (ret == GAME_OPTIONS)
    {
	// this is going to be a very scary recursive dialog thing
	// but hopefully it will simply work :-) No guts, no glory.

	clear_keybuf();
	return do_options_dialog();
    }

    // quit button
    if (ret == GAME_QUIT)
    {
	// shutdown graphics
	draw.shutdown_graphics();
	
        // end game, the rough way.
	// correction. The rough way gives problems with destructors of
	// string in the File object, apparently. Try the 'nice way'
	// instead.

	exit_program = 1;
	return ret;
    }

    if (ret == GAME_SAVE)
    {
	// -1 = cancel, 0 = error
	int sval = file.save(who, &aap, &logfile);
	
	if (!sval)
	{
	    allegro_message("An error occurred while saving the game.\n");
	}

	if (sval <= 0)
	{
	    // error or cancel
	    return do_game_dialog();
	}
	
	return ret;
    }

    if (ret == GAME_LOAD)
    {
	int lval = file.load(who, &aap, &logfile);

	if (!lval)
	{
	    allegro_message("An error occurred while loading the game.\n");
	}

	if (lval <= 0)
	{
	    // cancel or error
	    return do_game_dialog();
	}

	// success
	draw.remember(aap);

	// first time? enable resume game button
	if (first_time)
	{
	    game_dialog[GAME_RESUME].flags ^= D_DISABLED;
	    first_time = false;   // we just started our first game
	}
    }
    
    return ret;
}
