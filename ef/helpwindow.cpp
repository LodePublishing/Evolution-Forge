#include "helpwindow.hpp"

HelpWindow::HelpWindow(UI_Window* parent_window) :
	UI_Window(parent_window, HELP_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(HELP_WINDOW), theme.lookUpGlobalMaxHeight(HELP_WINDOW), SCROLLED),
	helpChapter(INDEX_CHAPTER),
	text(new UI_LongText(this, Rect(Point(20, 30), Size(getSize()-Size(80, 60))), Size(0, 0), UI_Object::theme.lookUpHelpChapterString(helpChapter), BRIGHT_TEXT_COLOR, FORCE_TEXT_COLOR, LARGE_FONT, TEXT_BUTTON, VISITED_TEXT_BUTTON, getScrollBar(), true)),
	indexButton(new UI_Button(this, Rect(10, 12, 0, 0), Size(0, 0), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, HELP_WINDOW_INDEX_STRING, TOP_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_SIZE)),
	backButton(new UI_Button(this, Rect(0, 12, 0, 0), Size(20, 0), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, HELP_WINDOW_BACK_STRING, TOP_RIGHT, SMALL_SHADOW_BOLD_FONT, AUTO_SIZE)),
	lastChapter()
{
	indexButton->Hide();
	backButton->Hide();
}

HelpWindow::~HelpWindow()
{
	delete indexButton;
	delete backButton;
	delete text;
}

void HelpWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(HELP_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(HELP_WINDOW));
	UI_Window::reloadOriginalSize();
	text->setOriginalSize(Size(getSize()-Size(80, 60)));
}

void HelpWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);
	if(helpChapter == INDEX_CHAPTER)
	{
		dc->DrawBitmap(UI_Object::theme.lookUpBitmap(HELP_MAIN_BITMAP), getAbsoluteRect().getTopRight() - Size(UI_Object::theme.lookUpBitmap(HELP_MAIN_BITMAP)->w+10, 0) + Size(0, 25));
		dc->addRectangle(Rect(getAbsoluteRect().getTopRight() - Size(UI_Object::theme.lookUpBitmap(HELP_MAIN_BITMAP)->w+10, 0) + Size(0, 25), Size(UI_Object::theme.lookUpBitmap(HELP_MAIN_BITMAP)->w, UI_Object::theme.lookUpBitmap(HELP_MAIN_BITMAP)->h)));
	}
}

void HelpWindow::process()
{
	UI_Window::process();

	if(indexButton->isLeftClicked())
		gotoChapter(INDEX_CHAPTER);
	else
	if(backButton->isLeftClicked())
		goBack();
	else
	{
		signed int pressed = text->getPressed();
		if((pressed>=0)&&(helpChapter!=pressed))
			gotoChapter(pressed);
	}
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound(), getAbsoluteClientRectLowerBound());
}

void HelpWindow::goBack()
{
	if((!lastChapter.empty())&&(lastChapter.back()!=helpChapter))
	{
		gotoChapter(lastChapter.back());
		lastChapter.pop_back();
		lastChapter.pop_back();
		if(lastChapter.empty())
			backButton->Hide();
	}
}


void HelpWindow::gotoChapter(unsigned int chapter)
{
	if(helpChapter==chapter)
		return;
	uiConfiguration.visitHelpChapter(helpChapter);
	lastChapter.push_back(helpChapter);
	backButton->Show();
	helpChapter = (eHelpChapter)chapter;
	text->updateText(UI_Object::theme.lookUpHelpChapterString(helpChapter));
	getScrollBar()->moveToTop();
	getScrollBar()->setLastItemY(text->getTextHeight());
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound(), getAbsoluteClientRectLowerBound());
//	setNeedRedrawMoved(); TODO
	if(helpChapter == INDEX_CHAPTER)
		indexButton->Hide();
	else indexButton->Show();
}

