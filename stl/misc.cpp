// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------
#include "misc.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

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

void parse_line(std::string& text, std::list<std::string>& words)
{
	size_t n = text.length();
	size_t stop;
	bool inParantheses=false;
	// " gefunden? ignoriere alle Sonderzeichen bis naechstes "
	size_t start = text.find_first_not_of("\t ");
	while (start!=std::string::npos) 
	{
		if(text[start]=='\"')
		{
			inParantheses=true;
			++start;
		}
	
		if(inParantheses)
			stop = text.find_first_of("\"", start);
		else 
			stop = text.find_first_of(",\t\" =", start);
		inParantheses=false;
		if(stop==std::string::npos)
			stop = n;
		std::string bla=text.substr(start, stop - start);
		words.push_back(bla);
	   
		start = text.find_first_not_of(",\t =", stop+1);
	}
}

void parse_block(std::ifstream& stream, std::list<std::list<std::string> >& block)
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
		parse_line(text, words);
		block.push_back(words);
	}
}

void parse_block(std::ifstream& stream, std::map<std::string, std::list<std::string> >& block)
{
	char line[1024];
	std::string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail())
			stream.clear(stream.rdstate() & ~std::ios::failbit);
		text=line;
		if(text.find("@END")!=std::string::npos) return;
		size_t start=text.find_first_not_of("\t\" ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		
		std::list<std::string> words;
		parse_line(text, words);
		std::string bla=words.front();
//		std::string bla=text.substr(start);
		block.insert(std::pair<std::string, std::list<std::string> >(bla, words));
	}
}

void parse_2nd_block(std::ifstream& stream, std::map<std::string, std::map<std::string, std::list<std::string> > >& block)
{
	char line[1024];
	std::string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail())
			stream.clear(stream.rdstate() & ~std::ios::failbit);
		text=line;
		if(text.find("@END")!=std::string::npos) return;
		size_t start=text.find_first_not_of("\t ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		std::map<std::string, std::list<std::string> > words;
		parse_block(stream, words);
		block.insert(std::pair<std::string, std::map<std::string, std::list<std::string> > > (text.substr(start), words));
	}
}

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


