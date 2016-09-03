#include "../core/settings.hpp"
#include "endrundialog.hpp"
#include "button.hpp"
#include <sstream>

// ISO-LEVEL 2 

UI_EndRunDialog::UI_EndRunDialog(UI_Object* edit_parent, const std::string& name_proposal) :
	UI_EditField(edit_parent, NULL, ENDRUN_DIALOG_TITLE_STRING, ENDRUN_QUESTION_STRING, ENDRUN_SAVE_AND_CONTINUE_STRING, ENDRUN_DONT_SAVE_AND_CONTINUE_STRING, name_proposal),
	autoSaveRuns(new UI_CheckButton(this, Rect(Point(25, 80), Size(200, 15)), SETTING_AUTO_SAVE_RUNS_STRING, SETTING_AUTO_SAVE_RUNS_TOOLTIP_STRING, configuration.isAutoSaveRuns()))
{ }

UI_EndRunDialog::~UI_EndRunDialog()
{
	if(!isCanceled())
	{
		lastString = getString();
		configuration.setAutoSaveRuns ( autoSaveRuns->isChecked() );
	}
	delete autoSaveRuns;
}

UI_EndRunDialog::UI_EndRunDialog(const UI_EndRunDialog& object) :
	UI_EditField((UI_EditField)object),
	autoSaveRuns(new UI_CheckButton(*object.autoSaveRuns))
{ }

UI_EndRunDialog& UI_EndRunDialog::operator=(const UI_EndRunDialog& object)
{
	(UI_EditField)(*this) = (UI_EditField)object;
	delete autoSaveRuns;
	autoSaveRuns = new UI_CheckButton(*object.autoSaveRuns);
	return(*this);
}

std::string& UI_EndRunDialog::getLastString()
{
	return lastString;
}

void UI_EndRunDialog::process()
{
	UI_EditField::process();
}

void UI_EndRunDialog::draw(DC* dc) const
{
	UI_EditField::draw(dc);
}

std::string UI_EndRunDialog::lastString="bo";

