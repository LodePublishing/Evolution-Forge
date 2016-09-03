#include "util.h"

void move(int& x, const int sx, const int tx)
{
	x+=((x>tx)?-1:0)+((x<tx)?1:0);
	x+=((x>sx)?((x-sx)<(tx-sx)/2?(x-sx)/2:(tx-x)/2):((x-sx)>(tx-sx)/2?(x-sx)/2:(tx-x)/2));
};

