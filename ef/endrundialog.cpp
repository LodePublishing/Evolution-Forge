#include "endrundialog.hpp"
#include "configuration.hpp"

// ISO-LEVEL 2 

EndRunDialog::EndRunDialog(UI_Object* edit_parent, const std::string& name_proposal) :
	UI_EditField(edit_parent, NULL, ENDRUN_DIALOG_TITLE_STRING, ENDRUN_QUESTION_STRING, ENDRUN_SAVE_AND_CONTINUE_STRING, ENDRUN_DONT_SAVE_AND_CONTINUE_STRING, name_proposal),
	autoSaveRuns(new UI_CheckButton(this, Rect(Point(25, 80), Size(200, 15)), SETTING_AUTO_SAVE_RUNS_STRING, SETTING_AUTO_SAVE_RUNS_TOOLTIP_STRING, efConfiguration.isAutoSaveRuns()))
{ }

EndRunDialog::~EndRunDialog()
{
	if(!isCanceled())
	{
		lastString = getString();
		efConfiguration.setAutoSaveRuns ( autoSaveRuns->isChecked() );
	}
	delete autoSaveRuns;
}

EndRunDialog::EndRunDialog(const EndRunDialog& object) :
	UI_EditField((UI_EditField)object),
	autoSaveRuns(new UI_CheckButton(*object.autoSaveRuns))
{ }

EndRunDialog& EndRunDialog::operator=(const EndRunDialog& object)
{
	(UI_EditField)(*this) = (UI_EditField)object;
	delete autoSaveRuns;
	autoSaveRuns = new UI_CheckButton(*object.autoSaveRuns);
	return(*this);
}

std::string& EndRunDialog::getLastString()
{
	return lastString;
}

void EndRunDialog::process()
{
	UI_EditField::process();
}

void EndRunDialog::draw(DC* dc) const
{
	UI_EditField::draw(dc);
}

std::string EndRunDialog::lastString="bo";

