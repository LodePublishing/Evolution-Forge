#include "../sdl/dc.hpp"

class ProgressBar
{
	public:
		ProgressBar(const Rect& area):bar(area), p(1), h(0) {};
		~ProgressBar() {};
		void draw(DC* dc, const unsigned int dp);
		void draw(DC* dc, const unsigned int dp, const std::string& text);
	private:
		Rect bar;
		unsigned int p;
		unsigned int h;
};

