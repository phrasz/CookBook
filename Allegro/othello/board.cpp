#include <stdio.h>
#include <allegro.h>
#include "board.h"
#include "eval.h"

Board::Board()
{
    p1 = p2 = 0;
    score_1 = 0;
    score_2 = 0;
    next = 0;
}

Board::Board(Board const &other)
{
    next = 0;
    copy(other);
}

Board::~Board()
{
    // do nothing
}

Board const &Board::operator=(Board const &other)
{
    if (this != &other)
    {
	copy(other);
    }

    return (*this);
}

void Board::copy(Board const &other)
{
    p1 = other.p1;
    p2 = other.p2;
    score_1 = other.score_1;
    score_2 = other.score_2;
}

void Board::init()
{
    // set  starting positions

    p1 = p2 = 0;
    score_1 = score_2 = 2;
    
//     set(p1,28);
//     set(p1,35);
//     set(p2,27);
//     set(p2,36);

    set(p1,28);
    set(p1,35);
    set(p2,27);
    set(p2,36);
}

void Board::print() const
{
    for (int i = 0; i < 8; i++)
    {
	for (int j = 0; j < 8; j++)
	{
	    int w = 0;
	    if (get(p1, i*8+j))
		w = 1;
	    else
	    if (get(p2, i*8+j))
		w = 2;
	    
	    printf("%d  ", w);
	}
	printf("\n\n");
    }
}

void Board::draw() const
{
//    clear_to_color(screen, makecol(0,0,0));
    
    for (int j = 0; j < 8; j++)
    {
	for (int i = 0; i < 8; i++)
	{
	    // draw a rectangle around this position
	    // use a 600x600 rectangle in the center of the screen
// 	    rect(screen,
// 		 120 + 70*i, 20 + 70*j,
// 		 120 + 70*(i+1), 20 + 70*(j+1),
// 		 makecol(200, 200, 200));

	    int w = makecol(0, 0, 0);
	    if (get(p1, j*8+i))
	    {
		w = makecol(255, 255, 0);
		circlefill(screen, 120 + 35 + 70*i, 20 + 35 + 70*j, 30, w);
	    }
	    else
	    if (get(p2, j*8+i))
	    {
		w = makecol(255, 0, 0);
		circlefill(screen, 120 + 35 + 70*i, 20 + 35 + 70*j, 30, w);
	    }
	    
	    // draw a circle
	}
    }
}

int Board::move(int who, int p)
{
    // perform the move. if it is not valid, return 0, otherwise
    // return the number of flips. -1 if the position is already taken.

    int y = p / 8;
    int x = p - y * 8;        // maybe it's faster than % :-)
    
    // check if the position is occupied
    if (get(p1, p) || get(p2, p))
    {
	return -1;
    }

    // start flipping stones, and count them.
    // mark stones to flip in temporary variable
    // only truly flip them at the end

    uint64_t *enemy;
    uint64_t *pal;
    int *score_enemy;
    int *score_pal;
    uint64_t flip_these;
    int
	total_flipped = 0,
	flipped,
	xp,
	yp,
	tp;

    if (who == 1)
    {
	enemy = &p2;
	pal = &p1;
	score_enemy = &score_2;
	score_pal = &score_1;
    }
    else
    {
	enemy = &p1;
	pal = &p2;
	score_enemy = &score_1;
	score_pal = &score_2;
    }
    
    // loop over all directions
    for (int di = -1; di < 2; di++)
    {
	for (int dj = -1; dj < 2; dj++)
	{
	    // skip 0,0
	    if (!(di || dj))
		continue;
	    
	    // initialize position
	    // loop until edge reached
	    // count number of enemy stones
	    // mark them for flipping
	    flipped = 0;
	    flip_these = 0;
	    xp = x + di;
	    yp = y + dj;

	    while (xp >= 0 && xp < 8 && yp >= 0 && yp < 8)
	    {
		// compute position
		tp = yp * 8 + xp;

		if (get(*enemy, tp))
		{
		    // mark field
		    set(flip_these, tp);
		    flipped++;
		}
		else
		{
		    // check if it's our own
		    if (flipped && get(*pal, tp))
		    {
			// flip marked stones
			p1 ^= flip_these;
			p2 ^= flip_these;
			
			// count flipped stones
			total_flipped += flipped;
		    }

		    break;
		}

		// move to next field
		xp += di;
		yp += dj;
	    }
	}
    }

    // we've checked all directions. total_flipped contains the
    // number of stones that were flipped, and also makes our
    // return value.

    if (total_flipped)
    {
	// place the stone of our move
	set(*pal, p);

	// count new score
	*score_pal   += total_flipped + 1;
	*score_enemy -= total_flipped;
    }
    
    return total_flipped;
}

