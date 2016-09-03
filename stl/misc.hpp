#ifndef _STL_MISC_HPP_
#define _STL_MISC_HPP_

// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------
#include <list>
#include <map>
#include <string>
#include <vector>

// ------ LANGUAGES ------
enum eLanguage
{
	ZERO_LANGUAGE,
	ENGLISH_LANGUAGE,
	GERMAN_LANGUAGE,
	ITALIAN_LANGUAGE,
	PORTUGESE_LANGUAGE,
	DUTCH_LANGUAGE,
	FINNISH_LANGUAGE,
	GREEK_LANGUAGE,
	FRENCH_LANGUAGE,
	SPANISH_LANGUAGE,
	POLSKI_LANGUAGE,
	KOREAN_LANGUAGE,
	CHINESE_LANGUAGE,
	RUSSIAN_LANGUAGE,

	MAX_LANGUAGES
};
// ------ END LANGUAGES ------


extern void toErrorLog(const std::string& msg);
extern void toErrorLog(int msg);
extern void toInitLog(const std::string& msg);
extern void toInitLog(int msg);
extern std::list<std::string> findFiles(const std::string& directory1, const std::string& directory2, const std::string& directory3);
extern const bool checkStreamIsOpen(std::ifstream& stream, const std::string function_name, const std::string& file_name);
extern const bool checkStreamIsOpen(std::ofstream& stream, const std::string function_name, const std::string& file_name);
extern const bool checkStreamForFailure(std::ifstream& stream, const std::string function_name, const std::string& file_name);
extern const bool checkStreamForFailure(std::ofstream& stream, const std::string function_name, const std::string& file_name);
extern const bool parse_line(std::string& text, std::list<std::string>& words);
extern const bool parse_block_map(std::ifstream& stream, std::map<std::string, std::list<std::string> >& block);
extern const bool parse_block_list(std::ifstream& stream, std::list<std::list<std::string> >& block);
//extern void parse_2nd_block(std::ifstream& stream, std::map<std::string, std::map<std::string, std::list<std::string> > >& block);

extern const std::string formatTime(const int time, const unsigned int speed);

extern const eLanguage getLanguageSubDataEntry(const std::string& item);
extern const bool loadStringFile(const std::string& string_file, const std::vector<std::string>& string_identifier, std::vector< std::vector<std::string> >& string_list, std::vector<bool>& language_loaded, const unsigned int max_strings);
// -----------------------------------------------
// ------  END PARSING TOOLS AND ERROR LOGS ------
// -----------------------------------------------

#endif

