#ifndef _SDL_SIZE_HPP
#define _SDL_SIZE_HPP

#include "../core/defs.hpp"

class Size
{
	public:

		Size();
		Size(const Size& size);
		Size(const unsigned int w, const unsigned int h);
		Size& operator=(const Size& size);
		const bool operator==(const Size& size) const;
		const bool operator!=(const Size& size) const;
		const Size operator+(const Size& size) const;
		const Size operator-(const Size& size) const;
		const Size operator/(const unsigned int d) const;
		void SetWidth(const unsigned int w);
		void SetHeight(const unsigned int h);
		const unsigned int GetWidth() const;
		const unsigned int GetHeight() const;
		void move(const Size& startSize, const Size& targetSize);
		static void mv(unsigned int& x, const unsigned int sx, const unsigned int tx);
	private:
		unsigned int width;
		unsigned int height;

};

inline Size::Size() : width(0), height(0)
{}

inline Size::Size(const Size& size) :
	width(size.width),
	height(size.height)
{ 
/*#ifdef _SCC_DEBUG
	if((width > 1280) || (height > 1024))
	{
		toLog("SIZE ERROR: width/height out of range");
	}
#endif*/
}

inline Size::Size(const unsigned int w, const unsigned int h) :
	width(w),
	height(h)
{
/*#ifdef _SCC_DEBUG
	if((width > 1280) || (height > 1024))
	{
		toLog("SIZE ERROR: width/height out of range");
	}
#endif*/
}

inline const Size Size::operator-(const Size& size) const 
{
#ifdef _SCC_DEBUG
	if((size.width > width) || (size.height > height))
	{
		toLog("RESIZE ERROR: Negative size is not allowed");
		return(Size(0,0));
	}
#endif
	return Size(width - size.width, height - size.height);
}

inline Size& Size::operator=(const Size& size)
{
	width = size.width;
	height = size.height;
	return(*this);
}

inline const bool Size::operator==(const Size& size) const {
	return ( (width == size.width) && (height == size.height) );
}
		
inline const bool Size::operator!=(const Size& size) const {
	return ( !(*this == size));
}

inline const Size Size::operator+(const Size& size) const {
	return Size(width + size.width, height + size.height);
}
		
inline const Size Size::operator/(const unsigned int d) const
{
	return(Size(width/d, height/d));
}

inline void Size::SetWidth(const unsigned int w) 
{
/*			#ifdef _SCC_DEBUG
			if(w > 1280)
				toLog("SIZE ERROR: width out of range");
			#endif*/
	width = w;
}
		
inline void Size::SetHeight(const unsigned int h) 
{
#ifdef _SCC_DEBUG
	if(h > 1024)
	{
		toLog("SIZE ERROR: height out of range");
		return;
	}
#endif
	height = h;
}

inline const unsigned int Size::GetWidth() const {
	return width;
}
																				
inline const unsigned int Size::GetHeight() const {
	return height;
}

#endif // _SDL_SIZE_HPP

