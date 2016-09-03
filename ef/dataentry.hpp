#ifndef _GUI_DATABASE_ENTRY_HPP
#define _GUI_DATABASE_ENTRY_HPP

#include "../ui/button.hpp"
#include "../core/goalentry.hpp"

class DataBaseEntry : public UI_Object
{
	public:
		DataBaseEntry(UI_Object* entry_parent, Rect entry_rect, Size distance_bottom_right, ePositionMode position_mode, std::string text, const bool is_goal);
		~DataBaseEntry();
		const bool isClicked() const;
		const bool isMarked() const;
		const bool isGoal() const;
		const bool isCurrentlyHighlighted() const;
		void forceUnpress();

		void draw(DC* dc) const;
		void process();
		void reloadOriginalSize();
		const GOAL_ENTRY* myGoal;
		unsigned int myBO;
	private:
		UI_Button* entry;
		UI_Button* checkButton;
		bool goal;
};

inline void DataBaseEntry::forceUnpress()
{
	entry->forceUnpress();
}

inline const bool DataBaseEntry::isClicked() const
{
	return(entry->isLeftClicked());
}

inline const bool DataBaseEntry::isMarked() const
{
	return(entry->isCurrentlyActivated());
}

inline const bool DataBaseEntry::isCurrentlyHighlighted() const
{
	return(entry->isCurrentlyHighlighted());
}

inline const bool DataBaseEntry::isGoal() const
{
	return(goal);
}

#endif
// TODO irgendwie edit mode... oder so ein Augensymbol, mit einem Click auf ein eigenes Game-Tab
