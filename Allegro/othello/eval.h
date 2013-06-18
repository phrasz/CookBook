#ifndef _EVAL_H_
#define _EVAL_H_

// score for one (extra) degree of freedom
// 50 seems to be a bit much, computer will sometimes skip getting a corner
// in favor of degrees of freedom. Hopefully 10 will do better.
#define FREEDOM_BONUS 10         
#define STONE_BONUS   10

#define OTH_INFINITY 10000000

extern int eval(Board *b, int ply, int who, int &score,
		int &move, bool no_move, int alpha, int beta);

extern int abort_eval;

#endif
