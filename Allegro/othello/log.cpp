#include <iostream>
#include <string>
#include "log.h"
#include "globals.h"
#include "file.h"

Log::Log()
{
    init();
}

Log::~Log()
{
    // nothing to do
}

void Log::init()
{
    move = 0;
    max_move = 0;
    for (int i = 0; i < 60; i++)
    {
	moves[i][0] = moves[i][1] = 0;
    }
}

// log this move
// no checking for valid moves is done!
void Log::log_move(int w, int p, Board *b)
{
    // sanity check. This may actually occur after reaching the
    // end of the game! (try loading a completed game)
    if (move < 0 || move >= 60)
	return;

    // special case: p = -1 -> only store current position and update
    // max_move if necessary. Do not adjust the move pointer, it's not a
    // real move. Only do this if we are at the end of the queue, so the
    // current move has not been entered yet!
    if (p < 0)
    {
	if (max_move > move)
	{
	    // we're not at the end of the queue.
	    // this means we've done multiple undo's, and there's no need
	    // to save the current position, has already been done.
	    return;
	}

	// we are going to save the position: update max_move
	max_move++;
    }
    
    // log the move, and remember who's turn it was
    moves[move][0] = w;
    moves[move][1] = p;

    // copy the board as it was BEFORE the move was made, so you can undo
    // to n = x, and find the move and board that went with it.
    hist[move] = *b;

    // p < 0 (-1) is actually used to store current position without a
    // valid move, when doing an undo. If this happens, DO NOT ADVANCE the
    // move counter! this is only intended to store the current position,
    // it is not a real move! The max_move counter may have to be advanced
    // though: if it is <= the current position, advance it by 1.

    if (p >= 0)
    {
        // a 'real' move
	move++;

	// max_move keeps track to where the hist array is filled (exclusive).
	// however, if go back with undo's, then perform a new move yourself,
	// you cannot redo anymore! Therefore max_move may be set anytime the
	// player makes a move: after that you cannot redo.
	max_move = move;
    }
}

// file format:
//
// LOGFILE (optional date)
// 0   who   move
// 1   who   move
// ...
// 60  who   move
//
// if who is 0, the move has not been made yet.

// save to file
int Log::write_to(ofstream &of)
{
    of << "LOGFILE" << endl;
    for (int i = 0; i < 60; i++)
    {
	if (i < move)
	{
	    of << i << " " << moves[i][0] << " " << moves[i][1] << endl;
	}
	else
	{
	    of << i << " 0 0" << endl;
	}
    }

    return 1;
}

// read from file
int Log::read_from(ifstream &is)
{
    string line;
    int n, w, p;
    Board b;                // scratch board to perform moves on
    
    // start with a clean slate
    init();
    
    // start with initialized board
    b.init();
    
    is >> line;             // read comment line

    for (int i = 0; i < 60; i++)
    {
	is >> n;            // read move number
	is >> w;            // read who
	is >> p;            // read move

	if (w)    // valid move? make and log it
	{
	    // log the move first
	    log_move(w, p, &b);

	    // now make it
	    if (b.move(w, p) <= 0)
	    {
		// error

//		cerr << "log::read_from: invalid move " << n << ": who = "
//		     << w << ", pos = " << p << endl;
		
		return 0;
	    }
	}
    }
    
    return 1;
}

// undo function
int Log::undo()
{
    // move points (in normal cases) to an empty array spot, so the current
    // board will be stored in moves[move]. We want to go back to
    // moves[move-1]. Make sure that this array position exists, otherwise
    // we cannot undo. 

//    cerr << "undo: move = " << move << ", max_move = " << max_move << endl;
    
    if (move < 1)
	return 0;
    
    // go back one step in time
    move--;
    
    // bring back the board
    aap = hist[move];
    
    // bring back who's turn it was
    who = moves[move][0];

    // let the display flip stones for visual effect, don't call remember
    return 1;
}

// redo function
int Log::redo()
{
    // we can only redo if max_move is larger than move, otherwise there is
    // no redo information available (or it's no longer valid).

//    cerr << "redo: move = " << move << ", max_move = " << max_move << endl;

    if (move + 1 >= max_move)
	return 0;

    // the state we want to restore is the state in move+1
    move++;

    // bring back the board
    aap = hist[move];
    
    // bring back who's turn it was
    who = moves[move][0];

    // again, let the display flip so no draw.remember
    return 1;
}
