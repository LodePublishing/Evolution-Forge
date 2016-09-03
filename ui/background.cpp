#include "background.hpp"
#include "theme.hpp"

UI_BackGround::UI_BackGround() : 
	UI_Object( NULL, Rect(Point(0,0), theme.getCurrentResolutionSize()), Size(), DO_NOT_ADJUST)
{
	makePufferInvalid();
}

UI_BackGround::~UI_BackGround()
{}

void UI_BackGround::reloadOriginalSize()
{
	setOriginalSize(theme.getCurrentResolutionSize());
	UI_Object::reloadOriginalSize();
}

void UI_BackGround::process()
{
	UI_Object::process();
}

void UI_BackGround::object_info()
{
	toErrorLog("ui_background");
}

void UI_BackGround::draw() const
{
	UI_Object::draw();
#if 0
// TODO!!!!!!!!!!!!! besonders das background bitmap nicht UEBERALL neu zeichnen
	if((!introWindow->isShown())&&(!languageMenu->isShown()))
	{
#endif
		SDL_Rect rc;
		rc.x = 0;rc.y = 0; rc.w = UI_Object::max_x; rc.h = UI_Object::max_y;
		if(uiConfiguration.isBackgroundBitmap())
			dc->Blit(UI_Object::theme.lookUpBitmap(BACKGROUND_SC_BITMAP), rc);
		else
			dc->clearScreen();
		
//	}
//	else
//		dc->clearScreen();
		dc->setFont(UI_Object::theme.lookUpFont(SMALL_FONT));
		Size s = dc->getTextExtent(CORE_VERSION);	
		dc->setTextForeground(DC::toSDL_Color(100, 100, 100));
		dc->DrawText(CORE_VERSION, Point(UI_Object::max_x - s.getWidth() - 10, UI_Object::max_y - s.getHeight() - 5));
		
	UI_Object::theme.setColorTheme(UI_Object::theme.getMainColorTheme());
#if 0
	if(languageMenu->isShown())
	{
		dc->setBrush(*UI_Object::theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
		dc->setPen(*UI_Object::theme.lookUpPen(NULL_PEN));
		dc->DrawRectangle(mainWindow->getAbsoluteRect());
//		dc->addRectangle(mainWindow->getAbsoluteRect());
	}
#endif

	
//	
#if 0
// ------ MOUSE DRAWING ------
		if(efConfiguration.isSoftwareMouse())
		{
//			SDL_ShowCursor(SDL_DISABLE);
			Point p = UI_Object::mouse - Size(20,10);//Point(90, 140);
			dc->setFont(UI_Object::theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
			switch(UI_Object::mouseType)
			{
				case 0://dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_NONE), p);
					break;
				case 1:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_LEFT), p);
					dc->setTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
				break;
				case 2:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_RIGHT), p);
					dc->setTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				case 3:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_BOTH), p);
					dc->setTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
					dc->setTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				default:toLog("error, mouseType out of range");break;
			}
		}
//		else
//			SDL_ShowCursor(SDL_ENABLE);
#endif
// ------ END MOUSE DRAWING ------
}


