#include "exitinfo.hpp"
#include "object.hpp"

UI_ExitInfo::UI_ExitInfo() :
	smoothExit(false)
{}

UI_ExitInfo::~UI_ExitInfo()
{
	if(UI_Object::dc!=NULL)
	{
		delete UI_Object::dc;
		UI_Object::dc = NULL;
	}

	if(!smoothExit)
		toInitLog("If you need help with the error message please post it on www.clawsoftware.de in the forums or contact me at ghoul@clawsoftware.de");
	else
		toInitLog("Exiting...");
}

UI_ExitInfo exitInfo;
