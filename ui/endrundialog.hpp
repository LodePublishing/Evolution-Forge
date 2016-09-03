#ifndef _UI_ENDRUNDIALOG_HPP
#define _UI_ENDRUNDIALOG_HPP

#include "editfield.hpp"
#include "checkbutton.hpp"
#include <string>

// simplified window class, maybe inheritate UI_Window from UI_EndRunDialog...
class UI_EndRunDialog : public UI_EditField
{
	public:
		UI_EndRunDialog(UI_Object* edit_parent, const std::string& name_proposal);
		~UI_EndRunDialog();
		UI_EndRunDialog(const UI_EndRunDialog& object);
		UI_EndRunDialog& operator=(const UI_EndRunDialog& object);
		void process();
		void draw(DC* dc) const;
		static std::string& getLastString();
	private:
		UI_CheckButton* autoSaveRuns;
		static std::string lastString;
};

#endif

