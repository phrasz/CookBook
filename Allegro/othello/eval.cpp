#include <allegro.h>
#include <stdlib.h>
#include "board.h"
#include "eval.h"

// eval.cc
//
// evaluate a board, and determine the best move to make for the
// player that's currently on. Recursive algorithm is the best I know
// (but not good enough, since slow): for each possible move evaluate
// the resulting board, and select best move. One ply (= recursive
// level), do the same, but this time for the other player. For him,
// ALSO select his best move! This means that, for ply = 2, we get the
// following situation:
//
//  current board:                 0         
//                                / \        2 possible moves for player 1
//  ply 1                        1   2      
//                       +---+---+   +---+   5 posible moves for player 2
//                      /   /   /     \   \                 .
//  ply 2              3   4   5       6   7
//
// For board 1, player 2 selects his best move, 4 (score 100).
// For board 2, player 2 selects his best move, 7 (score 150).
//
// These scores are passed 'upwards', and provide the score for boards
// 1 and 2. Player 1 selects his 'best case' from these: score 100
// (what is good for player 2, is bad for player 1).
//
// What you end up doing, is selecting the move that gives the
// opponent the worst options in his next move. If you can only look
// one step ahead, select the move that has the best board situation.
// If we can look 2 steps ahead, select not only the best (worst for
// the other) move, but add to the scoring mechanism the number of
// possible moves that the opponent has. If it is high, his score is
// better. If it is low, his score is worse, better for us.


// recursive function eval
//
// parameters:
// Board *b        an array of boards (MAX_PLY + 1 elements)
// ply             current ply depth. 0 -> stop
// who             the player that's "on"
// score           reference parameter to return the best (worst) score
// move            the suggested move for this board
// no_move         parameter indicating if this was called from a 'no_move'
//                 situation or not. If so, do not try that again so we
//                 don't get an infinite recursion.
//
// return values:
//
// EVAL_NORMAL     nothing special has happened
// EVAL_WIN        current player has won
// EVAL_LOSE       current player has lost

enum eval_retvals
{
    EVAL_NORMAL = 0,
    EVAL_WIN    = OTH_INFINITY/2,        // don't let a win or loss cut off
    EVAL_LOSE   = -OTH_INFINITY/2        // unless there are non-win/loss moves.
};

int win(Board *b, int who)
{
    int
	one,
	two;

    // differentiate to the number of stones that the winner has.
    // A better win (more stones) should return a higher score.
    // A worse loss (more enemy stones) should return a lower score.
    // It's quite ok to overdo it here with the stone factor, as long as
    // the total doesn't exceed OTH_INFINITY :-)
    
    b->count_stones(one, two);
    if ((who == 1 && one > two) || (who == 2 && two > one))
    {
	return EVAL_WIN + ((who == 1) ? one : two) * 100;
    }
    else if ((who == 1 && one < two) || (who == 2 && two < one))
    {
	return EVAL_LOSE - ((who == 1) ? two : one) * 100;
    }

    // draw
    return EVAL_NORMAL;
}

