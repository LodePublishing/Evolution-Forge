#ifndef _SDL_SIZE_HPP
#define _SDL_SIZE_HPP

#include "../core/defs.hpp"

class Size
{
	public:

		Size() :
		    width(0),
		    height(0)
		{ }                                                                                                                                                    
		Size(const Size& size);
		/*:
			width(size.width),
			height(size.height)
		{ }*/

		Size(const unsigned int w, const unsigned int h);/*
		    width(w),
		    height(h)
		{ }*/

		
		Size& operator=(const Size& size)
		{
			width = size.width;
		    height = size.height;
		    return(*this);
		}

		const bool operator==(const Size& size) const {
		    return ( (width == size.width) && (height == size.height) );
		}
		
		const bool operator!=(const Size& size) const {
		    return ( !(*this == size));
		}

		const Size operator+(const Size& size) const {
		    return Size(width + size.width, height + size.height);
		}
		
		const Size operator-(const Size& size) const;/* {
		#ifdef _SCC_DEBUG
			if((size.width > width) || (size.height > height))
			{
				toLog("RESIZE ERROR: Negative size is not allowed");
				return(Size(0,0));
			}
		#endif
		    return Size(width - size.width, height - size.height);
		}*/

		const Size operator/(const unsigned int d) const
		{
			return(Size(width/d, height/d));
		}

/*		void SetSize(const unsigned int w, const unsigned int h)
		{
		    width = w;
		    height = h;
		}*/

		void SetWidth(const unsigned int w) {
		#ifdef _SCC_DEBUG
            if(w > 1280)
            {
                toLog("SIZE ERROR: width out of range");
            }
        #endif
		    width = w;
		}
		
		void SetHeight(const unsigned int h) {
        #ifdef _SCC_DEBUG
            if(h > 1024)
            {
                toLog("SIZE ERROR: height out of range");
            }
        #endif
		    height = h;
		}
                                                                                
		const unsigned int GetWidth() const {
		    return width;
		}
                                                                                
		const unsigned int GetHeight() const {
		    return height;
		}
	
		void move(const Size& startSize, const Size& targetSize)
		{
			mv(width, startSize.width, targetSize.width);
			mv(height, startSize.height, targetSize.height);
		}	

		static void mv(unsigned int& x, const unsigned int sx, const unsigned int tx);
	private:
		unsigned int width;
		unsigned int height;

};



#endif // _SDL_SIZE_HPP

