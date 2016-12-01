#ifndef _UI_SIZE_HPP
#define _UI_SIZE_HPP

#ifdef _SCC_DEBUG
        #include "../stl/misc.hpp"
#endif

class Size
{
	public:

		Size();
		Size(const Size& size);
		Size(const unsigned int w, const unsigned int h);
		Size& operator=(const Size& size);
		const bool operator==(const Size& size) const;
		const bool operator<(const Size& size) const;
		const bool operator!=(const Size& size) const;
		const Size operator+(const Size& size) const;
		const Size operator-(const Size& size) const;
		const Size operator/(const unsigned int d) const;
		void setWidth(const unsigned int w);
		void setHeight(const unsigned int h);
		const unsigned int getWidth() const;
		const unsigned int getHeight() const;
		void move(const Size& startSize, const Size& targetSize);
		static const bool mv(unsigned int& x, const unsigned int sx, const unsigned int tx);
		static const bool mv2(unsigned int& x, const unsigned int sx, const unsigned int tx);
	private:
		unsigned int width;
		unsigned int height;

};

inline Size::Size() : width(0), height(0)
{}

inline Size::Size(const Size& size) :
	width(size.width),
	height(size.height)
{}

inline Size::Size(const unsigned int w, const unsigned int h) :
	width(w),
	height(h)
{
#ifdef _SCC_DEBUG
        if((w > 10000) || (h > 10000))
                toErrorLog("WARNING (Size::Size()): Coordinates out of boundary.");
#endif
}

inline const Size Size::operator-(const Size& size) const {
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

inline const bool Size::operator<(const Size& size) const {
	return ( (width < size.width) && (height < size.height) );
}	

inline const bool Size::operator!=(const Size& size) const {
	return ( !(*this == size));
}

inline const Size Size::operator+(const Size& size) const {
	return Size(width + size.width, height + size.height);
}
		
inline const Size Size::operator/(const unsigned int d) const {
	return(Size(width/d, height/d));
}

inline void Size::setWidth(const unsigned int w) {
	width = w;
#ifdef _SCC_DEBUG
        if((w > 10000))
                toErrorLog("WARNING (Size::setWidth()): Coordinates out of boundary.");
#endif
}
		
inline void Size::setHeight(const unsigned int h) {
	height = h;
#ifdef _SCC_DEBUG
        if((h > 10000))
                toErrorLog("WARNING (Size::setHeight()): Coordinates out of boundary.");
#endif
}

inline const unsigned int Size::getWidth() const {
	return width;
}
																				
inline const unsigned int Size::getHeight() const {
	return height;
}

#endif // _UI_SIZE_HPP

