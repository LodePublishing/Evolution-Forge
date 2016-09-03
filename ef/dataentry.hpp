#ifndef _GUI_DATABASE_ENTRY_HPP
#define _GUI_DATABASE_ENTRY_HPP

#include "../ui/button.hpp"
#include "../core/goalentry.hpp"

class DataBaseEntry : public UI_Object
{
	public:
		DataBaseEntry(UI_Object* entry_parent, const Rect& entry_rect, const Size distance_bottom_right, const ePositionMode position_mode, const std::string& text, const bool is_goal = false, const unsigned int bo_count = 0);
		~DataBaseEntry();
		
		const bool isClicked() const;
		const bool isMarked() const;
		const bool isGoal() const;
		const bool isCurrentlyHighlighted() const;
		void forceUnpress();

		void draw(DC* dc) const;
		void process();
		void reloadOriginalSize();
		unsigned int myGoal;
		unsigned int myBO;
	private:
		UI_Button* entry;
		UI_Button* checkButton;
		bool goal;
};

inline void DataBaseEntry::forceUnpress() {
	entry->forceUnpress();
}

inline const bool DataBaseEntry::isClicked() const {
	return(entry->isLeftClicked());
}

inline const bool DataBaseEntry::isMarked() const {
	return(entry->isCurrentlyActivated());
}

inline const bool DataBaseEntry::isCurrentlyHighlighted() const {
	return(entry->isCurrentlyHighlighted());
}

inline const bool DataBaseEntry::isGoal() const {
	return(goal);
}

#endif
// TODO irgendwie edit mode... oder so ein Augensymbol, mit einem Click auf ein eigenes Game-Tab
