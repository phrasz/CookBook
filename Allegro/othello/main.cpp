#include <stdio.h>
#include <allegro.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "file.h"
#include "board.h"
#include "eval.h"
#include "setup.h"
#include "globals.h"
#include "draw.h"
#include "log.h"
#include "kbd.h"

// Global variables
//
// a scratch array of boards for the computer to work with
//Board boards[MAX_PLY];
Board *boards;
Board aap;
int who_starts = 1;
int who = who_starts;
int ply = 4;
int computer_player[2] = {0, 1};        // default: computer is player 2
int quit = 0;
int redo = 0;
int undo = 0;
int exit_program = 0;                   // set this to exit the program
int win_message = 0;                    // '%d will win' message has been shown
int animations = 1;                     // animate pancakes or not
int paused = 0;                         // pause game for undo / redo
int hint = 0;
Draw draw;                              // display object
Log logfile;
File file;

void clear_kbd(int k)
{
    do
    {
	rest(1);
	poll_keyboard();
    }
    while (key[k]);

    clear_keybuf();
}

int get_command_button()
{
    // check the position of the mouse with respect to the command
    // buttons. These are located at the following positions:
    // x: BOARD_X + BOARD_SIZE + BOARD_X / 2 - STONE_SIZE / 2
    // y: BOARD_Y + BOARD_SIZE/2 + n * STONE_SIZE, n = -2, -0.5, 1

    // return 0, 1, 2 for pause, undo, redo, -1 for nothing

    int x_left = Draw::BOARD_X + Draw::BOARD_SIZE + Draw::BOARD_X/2 -
	Draw::STONE_SIZE/2;
    int y_middle = Draw::BOARD_Y + Draw::BOARD_SIZE/2;
    int margin = (Draw::V_BUTTON_SPACE - Draw::STONE_SIZE) / 2;
    
    
    // don't be too picky about hitting the button
    if (mouse_x < x_left || mouse_x > x_left + Draw::STONE_SIZE)
	return -1;

    if (mouse_y >= y_middle - 2 * Draw::V_BUTTON_SPACE + margin
	&& mouse_y <= y_middle - 2 * Draw::V_BUTTON_SPACE + margin + Draw::STONE_SIZE)
	return 0;   // pause/play

    if (mouse_y >= y_middle - Draw::V_BUTTON_SPACE + margin &&
	mouse_y <= y_middle - Draw::V_BUTTON_SPACE + margin + Draw::STONE_SIZE)
	return 1;   // undo

    if (mouse_y >= y_middle + margin &&
	mouse_y <= y_middle + margin + Draw::STONE_SIZE)
	return 2;   // redo

    if (mouse_y >= y_middle + Draw::V_BUTTON_SPACE + margin &&
	mouse_y <= y_middle + Draw::V_BUTTON_SPACE + margin + Draw::STONE_SIZE)
	return 3;   // hint
    
    // miss
    return -1;
}

// 0 : nothing happened
// 1 : a button was clicked and dealt with
int handle_buttons()
{
    int b;
    
    // see if a button was clicked
    b = get_command_button();

    switch (b)
    {
	case 0:
	    // pause/play
	    paused = !paused;
	    break;
	case 1:
	    // undo
	    undo = 1;
	    paused = 1;
	    break;
	case 2:
	    // redo
	    paused = 1;
	    redo = 1;
	    break;
	case 3:
	    // hint
	    hint = 1;
	    break;
	default:
	    // missed all buttons
	    return 0;
	    break;
    }

    return 1;
}

