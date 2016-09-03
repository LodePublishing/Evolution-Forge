// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------
#include "misc.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../core/configuration.hpp"

void toErrorLog(const std::string& msg) {
	std::cout << msg << "\n";
}

void toErrorLog(int msg)
{
	std::cout << msg << "\n";
}

void toInitLog(const std::string& msg)
{
	std::cout << msg << "\n";
}

void toInitLog(int msg)
{
	std::cout << msg << "\n";
}

#include <dirent.h>
#include <sys/types.h>

#ifdef __linux__
#else 
#include <windows.h>
#endif

std::list<std::string> findFiles(const std::string& directory1, const std::string& directory2, const std::string& directory3, const std::string&   directory4)
{
	std::list<std::string> fileList;
	std::ostringstream os;
	os.str("");
#ifdef __linux__
	DIR *dir;
	struct dirent *entry;
	if(directory4!="")
		os << directory1 << "/" << directory2 << "/" << directory3 << "/" << directory4;
	else if(directory3!="")
		os << directory1 << "/" << directory2 << "/" << directory3;
	else if(directory2!="")
		os << directory1 << "/" << directory2;
	else if(directory1!="")
		os << directory1;

	if ((dir = opendir(os.str().c_str())) == NULL)
		toErrorLog("ERROR (findFiles()): Cannot open directory " + os.str());
	else 
	{
		while ((entry = readdir(dir)) != NULL)
			fileList.push_back(os.str() + "/" + entry->d_name);
		closedir(dir);
	}
#elif __WIN32__
	WIN32_FIND_DATA dir;
	HANDLE fhandle;
	os << directory1 << "\\" << directory2 << "\\" << directory3 << "\\" << directory4 << "\\" << "*.*";
	if ((fhandle=FindFirstFile(os.str().c_str(), &dir)) !=INVALID_HANDLE_VALUE)
	{
		do
			fileList.push_back(directory1 + "\\" + directory2 + "\\" + directory3 + "\\" + directory4 + "\\" + dir.cFileName);
		while(FindNextFile(fhandle, &dir));
	} else
		toErrorLog("ERROR (findFiles()): Cannot load " + directory1 + "\\" + directory2 + "\\" + directory3 + "\\" + directory4 + ".");
	FindClose(fhandle);
#endif
	return fileList;
}


const bool checkStreamIsOpen(std::ifstream& stream, const std::string function_name, const std::string& file_name)
{
	if(!stream.is_open())
	{
		toErrorLog("ERROR (" + function_name + "()): File " + file_name + " not found or cannot be opened => canceling request.");
		return(false);
	}
	return(true);
}

const bool checkStreamIsOpen(std::ofstream& stream, const std::string function_name, const std::string& file_name)
{
	if(!stream.is_open())
	{
		toErrorLog("ERROR (" + function_name + "()): Could not create file " + file_name + " (write protection? disk space?) => canceling request.");
		return(false);
	}
	return(true);
}

const bool checkStreamForFailure(std::ifstream& stream, const std::string function_name, const std::string& file_name)
{
	if(stream.bad())
	{
		toErrorLog("ERROR (" + function_name + "()): Fatal error while accessing file " + file_name + " => canceling request.");
		return(false);
	}
	if(stream.fail())
	{
		stream.clear(stream.rdstate() & ~std::ios::failbit);
		toErrorLog("WARNING (" + function_name + "()): Error while reading from file " + file_name + " => trying to continue.");
	}
	return(true);
}

const bool checkStreamForFailure(std::ofstream& stream, const std::string function_name, const std::string& file_name)
{
	if(stream.bad())
	{
		toErrorLog("ERROR (" + function_name + "()): Fatal error while accessing file " + file_name + " => canceling request.");
		return(false);
	}
	if(stream.fail())
	{
		stream.clear(stream.rdstate() & ~std::ios::failbit);
		toErrorLog("WARNING (" + function_name + "()): Error while writing to file " + file_name + " => trying to continue.");
	}
	return(true);
}