// Minimax algorithm with alpha-beta cutoff
// the previous version of eval (eval.minimax) did not contain the alpha
// beta cutoffs. I am now introducing them to (hopefully) speed things up
// greatly, and possibly simplify things a bit.
//
// Cutoff system: if the parent is maximizing, a child can stop minimizing
// if it has found a score below the best score the parent has found
// sofar. If a parent is minimizing, the child can stop if it finds a value
// higher than the best value of the parent. Reasoning: the parent will
// never choose that value, so no point in continuing with the rest of the
// possible moves. Implementation (normally): keep an alpha (minimum) and
// beta (maximum) cutoff and pass them on each time. A maximizing node will
// modify the alpha (minimum) cutoff value, and a minimizing node will
// modify the beta (maximum) cutoff value.
//
// I do not minimize and maximize, instead I flip my scores around each
// time I go a level deeper, so I am always maximizing. This means I need
// to always modify the minimum cutoff value for the child (initialized at
// my own maximum cutoff value). This simply means each time eval is called
// it should swap the alpha and beta parameters, and multiply them by -1.
// Alpha and Beta should NOT be reference parameters, as they are only
// passed on towards children, and I want to be able to change them
// locally.
//
// Because the minimax algorithm works purely on score, I will get rid of
// my EVAL_LOOSE and EVAL_WIN return values, and instead use an extremely
// high maximum score. Keep in mind that this score may be modified up and
// down by the degrees of freedom scoring, so not all 'wins' may weigh the
// same.
//
// I'm not sure (yet) if it's allowed to modify scores halfway the
// tree. minimax passes on the cutoff values, which works nicely. If
// however a parent node starts changing my returned score, it can either
// get out of bounds (I would not have returned it myself, hitting a
// cutoff) or it may lie within the bounds again, so I have stopped too
// soon. This will affect the degree of freedom algorithm: it's not allowed
// to modify the score on the way down the tree. Modifying it at the leaf
// nodes is hard because we don't know (yet) how many valid moves there
// are. Solution: make an array of 64 moves or a linked list of possible
// moves, and only after computing the number of moves start evaluating
// them. Another solution: do the degree of freedom computation only at ply
// 1, where you can directly modify the returned score, and still keep it
// within your own limits before returning.
//
// Hints from kreversi:
// - increase ply depth near the end, and cut it off if necessary at the
//   ply depth needed to reach the end.
// - there should be a shifting balance between positional play (beginning
//   of game), total number of stones (endgame) and degrees of freedom
//   (always). Modify scoring algorithm to reflect this.
// - keep an incremental count of the number of stones each player has on
//   the board, and simplify the count_stones function accordingly.

int abort_eval = 0;