// get the board coordinates of the mouse cursor
int get_mouse_pos()
{
    // roundoff trouble: -50/70 = 0
    // can I please shoot the idiot that made floor a _double_ function?
    // how the *(#^&*& am I supposed to make an integer value without
    // having to doubt floating point precision? If floor returns 5.99999,
    // this code will still break.
    
    int x = (int)floor((mouse_x - Draw::BOARD_X - Draw::BOARD_MARGIN)
		       / (float)Draw::FIELD_SIZE);
    int y = (int)floor((mouse_y - Draw::BOARD_Y - Draw::BOARD_MARGIN)
		       / (float)Draw::FIELD_SIZE);

//    allegro_message("x: %d  y: %d\n", x, y);
    
    if (x < 0 || x > 7 || y < 0 || y > 7)
	return -1;

    return y * 8 + x;
}

// 0 = valid move    1 = skipped a player   2 = game over
int check_game_over(Board &b, int who)
{
    // count_moves does (should) not modify the board
    if (!b.count_moves(who))
    {
	draw.message("Player %d has no valid moves.",who);

	if (!b.count_moves(3 - who))
	{
	    int one, two;
	    b.count_stones(one, two);
	    
	    // no moves either? game over.
//	    draw.message("Player %d has no valid moves.\n", 3 - who);
//	    allegro_message("Player 1: %d stones. Player 2: %d stones.\n",
//			    one, two);
	    draw.message("Game Over, press a key.");

	    // clear keybuffer just in case
	    clear_keybuf();

	    return 2;
	}
	else
	{
	    // the other player may play again
	    return 1;
	}
    }

    // player can move normally
    return 0;
}    


// determine which move to make
// this differs for computer / human players
// return the selected move
int get_move()
{
    int
	pos = -1,
	n1,
	n2,
	ntot,
	nply = ply;

    // clear message if necessary
    if (clear_message)
	draw.clear_message();
    
    // check if a key was pressed. This is necessary to determine if the
    // menu was requested during a computer turn (otherwise you can't quit
    // in a 2 computer game). Because the key may have been pressed
    // earlier, use readkey to detect it. Because MANY keys may have been
    // pressed, clear the from the buffer before going into the menu :-)

    if (keypressed())
    {
	if (readkey() >> 8 == KEY_ESC)
	{
	    quit = 1;

	    // clear all keys
	    clear_kbd(KEY_ESC);
	    
	    return 0;
	}
    }

    // hint == 1: request hint.
    // hint == 2: computer start working
    // hint == 3: hint completed, draw on screen
    if (computer_player[who - 1] || hint == 2)
    {
	// computer is up
	int score;

	// compute the ply level
	aap.count_stones(n1, n2);
	ntot = n1 + n2 + ply;

 	if (ntot > 50)
	    nply++;
	if (ntot > 52)
	    nply++;
	if (ntot > 54)
	    nply++;
	if (ntot > 56)
	    nply++;
	if (ntot > 58)
	    nply++;

	// make sure we don't pass our array boundaries
	if (nply >= MAX_PLY)
	    nply = MAX_PLY - 1;

	// put the current situation on the stack
	// and do it AFTER computing the ply level please!!!
	// boards[nply] = aap;
	*boards = aap;

	// let the computer find the best move
	int ret;
	abort_eval = 0;    // reset abort flag
	try
	{
	    ret = eval(boards, nply, who, score, pos,
		       false, -OTH_INFINITY, OTH_INFINITY);
	}
	catch (int err)
	{
	    // clear keyboard
	    clear_kbd(KEY_ESC);

            // aborted: ignore result
	    paused = 1;
	    return 0;
	}

//	allegro_message("computer move: ret %d, score %d, pos %d, ply %d\n",
//			ret, score, pos, nply);

	if (pos < 0 || pos > 63)
	{
	    paused = 1;
//	    draw.message("Game stopped, press a key.");
//	    readkey();
//	    quit = 1;
	    return 0;
	}		    

	if (!win_message && (score > OTH_INFINITY/2 || score < -OTH_INFINITY/2))
	{
	    draw.message("Player %d will win.",
			    (score < 0) ? who : 3 - who);
	    win_message = 1;
	}
    }
    else
    {
	// player is up
	// read a mouse click or escape key

	while (pos < 0)
	{
	    while (!(mouse_b & 1))
	    {
		// clear message if necessary and redraw
		if (clear_message)
		{
		    draw.clear_message();
		    draw.draw_screen(aap);
		}

		poll_keyboard();
		if (key[KEY_ESC])
		{
		    quit = 1;
		    return 0;
		}

		// wait patiently
		rest(1);
	    }
	    
	    // mouse was clicked
	    if (handle_buttons())
	    {
		// a command button was clicked and dealt with
		// wait until mouse is released
		while (mouse_b & 1)
		    rest(1);
		    ;
		
		return 0;
	    }
	    
	    // get the move coordinates
	    pos = get_mouse_pos();
	    
	    // wait until it is released
	    while (mouse_b & 1)
		rest(1);
		;
		
	    // if invalid, try again
	    if (pos < 0 || pos > 64)
		continue;
	}
    }

    return pos;
}