int Board::count_moves(int who) const
{
    Board tmp;
    int nr_moves = 0;
    
    // count the number of possible moves for player who
    if (who != 1 && who != 2)
	return -1;

    for (int pos = 0; pos < 64; pos++)
    {
	// copy from this board
	tmp = *this;

	if (tmp.move(who, pos) > 0)
	    nr_moves++;
    }

    return nr_moves;
}


// count the stones for each player
void Board::count_stones(int &one, int &two) const
{
    one = score_1;
    two = score_2;
}

// compute the board score
// use a score value for each square of the board
// optionally do a lot of clever things here like
//
// - checking if we control a neighbour stone and modifying the score
// - not counting dead stones (other player controls row from the corner)
// - somehow detecting that empty squares already 'belong' to a player
//
// the last one may not be useful as our eval algorithm will mostly catch
// this. The same may go for the second option.

// scoreboard, positive scores are good, negative scores are bad
// I want to separately score positional play and number of stones. This
// doesn't work if the whole board has positive scores: the computer will
// implicitly favor moves which result in a higher number of stones for
// him, since it increases his score. Therefore make the average score on
// the board about 0, with the exception of the corners.
static int scores[64] =
{
    100, -30,  15,  15,  15,  15, -30, 100,
    -30, -50, -20, -20, -20, -20, -50, -30,
    15,  -20,   0,   0,   0,   0, -20,  15,
    15,  -20,   0,   0,   0,   0, -20,  15,
    15,  -20,   0,   0,   0,   0, -20,  15,
    15,  -20,   0,   0,   0,   0, -20,  15,
    -30, -50, -20, -20, -20, -20, -50, -30,
    100, -30,  15,  15,  15,  15, -30, 100
};

// weight given to the amount of stones in the scoring algorithm, in %
int stone_weights[64] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 10, 10, 10, 10, 10,
    20, 20, 20, 20, 20, 20, 30, 40, 60, 70,
    80, 90, 90, 100
};

// initialize corner bitflags
// neighbours of the corners
// static uint64_t ul =
//     ((uint64_t) 1 << 1)  &
//     ((uint64_t) 1 << 8)  &
//     ((uint64_t) 1 << 9);
// static uint64_t ur =
//     ((uint64_t) 1 << 6)  &
//     ((uint64_t) 1 << 14) &
//     ((uint64_t) 1 << 15);
// static uint64_t ll =
//     ((uint64_t) 1 << 48) &
//     ((uint64_t) 1 << 49) &
//     ((uint64_t) 1 << 57);
// static uint64_t lr =
//     ((uint64_t) 1 << 54) &
//     ((uint64_t) 1 << 55) &
//     ((uint64_t) 1 << 62);

// the corners themselves
static uint64_t ulc = (uint64_t) 1 << 0;
static uint64_t urc = (uint64_t) 1 << 7;
static uint64_t llc = (uint64_t) 1 << 56;
static uint64_t lrc = (uint64_t) 1 << 63;

