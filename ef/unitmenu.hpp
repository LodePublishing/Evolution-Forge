#ifndef _GUI_UNITMENU_HPP
#define _GUI_UNITMENU_HPP

#include "../ui/button.hpp"

#include "../core/anarace.hpp"
#include "../core/settings.hpp"

class UnitMenuEntry : public UI_Button
{
	public:
        UnitMenuEntry(UI_Object* parent, Rect rahmen, Rect maxSize, string unit);
        ~UnitMenuEntry();
        void updateText(string utext);
        void process();
        void draw(DC* dc) const;
        eUnitType type;
    private:
};


class UnitMenu : public UI_Object
{
    public:
        UnitMenu(UI_Object* parent, ANARACE* anarace, Rect rect);
        ~UnitMenu();
		int pressedUnit;
        void resetData();
        void process();
        void draw(DC* dc) const;
        const int getMarkedUnit() const;
        const bool hasChanged();
		const int getHeight() const;
    private:
		int height;
        bool changed;
        UnitMenuEntry* unitEntry[UNIT_TYPE_COUNT];
        int markedUnit;
        ANARACE* anarace;
        int goalListOpened;
        int currentUnitType;
        int goalFileListOpened;
		UI_Button* addGoalButton;
};

#endif // _GUI_UNITMENU_HPP

