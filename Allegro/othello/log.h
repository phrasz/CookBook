#ifndef _LOG_H_
#define _LOG_H_

#include <fstream>
#include "board.h"

class Log
{
public:
    Log();
    ~Log();

    void init();                                // initialize to 0
    void log_move(int who, int pos, Board *b);  // log this move
    int write_to(ofstream &of);                 // save to file
    int read_from(ifstream &is);                // read from file
    int undo();                                 // undo a move
    int redo();                                 // redo a move
    
private:
    int move;           // move number
    int max_move;       // how far have we been?
    int moves[60][2];   // moves made sofar   [who][pos]
    Board hist[60];     // the undo buffer
};

#endif
