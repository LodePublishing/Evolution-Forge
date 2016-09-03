#ifndef _EF_ENDRUNDIALOG_HPP
#define _EF_ENDRUNDIALOG_HPP

#include "../ui/editfield.hpp"
#include "../ui/checkbutton.hpp"
#include <string>

// simplified window class, maybe inheritate Window from EndRunDialog...
class EndRunDialog : public UI_EditField
{
	public:
		EndRunDialog(UI_Object* edit_parent, const std::string& name_proposal);
		~EndRunDialog();
		EndRunDialog(const EndRunDialog& object);
		EndRunDialog& operator=(const EndRunDialog& object);
		void process();
		void draw(DC* dc) const;
		static std::string& getLastString();
	private:
		UI_CheckButton* autoSaveRuns;
		static std::string lastString;
};

#endif

