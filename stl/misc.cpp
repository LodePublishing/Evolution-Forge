// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------
#include "misc.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <dirent.h>
#include <sys/types.h>

#ifdef __linux__
#else 
#include <windows.h>
#endif


std::string lastMsg;
void toLog(const std::string& msg)
{
//	if(msg == lastMsg)
//		return;
	lastMsg=msg;
	std::cout << msg << "\n";
//	ofstream pFile("error.log", std::ios_base::app);
//	pFile << msg.c_str() << endl;
}

void toLog(int msg)
{
	std::cout << msg << "\n";
}

std::list<std::string> findFiles(const std::string& directory1, const std::string& directory2, const std::string& directory3)
{
	std::list<std::string> fileList;
	std::ostringstream os;
	os.str("");
#ifdef __linux__
	DIR *dir;
	struct dirent *entry;
	if(directory3!="")
		os << directory1 << "/" << directory2 << "/" << directory3;
	else if(directory2!="")
		os << directory1 << "/" << directory2;
	else if(directory1!="")
		os << directory1;

	if ((dir = opendir(os.str().c_str())) == NULL)
		toLog("ERROR (findFiles()): Cannot open directory " + os.str());
	else 
	{
		while ((entry = readdir(dir)) != NULL)
			fileList.push_back(os.str() + "/" + entry->d_name);
		closedir(dir);
	}
#elif __WIN32__
	WIN32_FIND_DATA dir;
	HANDLE fhandle;
	os << directory1 << "\\" << directory2 << "\\" << directory3 << "\\" << "*.*";
	if ((fhandle=FindFirstFile(os.str().c_str(), &dir)) !=INVALID_HANDLE_VALUE)
	{
		do
			fileList.push_back(directory1 + "\\" + directory2 + "\\" + directory3 + "\\" + dir.cFileName);
		while(FindNextFile(fhandle, &dir));
	} else
		toLog("ERROR (findFiles()): Cannot load " + directory1 + "\\" + directory2 + "\\" + directory3 + ".");
	FindClose(fhandle);
#endif
	return fileList;
}


const bool checkStreamIsOpen(std::ifstream& stream, const std::string function_name, const std::string& file_name)
{
	if(!stream.is_open())
	{
		toLog("ERROR (" + function_name + "()): File " + file_name + " not found or cannot be opened => canceling request.");
		return(false);
	}
	return(true);
}

const bool checkStreamIsOpen(std::ofstream& stream, const std::string function_name, const std::string& file_name)
{
	if(!stream.is_open())
	{
		toLog("ERROR (" + function_name + "()): Could not create file " + file_name + " (write protection? disk space?) => canceling request.");
		return(false);
	}
	return(true);
}

const bool checkStreamForFailure(std::ifstream& stream, const std::string function_name, const std::string& file_name)
{
	if(stream.bad())
	{
		toLog("ERROR (" + function_name + "()): Fatal error while accessing file " + file_name + " => canceling request.");
		return(false);
	}
	if(stream.fail())
	{
		stream.clear(stream.rdstate() & ~std::ios::failbit);
		toLog("WARNING (" + function_name + "()): Error while reading from file " + file_name + " => trying to continue.");
	}
	return(true);
}

const bool checkStreamForFailure(std::ofstream& stream, const std::string function_name, const std::string& file_name)
{
	if(stream.bad())
	{
		toLog("ERROR (" + function_name + "()): Fatal error while accessing file " + file_name + " => canceling request.");
		return(false);
	}
	if(stream.fail())
	{
		stream.clear(stream.rdstate() & ~std::ios::failbit);
		toLog("WARNING (" + function_name + "()): Error while writing to file " + file_name + " => trying to continue.");
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

const std::string formatTime(const int time)
{
	std::ostringstream os;
	os.str("");
	if(time < 0) os << "- ";
	os << time/60 << ":" << std::setfill('0') << std::setw(2) << time%60;
	return(os.str());
}

const std::string formatTime2(const int time)
{
	std::ostringstream os;
	os.str("");
	if(time < 0) os << "- ";
	os << time/60 << "_" << std::setfill('0') << std::setw(2) << time%60;
	return(os.str());
}


// -----------------------------------------------
// ------  END PARSING TOOLS AND ERROR LOGS ------
// -----------------------------------------------