int eval(Board *b, int ply, int who, int &score, int &move,
	 bool no_move, int alpha, int beta)
{
//    Board *b1 = &boards[ply];
    Board *b1 = b;
    Board *b2;

    int
	best_score = 0,
	best_move = 0,
	fr = 0,
	ret = 0,
	init = 0;

    // compute enemy neighbour stones
    // this costs more time than it gains so get rid of it
    // maybe with an incremental neighbour system it could work.
    //
    // uint64_t
    //     neighbours = b1->neighbours(3 - who);

    // these aren't always used, initialize to 'unused' values.
    move = -1;
    score = 0;

//    allegro_message("eval called, ply %d, who %d\n", ply, who);

    // at the very top: check global variable abort_eval, to be able to
    // interrupt the whole thing. Set abort_eval from a timer routine, to
    // avoid checking for key[KEY_ESC] 16 million times. Not that makes any
    // difference, but it may matter if you want to do more complicated
    // mouse checks.

    if (abort_eval)
	throw 1;
    
    // make a distinction based on ply level
    // if we arrive at ply 0, compute score and don't look further.
    if (!ply)
    {
	// compute score and return it
	// I need to know how far in the game we are for this,
	// take it from a table.
	score = b1->score();

	// player 1 gets positive scores, player 2 negative.
	// since the player interested in this boards score is the
	// one that was on _last turn_, swap the results.
	if (who == 1)
	    score *= -1;

//	allegro_message("eval returns score %d\n", score);
	return EVAL_NORMAL;
    }
    
    // pick a scratch board
//    b2 = &boards[ply - 1];
    b2 = b1->next;
    if (!b2)
    {
	// panic real loud
	allegro_message("BUG! out of stack space or damaged stack list.\n");
	exit(1);
    }
    
    // traverse all possible moves
    // continue one too far: if i == 64, we do a 'no move' and let the
    // other player continue with that situation.
    for (int i = 0; i < 65; i++)
    {
	// only try to move if we have enemy neighbours
//	if (i < 64 && !get(neighbours, i))
//	    continue;
	
	// copy original board
	*b2 = *b1;

	if (i < 64)
	{
            // perform each move
	    if (b2->move(who, i) <= 0)
	    {
//		allegro_message("invalid move: who %d, pos %d\n", who, i);
		
		// not a valid move
		// continue with the next one
		continue;
	    }
	    else
	    {
		// count valid move
		fr++;
	    }
	    
	    // call eval on the new board with (3 - who) and (ply - 1)
//	    allegro_message("normal eval call for valid move: ply %d who %d\n",
//			    ply, who);
//	    ret = eval(boards, ply - 1, 3 - who, score, move,
//		       false, -beta, -alpha);
	    ret = eval(b2, ply - 1, 3 - who, score, move,
		       false, -beta, -alpha);
	}
	else
	{
	    // we arrived at move 65: the 'no-move'.
	    // if we found valid moves, don't continue.
	    if (fr)
		break;

            // else: do a 'no-move'.  pass true for the no-move parameter
	    // so the next eval will know that if it happens again, they
	    // should stop evaluating. First check if we should stop
	    // ourselves.
	    if (no_move)
	    {
		// game over. compute winner/looser and return.
		// this just gives an insanely high score for winning and
		// an insanely low score for loosing. A draw will give
		// score 0.
		score = win(b2, who);
	    }
	    else
	    {
		// one no_move is allowed. The next player is on.
		// do not decrease the ply depth, since the tree did not
		// expand from this move, although we did have to try all
		// the moves.

		// whoooo baby, that's no good. You can't just change the
		// ply level here, as it is used to select a scratch board
		// from the stack. The next eval will overwrite itself
		// while moving, thus screwing everything up.
		// This can only work if you use a linked list and get
		// b->next as the next scratch board.
		// ok, changed to linked list so can call with ply now
		// instead of ply - 1
		
//		allegro_message(
//		    "no-move eval call for valid move: ply %d who %d\n",
//		    ply, 3 - who);
//		ret = eval(boards, ply, 3 - who, score, move,
//			   true, -beta, -alpha);
		ret = eval(b2, ply, 3 - who, score, move,
			   true, -beta, -alpha);
	    }
	}

	// introduce a little randomness if scores are equal
	// I don't want to save all possible moves (stubborn) so I make a
	// non-fair random distribution. Favour later moves, so that the
	// very last one also has a chance.
	
	// compare score with best score and optionally change it
	// to this move. The first valid move always is the
	// best. If we were loosing, this is by definition the
	// first valid move.
	if (!init
	    || score > best_score
	    || (score == best_score && rand() > RAND_MAX * 3 / 2))
	{
	    best_score = score;    // save score
	    init = 1;              // we had a valid move

	    // if we couldn't move, then i is 64. We may still have a valid
	    // move stored in 'move', from the second call to eval. If that
	    // one also couldn't move (returned 64 in 'move') then we have
	    // a game over situation. Either our caller is not interested
	    // in our returned move then, or it is a bug.
	    if (i == 64)
	    {
		best_move = move;
	    }
	    else
	    {
		best_move = i;                    // save move
	    }
	}

	// alpha / beta cutoff
	// alpha is minimum value, beta maximum value.

	// modify the lower cutoff limit
	if (score > alpha)
	    alpha = score;

	// since we are always maximizing, we only cutoff on beta.
	if (score > beta)
	{
	    // hit the cutoff limit?
	    // the other results are no longer interesting as this branch
            // will not be selected anyway, unless it is the only branch
            // available. Jump out of the loop and return normally.
	    break;
	}
    } 

    // done traversing all the moves.

    // modify score by the number of possible moves
    // degrees of freedom are good, so value them positively.  this will
    // cancel out against the other players degrees of freedom anyway, so
    // only the ratio FREEDOM_BONUS to field bonus matters.

    // this doesn't work anymore with the alpha-beta cutoff mechanism,
    // unless we save the moves we have done until now and evaluate the
    // scoring afterwards. This requires keeping an array of 62 ints, for
    // the returned score values.
    
    // return the best score, and the move that goes with it
    // the next player is interested in negative scores, as
    // he wants to bother me and not help me.
    score = -1 * best_score;
    move = best_move;

    return EVAL_NORMAL;
}
