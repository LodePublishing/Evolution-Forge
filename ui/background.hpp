#ifndef _UI_BACKGROUND_HPP
#define _UI_BACKGROUND_HPP

#include "object.hpp"

class UI_BackGround : public UI_Object
{
	public:
		UI_BackGround();
		~UI_BackGround();
		void process();
		void object_info();
		void draw() const;
		void reloadOriginalSize();
	private:
};

#endif

