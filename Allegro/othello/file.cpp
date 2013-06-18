#include <iostream>
#include <sys/stat.h>
#include "file.h"
#include "board.h"
#include "log.h"

File::File()
{
    configfilename = othello_dir();
    configfilename += "othello.cfg";
}

File::~File()
{
    // nothing to do
}

string File::othello_dir()
{
    char *mydir;
    char mydircpy[1024];
    string ret;
    
    // first, try home dir
    mydir = getenv("HOME");

    if (!mydir)
    {
	// we're obviously not on unix.
	// try ".", as I know no better alternative.

	mydir = ".";

	// make a writeable copy
	strncpy(mydircpy, mydir, 1024);
	mydircpy[1023] = '\0';

	// append slash at the end, appropriate for platform
	put_backslash(mydircpy);

	ret = mydircpy;
	return ret;
    }

    // make a writeable copy
    strncpy(mydircpy, mydir, 1024);
    mydircpy[1023] = '\0';
    
    // append slash at the end, appropriate for platform
    put_backslash(mydircpy);

    // append "othello"
    strcat(mydircpy, ".othello");

    // and another backslash
    put_backslash(mydircpy);
        
    // back to string
    ret = mydircpy;

    // test if the result exists. If not, make it.
    struct stat buf;
    if (stat(ret.c_str(), &buf) < 0)
    {

#if defined(ALLEGRO_MSVC) || defined(ALLEGRO_BCC32) || defined(ALLEGRO_WATCOM) || defined(ALLEGRO_MINGW)
	if (mkdir(ret.c_str()) < 0)
#else
	if (mkdir(ret.c_str(), 0755) < 0)
#endif

	{
	    allegro_message("Could not create %s, exiting.\n", ret.c_str());
	}
    }
    
    return ret;
}


// return values of save and load: -1 = cancel, 0 = error, 1 = success
int File::save(int who, Board *b, Log *l)
{
    ofstream ofs;
    int ret;
    
    // ask for a filename
    string fname = ask_file_name("Save Game");

    if (!fname.length())
    {
	// canceled
	return -1;
    }
    
    // open the file
    ofs.open(fname.c_str());

    if (!ofs)
    {
	allegro_message("Could not open '%s' for writing.\n", fname.c_str());
	return 0;
    }

    ret = save_file(ofs, who, b, l);
    ofs.close();

    return ret;
}

int File::load(int &who, Board *b, Log *l)
{
    ifstream ifs;
    int ret;
    
    // ask for a filename
    string fname = ask_file_name("Load Game");

    if (!fname.length())
    {
	// canceled
	return -1;
    }
    
    // open the file
    ifs.open(fname.c_str());
    if (!ifs)
    {
	allegro_message("Could not open '%s' for reading.\n", fname.c_str());
	return 0;
    }
    
    ret = load_file(ifs, who, b, l);
    ifs.close();

    return ret;
}

int File::save_file(ofstream &ofs, int who, Board *b, Log *l)
{
    ofs << "OTHELLO_SAVEFILE" << endl;
    
    // write who
    ofs << who << endl;

    // write board
    b->write_to(ofs);
    
    // write log
    if (!l->write_to(ofs))
	return 0;

    return 1;
}

int File::load_file(ifstream &ifs, int &who, Board *b, Log *l)
{
    string dummy;

    ifs >> dummy;     // read first comment line
    ifs >> who;       // read who

    // read the board
    if (!(b->read_from(ifs)))
    {
//	cerr << "error reading board." << endl;
	return 0;
    }

    // read the log
    if (!l->read_from(ifs))
    {
//	cerr << "error reading log." << endl;
	return 0;
    }

    return 1;
}

void File::save_settings()
{
    // just to be sure: use allegro's push_config_state and
    // pop_config_state to make it so that the global and user allegro.cfg
    // can still be used. I just want to use othello.cfg for game settings,
    // not allegro settings.

    push_config_state();
    set_config_file(configfilename.c_str());

    // save computer / human players, ply level
    set_config_int("othello", "computer_player_1", computer_player[0]);
    set_config_int("othello", "computer_player_2", computer_player[1]);
    set_config_int("othello", "ply_level",         ply);
    set_config_int("othello", "who_starts",        who_starts);
    set_config_int("othello", "animations",        animations);
    
    // flush file to disk
    flush_config_file();
    
    // restore original config state
    pop_config_state();
}

void File::load_settings()
{
    // see above: push config state to avoid mangling with the main allegro
    // config file.
    
    push_config_state();
    set_config_file(configfilename.c_str());

    // load settings
    computer_player[0] = get_config_int("othello", "computer_player_1", 0);
    computer_player[1] = get_config_int("othello", "computer_player_2", 1);
    ply                = get_config_int("othello", "ply_level", 4);
    
    // who variable: initialize from config file too. This cannot be set
    // from the options menu (yet), but is very practical to override who
    // starts if you want to play against backward othello programs like my
    // own that start with the wrong stone :-)
    who_starts         = get_config_int("othello", "who_starts", 1);
    animations         = get_config_int("othello", "animations", 1);
    
    // pop config state
    pop_config_state();
}

string File::ask_file_name(char const *msg)
{
    char buf[1024];
    string str;

    strcpy(buf, othello_dir().c_str());
    
    if (file_select_ex(msg, buf, "sav;SAV", 1024, 400, 400))
    {
	str = buf;
	return str;
    }

    // we have a problem: no valid filename was selected.
    str = "";
    return str;
}