// perform the given move for player who (global)
int move(int pos)
{
    Board test;
    int nflips;
    
    // copy our board
    test = aap;
	
    // try the move
    nflips = test.move(who, pos);
	
    if (nflips <= 0)
    {
	// occupied or invalid, beep.
	draw.message("Invalid move.");
	draw.beep();
    }
    else
    {
	// succesful: copy the move to the real board
	aap = test;

//	allegro_message("Successful move: flipped %d stones.\n",
//			nflips);
	    
	// next player is on
	who = 3 - who;

	// function no longer modifies who variable
	// 0 = valid move found
	// 1 = skipped a player (change who)
	// 2 = game over
	int r = check_game_over(aap, who);
	if (r == 2)
	{
	    // indicate we cannot continue.
	    return -1;
	}
	else if (r == 1)
	{
	    // we skipped a player
	    who = 3 - who;
//	    allegro_message("Player %d is up again!\n", who);
	}
	else
	{
//	    allegro_message("Player %d is up!\n", who);
	}

//	allegro_message("Player %d has %d valid moves.\n",
//			who, aap.count_moves(who));
	
	// succesful move performed
	return 1;
    }

    return 0;
}

void paused_loop()
{
    // let player do only command buttons and menu (ESC)

    int update = 0;
    
    while (paused)
    {
	if (undo)
	{
	    // before you undo, log the current situation with a -1 move.
	    // this is necessary so you can get _back_ to that situation with
	    // redo's. Otherwise, you can only get back to the one-before-last
	    // situation with redo. -1 move will make certain that if it ever
	    // gets used you'll notice it. Also, the move counter won't be
	    // advanced so we don't need to go back 2 steps after this.
	    logfile.log_move(who, -1, &aap);
	    
	    if (!logfile.undo())
	    {
		draw.beep();
		draw.message("No undo information available.");
	    }
	    undo = 0;
	    update = 1;
	}
	
	if (redo)
	{
	    if (!logfile.redo())
	    {
		draw.message("No redo information available.");
		draw.beep();
	    }
	    redo = 0;
	    update = 1;
	}

	if (hint == 1)
	{
	    // hint was set from the pause loop need to do some tweaking to
	    // get around twisted game logic here: set hint to 2 so it will
	    // go straight to hint computation when coming back in the main
	    // loop. Unpause the game, otherwise nothing will happen.
	    
	    draw.message("Game resumed, computing hint ...");
	    hint = 2;
	    update = 1;
	    paused = 0;
	}
	else if (hint > 1)
	{
	    // if we come here and hint is 2, then the hint computation
	    // was aborted and pause turned on. if we unpause, don't
	    // start computing again right away!
	    
	    draw.message("Hint computation aborted.");
	    hint = 0;
	    update = 1;
	}
    
        // clear message if necessary
	if (clear_message)
	{
	    draw.clear_message();
	    update = 1;
	}

	// check for mouse input
	if (mouse_b & 1)
	{
	    // deal only with command buttons
	    handle_buttons();

	    // wait until mouse is released
	    while (mouse_b & 1)
		rest(1);
		;

	    // update may be needed (pause button)
	    update = 1;
	}

	// check for escape
	poll_keyboard();
	if (key[KEY_ESC])
	{
	    clear_kbd(KEY_ESC);
	    quit = 1;
	    return;
	}

	// need to update screen ourselves
	if (update)
	{
	    draw.remember(aap);
	    draw.draw_screen(aap);
	    update = 0;
	}
	
	// be nice to the cpu
	rest(1);
    }
}

