#include "bitmap.hpp"

Bitmap::Bitmap(const string& name):Surface(SDL_LoadBMP(name.c_str())) 
{
}

Bitmap::Bitmap(const Bitmap& bmp):Surface(bmp) 
{
}

Bitmap::Bitmap():Surface() 
{
}

Bitmap::~Bitmap() 
{
}

