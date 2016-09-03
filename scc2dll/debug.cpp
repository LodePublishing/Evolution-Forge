#include "debug.h"
#include <fstream>

void toLog(const string& msg)
{
    ofstream pFile("error.log");
	pFile << msg.c_str();
};