int game_loop()
{
    int
	r,
	pos,
	w,
	p;

    Board
	logboard;
    
    // flip pancakes if necessary
    draw.pancake(aap);

    // start with updating the screen
    draw.draw_screen(aap);
    
    // have the draw object remember this state so it can determine which
    // stones to flip later
    draw.remember(aap);

    // if we're paused, don't try to move.

    if (paused)
    {
	// if pause is on, let the player know what happened
	draw.message("Game paused.");
    
	paused_loop();
	if (quit)
	    return 0;
    }
    
    // first, obtain the move to make. this is of course different for
    // the computer and the player.
    pos = get_move();

    if (hint == 1)
    {
	// hint computation requested
	draw.message("Computing hint ...");
	hint = 2;
	return 1;
    }

    if (hint == 2)
    {
	// hint computation performed
	// if it was a hint computation, set the position for draw
	hint = 3;
	
	draw.set_hint(pos);
	draw.clear_message();
	return 1;
    }
    
    // check if the player wants to quit
    if (quit)
	return 0;

    // check if pause was turned on. if so, handle it next game loop
    if (paused)
	return 1;
    
    // save pos and who for logging, and the board
    p = pos; w = who; logboard = aap;
    
    // next, actually perform the move
    r = move(pos);

    // succesful move, reset hint flag
    hint = 0;
    
    // log the move if it was valid (-1 or 1)
    if (r)
	logfile.log_move(w, p, &logboard);
    
    if (r < 0)
    {
	// game over
	// do a final screen update before we return so the player can see
	// the result.
	draw.draw_screen(aap);
	draw.pancake(aap);
	draw.remember(aap);
	readkey();

	// save game automagically
 	string tmp = file.othello_dir();
 	tmp += "last.sav";
 	ofstream ofs(tmp.c_str());
 	if (!ofs || !file.save_file(ofs, who, &aap, &logfile))
 	{
 	    allegro_message("Could not save '%s'.\n", tmp.c_str());
 	}

	return 0;
    }
    
    // no game over and no quit -> next game loop
    return 1;
}

// allocate MAX_PLY boards
void boards_init()
{
    boards = 0;
    for (int i = 0; i < MAX_PLY; i++)
    {
	Board *tmp = new Board;
	tmp->next = boards;
	boards = tmp;
    }
}

int main()
{
    allegro_init();

    // keyboard, mouse, timer
    install_timer();
    install_mouse();
    install_keyboard();

    // init gfx
    if (!draw.init_graphics())
	return 0;

    // sound
    reserve_voices(1, 0);
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0) < 0)
    {
	allegro_message("Failed to install digital sound.\n");
	    
	// failed
	if (install_sound(DIGI_NONE, MIDI_NONE, 0) < 0)
	{
	    // error
	    allegro_message("Error: Failed to install no-sound.\n");
	}
    }
    
    // init random generator
    srand(time(0));

    // initialize options
    file.load_settings();

    // boards init
    boards_init();
    
    // draw the dialog
    do_game_dialog();
    
    while (1)
    {
	// see if we should stop
	if (exit_program)
	    return 0;

	// do nothing
	int ret = game_loop();

	// menu requested?
	if (!ret)
	{
	    //clear_keybuf();
	    do_game_dialog();
	}
    }

    return 0;
}
END_OF_MAIN()
    
