#ifndef _UI_GROUP_HPP
#define _UI_GROUP_HPP

#include "object.hpp"
#include <string>

// simplified window class, maybe inheritate UI_Window from UI_Group...
class UI_Group : public UI_Object
{
	public:
		UI_Group(UI_Object* parent, Rect rect, const string& title);
		~UI_Group();

		void draw(DC* dc) const;
		void process();
		UI_Object* checkHighlight();
	private:
		string title;
};

#endif

