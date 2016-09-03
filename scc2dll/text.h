#ifndef __TEXT_H
#define __TEXT_H

#include "main.h"

// this is mainly for for io to display some of the output
const char error_message[ERROR_MESSAGES][21]=
{
	"OK",
	"enough Minerals",
	"enough Gas",
	"supply satisfied",
	"prerequisite built",
	"facility availible",
	"timeout",
	"unknown problem"
};


char error_small_message[ERROR_MESSAGES]=
{
	'O','M','G','S','P','F','T','U'
};

#endif

