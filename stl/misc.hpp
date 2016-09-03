#ifndef _STL_MISC_HPP_
#define _STL_MISC_HPP_

// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------
#include <list>
#include <map>
#include <string>

extern void toLog(const std::string& msg);
extern void toLog(int msg);
extern void parse_line(std::string& text, std::list<std::string>& words);
extern void parse_block(std::ifstream& stream, std::map<std::string, std::list<std::string> >& block);
extern void parse_block(std::ifstream& stream, std::list<std::list<std::string> >& block);
extern void parse_2nd_block(std::ifstream& stream, std::map<std::string, std::map<std::string, std::list<std::string> > >& block);

const std::string formatTime(const int time);
const std::string formatTime2(const int time);

// -----------------------------------------------
// ------  END PARSING TOOLS AND ERROR LOGS ------
// -----------------------------------------------

#endif