const bool parse_line(std::string& text, std::list<std::string>& words)
{
	size_t n = text.length();
	size_t stop;
	bool in_parentheses=false;
	// " gefunden? ignoriere alle Sonderzeichen bis naechstes "
	size_t start = text.find_first_not_of("\t ");
	while (start!=std::string::npos) 
	{
		if(text[start]=='\"')
		{
			in_parentheses=true;
			++start;
		}
	
		if(in_parentheses)
			stop = text.find_first_of("\"", start);
		else 
			stop = text.find_first_of(",\t\" =", start);
		in_parentheses=false;
		if(stop==std::string::npos)
			stop = n;
		std::string word = text.substr(start, stop - start);
		if(word == "@END")
			return(false);
		words.push_back(word);
	   
		start = text.find_first_not_of(",\t =", stop+1);
	}
	return(true);
}

const bool parse_block_list(std::ifstream& stream, std::list<std::list<std::string> >& block)
{
	char line[1024];
	std::string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail())
			stream.clear(stream.rdstate() & ~std::ios::failbit);
		text=line;
		size_t start=text.find_first_not_of("\t\" ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		std::list<std::string> words;
		if(!parse_line(text, words)) 
// => @END was found
			return(true);
		block.push_back(words);
	}
// end of file but no @END was found!
	return(false);
}

// parse until the next "@END", if there is no @END => return false
const bool parse_block_map(std::ifstream& stream, std::map<std::string, std::list<std::string> >& block)
{
	char line[1024];
	std::string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail())
			stream.clear(stream.rdstate() & ~std::ios::failbit);
		text=line;
		size_t start=text.find_first_not_of("\t\" ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		
		std::list<std::string> words;
		if(!parse_line(text, words))
			return(true);
		std::string word = words.front();
		block.insert(std::pair<std::string, std::list<std::string> >(word, words));
	}
	return(false);
}
/*
void parse_2nd_block(std::ifstream& stream, std::map<std::string, std::map<std::string, std::list<std::string> > >& block)
{
	char line[1024];
	std::string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail())
			stream.clear(stream.rdstate() & ~std::ios::failbit);
		text=line;
//		if(text.find("@END")!=std::string::npos) 
//			return;
		size_t start=text.find_first_not_of("\t ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		std::map<std::string, std::list<std::string> > words;
		parse_block(stream, words);
		block.insert(std::pair<std::string, std::map<std::string, std::list<std::string> > > (text.substr(start), words));
	}
}*/

const std::string formatTime(const int time, const unsigned int speed)
{
	std::ostringstream os;
	os.str("");
	if(time < 0) os << "- ";
	int my_time = time / (6+speed*3);
	os << my_time/60 << ":" << std::setfill('0') << std::setw(2) << my_time%60;
	return(os.str());
}

// -----------------------------------------------
// ------  END PARSING TOOLS AND ERROR LOGS ------
// -----------------------------------------------

const eLanguage getLanguageSubDataEntry(const std::string& item)
{
	if(item=="@ENGLISH") return(ENGLISH_LANGUAGE);else
	if(item=="@GERMAN") return(GERMAN_LANGUAGE);else
	if(item=="@ITALIAN") return(ITALIAN_LANGUAGE);else
	if(item=="@PORTUGESE") return(PORTUGESE_LANGUAGE);else
	if(item=="@DUTCH") return(DUTCH_LANGUAGE);else
	if(item=="@FINNISH") return(FINNISH_LANGUAGE);else
	if(item=="@GREEK") return(GREEK_LANGUAGE);else
	if(item=="@FRENCH") return(FRENCH_LANGUAGE);else
	if(item=="@SPANISH") return(SPANISH_LANGUAGE);else
	if(item=="@POLSKI") return(POLSKI_LANGUAGE);else
	if(item=="@KOREAN") return(KOREAN_LANGUAGE);else
	if(item=="@CHINESE") return(CHINESE_LANGUAGE);else
	if(item=="@RUSSIAN") return(RUSSIAN_LANGUAGE);else
	return(ZERO_LANGUAGE);
}

