#ifndef __UTIL_H
#define __UTIL_H

//#include "wx/wx.h" ?

inline void xchg(int& a, int& b)
{
	a^=b;b^=a;a^=b;
};

inline void move(int& pos, int target)
{
        if(target>pos)
                pos+=1+(target-pos)/2;
        else if(target<pos)
                pos-=1+(pos-target)/2;
};

void move(int& x, int sx, int tx);

#endif

