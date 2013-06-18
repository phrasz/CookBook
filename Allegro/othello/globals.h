#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "board.h"
#include "draw.h"

// don't include log.h, it causes troubles with get because of <fstream>
class Log;
class File;

// declare global variables

#define MAX_PLY 20

//extern Board boards[MAX_PLY];

extern Board *boards;
extern Board aap;
extern int who;
extern int who_starts;
extern int ply;
extern int computer_player[2];     // default: computer is player 2
extern int quit;
extern int exit_program;
extern int win_message;
extern int animations;
extern int clear_message;
extern int paused;
extern int hint;
extern Draw draw;
extern Log logfile;
extern File file;

#endif