const bool loadStringFile(const std::string& string_file, const std::vector<std::string>& string_identifier, std::vector< std::vector<std::string> >& string_list, std::vector<bool>& language_loaded, const unsigned int max_strings)
{
	if((string_file.substr(string_file.size()-2,2) == "..") ||(string_file.substr(string_file.size()-1,1) == "."))
		return(true);
	
	bool string_mode = false;
	bool language_mode = false;
	eLanguage current_language = ZERO_LANGUAGE;

	std::ifstream pFile(string_file.c_str());

	if(!checkStreamIsOpen(pFile, "loadStringFile", string_file))
		return(false);

//	toInitLog("* " + lookUpString(START_LOADING_STRING) + " " + string_file);
//problem: Stringfile ist ja noch nicht geladen :>
	toInitLog("* Loading " + string_file);
	bool found_any_language_block = false;
	bool found_language_block[MAX_LANGUAGES];
	for(unsigned int i = MAX_LANGUAGES; i--;)
		found_language_block[i] = false;
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "loadStringFile", string_file))
			return(false);
		
		std::string text = line;
		size_t start = text.find_first_not_of("\t ");
		if((start == std::string::npos) || (text[0] == '#') || (text[0] == '\0'))
			continue; // ignore line
		size_t stop = text.find_first_of("\t ", start);
		if(stop == std::string::npos) 
			stop = text.size();
		std::string index = text.substr(start, stop);
		if(!string_mode)
		{
			string_mode = (index == "@STRINGS");
			if(!string_mode)
			{
				if(index == "@END")
					toErrorLog("WARNING (loadStringFile()): Lonely @END => ignoring line.");
				else
					toErrorLog("WARNING (loadStringFile()): Line '" + index + "' is outside a block but is not marked as comment => ignoring line.");
			} else
				language_mode = true;
		}  else
		if((language_mode) && (index == "@END"))
		{
			current_language = ZERO_LANGUAGE;
			language_mode = false;
		} else		
		if(string_mode && (index == "@END"))
		{
			string_mode = false;
		} else
		if((language_mode) && (current_language == ZERO_LANGUAGE))
		{
			current_language = getLanguageSubDataEntry(index);
			if(current_language==ZERO_LANGUAGE) {
				toErrorLog("ERROR (loadStringFile()): Invalid language entry '" + index + "'.");return(false);
			} else if(language_loaded[current_language]) {
				toErrorLog("ERROR (loadStringFile()): Language '" + index + "' already initialized.");return(false);
			}
			else
			{
				found_language_block[current_language] = true;
				found_any_language_block = true;
				string_list[current_language].resize(max_strings);
			}
		}
		// => hat nur 1 Ebene => Position festgestellt!
		else if((language_mode) && (current_language != ZERO_LANGUAGE))
		{
			std::map<std::string, std::list<std::string> >::iterator i;
			std::map<std::string, std::list<std::string> > block;
			pFile.seekg(old_pos);
			if(!parse_block_map(pFile, block)) {
				toErrorLog("WARNING (loadStringFile()): No concluding @END for @STRINGS block was found in file " + string_file + " => trying to parse what we have so far.");
			}
			for(unsigned int j = 0; j < max_strings; j++)
			{
				if(string_identifier[j] == "")
				{
#ifdef _SCC_DEBUG
/*					std::ostringstream os;
					os << "DEBUG (loadStringFile()): Entry number " << j << " not found in file " << string_file << ".";
					toErrorLog(os.str());*/
#endif
				}
				else if((i=block.find(string_identifier[j])) != block.end())
				{
					i->second.pop_front();
					string_list[current_language][j] = i->second.front();
					block.erase(i);
				} 
				else
					toErrorLog("WARNING (loadStringFile()): Entry " + string_identifier[j] + " not found in file " + string_file + ".");
			}
			for(std::map<std::string, std::list<std::string> >::iterator j = block.begin(); j != block.end(); ++j)
				if(j->first != "")
					toErrorLog("WARNING (loadStringFile()): Entry " + j->first + " not identified in file " + string_file + ".");
					
			current_language = ZERO_LANGUAGE;
			// TODO nicht gefundene Eintraege bemaengeln
		} 
			old_pos = pFile.tellg();
	} // end while

	if(!found_any_language_block)
	{
		toErrorLog("ERROR (loadStringFile()): No language block (@ENGLISH, @GERMAN etc.) was found in file " + string_file + " => ignoring file.");
		return(false);
	}
	for(unsigned int i = MAX_LANGUAGES; i--;)
		if(found_language_block[i])
			language_loaded[i] = true;
	return(true);
}

