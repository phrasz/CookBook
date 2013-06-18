#ifndef _OTHELLO_FILE_H_
#define _OTHELLO_FILE_H_

#include <string>
#include <iosfwd>
#include "globals.h"

// file related functions together in a class
// - function to determine a directory where we can read / write config and
//   save files.
// - log moves to a log file is handled by the Log class now
//   we just write the log into the save files.
// - save / load options from a config file
// - save / load games to/from file (can be config file too)
// - optional : high score file (requires name entering, really)

// naming is a little dangerous, if it goes wrong rename the class.
class File
{
public:
    File();
    ~File();

    string othello_dir();     // platform dependant

    // save/load functions
    int save(int who, Board *b, Log *l);    // save current game
    int load(int &who, Board *b, Log *l);   // load game
    int save_file(ofstream &ofs, int who, Board *b, Log *l);  
    int load_file(ifstream &ifs, int &who, Board *b, Log *l);
 
    // settings
    void save_settings();             // save settings in config file
    void load_settings();             // load settings from config file

    string configfilename;
    
private:
    string ask_file_name(const char *msg);   // ask user for a filename

};

#endif
