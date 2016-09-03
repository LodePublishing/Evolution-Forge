#ifndef _GUI_DATABASE_ENTRY_HPP
#define _GUI_DATABASE_ENTRY_HPP

#include "../ui/button.hpp"
#include "../core/goalentry.hpp"

enum eDataEntry
{
	DATA_RACE_ENTRY,
	DATA_GOAL_ENTRY,
	DATA_BO_ENTRY
};

class DataBaseEntry : public UI_Object
{
	public:
		DataBaseEntry(UI_Object* entry_parent, const Rect& entry_rect, const Size distance_bottom_right, const ePositionMode position_mode, const std::string& text, const eDataEntry data_entry_type, const unsigned int entry_count = 0);
		~DataBaseEntry();
		
		const bool isClicked() const;
		const bool isCurrentlyActivated() const;
		const bool isGoal() const;
		const bool isCurrentlyHighlighted() const;
		void forceUnpress();

		void check(const bool do_check = true);

		void draw() const;
		void process();
		void reloadOriginalSize();
		
		unsigned int myGoal;
		unsigned int myBO;
		unsigned int myRace;
		const eDataEntry getEntryType() const;

		bool isChecked() const;
		bool isJustChecked() const;

		void reloadStrings();
	private:
		UI_Button* entry;
		UI_Button* checkButton;
		eDataEntry entryType;

		unsigned int entryCount;
		std::string entryText;
};

inline bool DataBaseEntry::isChecked() const {
	return(checkButton->isCurrentlyActivated());
}

inline bool DataBaseEntry::isJustChecked() const {
	return(checkButton->isLeftClicked());
}

inline void DataBaseEntry::forceUnpress() {
	entry->forceUnpress();
}

inline const bool DataBaseEntry::isClicked() const {
	return(entry->isLeftClicked());
}

inline const bool DataBaseEntry::isCurrentlyActivated() const {
	return(entry->isCurrentlyActivated());
}

inline const bool DataBaseEntry::isCurrentlyHighlighted() const {
	return(entry->isCurrentlyHighlighted());
}

inline const eDataEntry DataBaseEntry::getEntryType() const {
	return(entryType);
}

#endif
// TODO irgendwie edit mode... oder so ein Augensymbol, mit einem Click auf ein eigenes Game-Tab
