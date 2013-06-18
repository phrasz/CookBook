#ifndef _BOARD_H_
#define _BOARD_H_

#include <fstream>
#include "longlong.h"

using namespace std;

#define set(var, pos)   ((var) |= ((uint64_t)1 << (pos)))
#define get(var, pos)   ((var) & ((uint64_t)1 << (pos)))

class Board
{
public:
    Board();
    Board(Board const &other);
    ~Board();

    Board const &operator=(Board const &other);
    void copy(Board const&other);
    
    void init();                              // set initial positions
    void print() const;                       // print board status
    void draw() const;                        // draw on screen

    int getstone(int pos) const;              // return 0, 1 or 2 (who)
    int setstone(int pos, int who);           // set stone at pos
    int move(int who, int pos);               // make a move, return success
    int move_old(int who, int pos);
    int count_moves(int who) const;           // count possible moves for who
    int score() const;                        // compute score for this board
    void count_stones(int &one, int &two) const;    // count for each player
    uint64_t neighbours(int who) const;   // compute neighbour stones
    uint64_t diff(Board const &other) const;    // difference
    uint64_t flipped(Board const &other) const; // flipped stones   

    int read_from(ifstream &ifs);
    void write_to(ofstream &ofs) const;
    
    Board *next;
    
private:
    uint64_t p1;
    uint64_t p2;
    int score_1;
    int score_2;
};

#endif
