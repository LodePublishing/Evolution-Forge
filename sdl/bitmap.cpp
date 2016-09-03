#include "bitmap.hpp"
#include "../core/defs.hpp"
//#include <sstream>

Bitmap::Bitmap(const std::string& name) : 
	Surface(SDL_LoadBMP(name.c_str())) 
{
	if(((Surface*)this)->getSurface()==NULL)
	{
	//	ostringstream os;
//		os << "Could not load Bitmap " + name;
//		toLog(os.str());
		toLog("Could not load Bitmap..."); // TODO
	}
}

Bitmap::Bitmap(const Bitmap& bmp):Surface(bmp) 
{ }

Bitmap::Bitmap():Surface() 
{ }

Bitmap::~Bitmap() 
{ }