// player 1 produces positive scores,
// player 2 produces negative scores
// stone_weight is a factor (0 to 100%)
// determining how heavy the number of stones is weighed in the
// computation.
int Board::score() const
{
    int pos_score = 0;
    int stone_score = (score_1 - score_2) * STONE_BONUS;
    int stone_weight = stone_weights[score_1 + score_2];

    // sum up the positional scores
    for (int i = 0; i < 64; i++)
    {
	if (get(p1, i))
	{
	    // optimize this using ul,ur,ll,lr bitfields
	    // if (((ipos & ul) && (p1 & ulc)
	    //
	    // the negatives around the corners if you own the corner
	    if (
		((i == 1 || i == 8 || i == 9) && (p1 & ulc))
		||
		((i == 6 || i == 14 || i == 15) && (p1 & urc))
		||
		((i == 48 || i == 49 || i == 57) && (p1 & llc))
		||
		((i == 54 || i == 55 || i == 62) && (p1 & lrc))
		)
	    {
		pos_score -= scores[i];
	    }
	    else
	    {
		pos_score += scores[i];
	    }
	}
	if (get(p2, i))
	{
            // nullify the negatives around the corners if you own the corner
	    if (
		((i == 1 || i == 8 || i == 9) && (p2 & ulc))
		||
		((i == 6 || i == 14 || i == 15) && (p2 & urc))
		||
		((i == 48 || i == 49 || i == 57) && (p2 & llc))
		||
		((i == 54 || i == 55 || i == 62) && (p2 & lrc))
		)
	    {
		pos_score += scores[i];
	    }
	    else
	    {
		pos_score -= scores[i];
	    }
	}
    }

    return (pos_score*(100-stone_weight) + stone_score*stone_weight) / 100;
}

// for each position, compute if it has neighbours of owner who
// this should speed up moving a little, as you don't need to try a
// position that doesn't have enemy neighbours.
//
// also, check if the position is already filled, if so don't mark it
// so it will be skipped in the moving.
uint64_t Board::neighbours(int who) const
{
    uint64_t
	nb = 0;
    uint64_t const
	*me    = ((who == 1) ? &p1 : &p2),
	*enemy = ((who == 2) ? &p1 : &p2);
    int
	offsets[8] = {-9, -8, -1, 1, 7, 8, 9};
    int
	p,
	o,
	tp;
	    	
    for (p = 0; p < 64; p++)
    {
	if (get(*me, p))
	{
	    for (o = 0; o < 8; o++)
	    {
		tp = p + offsets[o];

		if (tp >= 0 && tp < 64)
		{
		    if (!(get(*me, tp) || get(*enemy, tp)))
			set(nb, tp);
		}
	    }
	}
    }

    return nb;
}

int Board::getstone(int pos) const
{
    if (get(p1, pos))
	return 1;
    else if (get(p2, pos))
	return 2;
    else
	return 0;
}

int Board::setstone(int pos, int who)
{
    if (pos < 0 || pos > 63)
	return 0;
    
    if (who == 1)
    {
	set(p1, pos);
	return 1;
    }
    else if (who == 2)
    {
	set(p2, pos);
	return 1;
    }
    else if (!who)
	return 1;

    return 0;
}

// return only the fields that are different.
// both new stones and flipped stones.
uint64_t Board::diff(Board const &other) const
{
    uint64_t ret = 0;

    ret = (p1 ^ other.p1) | (p2 ^ other.p2);

    return ret;
}

// return only flipped stones
uint64_t Board::flipped(Board const &other) const
{
    uint64_t ret = 0;

    ret = (p1 ^ other.p1) & (p2 ^ other.p2);

    return ret;
}

int Board::read_from(ifstream &ifs)
{
    int p, w;

    p1 = p2 = 0;
    score_1 = 0;
    score_2 = 0;
    
    for (int i = 0; i < 64; i++)
    {
	ifs >> p;
	ifs >> w;

//	cerr << "trying to setstone(" << p << ", " << w << ")"
//	     << endl;
	
	if (!setstone(p, w))
	    return 0;

	// compute scores
	if (w == 1)
	    score_1++;
	if (w == 2)
	    score_2++;
    }
    
    return 1;
}

void Board::write_to(ofstream &ofs) const
{
    // write board
    for (int i = 0; i < 64; i++)
    {
	ofs << i << " " << getstone(i) << endl;
    }
}
