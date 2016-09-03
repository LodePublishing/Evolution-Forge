#ifndef _UI_TITLE_GROUP_HPP
#define _UI_TITLE_GROUP_HPP

#include "object.hpp"
#include "statictext.hpp"
#include <string>

// TODO eString erlauben!

// simplified window class, maybe inheritate UI_Window from UI_Group...
class UI_Group : public UI_Object
{
	public:
		UI_Group(UI_Object* group_parent, Point position, const eString txt=NULL_STRING);
		~UI_Group();
		UI_Group(const UI_Group& object);
		UI_Group& operator=(const UI_Group& object);

		void draw(DC* dc) const;
		void process();
		UI_Object* checkToolTip();
		UI_Object* checkHighlight();

		void reloadStrings();

		void calculateBoxSize(const bool horizontal=false);
		void calculateSameWidthOfButtons(const bool horizontal=false);
	private:
		UI_StaticText* title;
	protected:		
		Size boxSize;
		unsigned int maxWidth;
};

#endif

