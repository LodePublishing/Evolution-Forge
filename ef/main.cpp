#include "guimain.hpp"
#include "../sdl/framerate.hpp"

int main(int argc, char *argv[])
{
	//fillInfluenceList();

	toLog("Starting ... ");
	long unsigned int startTime = SDL_GetTicks();
	toLog("Loading core settings (settings/default.cfg)...");
	configuration.loadConfigurationFile();
	
// ------ INIT SDL ------
	toLog("Initializing SDL ... ");
	
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0 )
	{
		ostringstream os;
		os << "Unable to init SDL: " << SDL_GetError();
		toLog(os.str());
		return(1);
	}
	atexit(SDL_Quit);
	SDL_Event event;
// ------ END INIT SDL -------


// ------ INIT WINDOW ------
	toLog("Initializing videomode...");
	
 	SDL_Rect clientWindow;
	clientWindow.x=0;clientWindow.y=0;clientWindow.w=1024;clientWindow.h=768;
	DC* screen;
	if (configuration.isFullScreen()) {
		toLog("Setting fullscreen...");
		screen=new DC(SDL_SetVideoMode(clientWindow.w, clientWindow.h, 32, SDL_SWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT|SDL_FULLSCREEN));
	}
	else {
		toLog("Setting window mode...");
		screen=new DC(SDL_SetVideoMode(clientWindow.w, clientWindow.h, 32, SDL_SWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT));
	}
	if ( screen == NULL )
	{
		toLog("Error setting video mode");
		return(1);
	}
	screen->setResolution(clientWindow.w, clientWindow.h);
	UI_Object::max_x=clientWindow.w;
	UI_Object::max_y=clientWindow.h;
	SDL_WM_SetCaption("EVOLUTION FORGE (C) CLEMENS LODE, 2004 www.clawsoftware.de","");
	if(configuration.isSoftwareMouse())
		SDL_ShowCursor(SDL_DISABLE);
// ------ END INIT WINDOW ------


// ------ INTRO PICTURE ------
	Bitmap progress("data/bitmaps/back2.bmp");
	Bitmap claw("data/bitmaps/clawsoftware.bmp");
	screen->DrawBitmap(progress, (clientWindow.w - progress->w)/2, (clientWindow.h - progress->h)/2);
	screen->DrawBitmap(claw, clientWindow.w - claw->w, clientWindow.h - claw->h);
	screen->SetPen(Pen(Color(screen->GetSurface(), 255, 255, 255), 1, SOLID_PEN_STYLE));
	screen->SetBrush(Brush(Color(screen->GetSurface(), 100, 150, 255), SOLID_BRUSH_STYLE));
	Main::bar = new ProgressBar(Rect((clientWindow.w-progress->w)/2 + 10, (clientWindow.h - progress->h)/2 + 10, progress->w - 20, progress->h - 20));
	Main::bar->draw(screen, 5);
// ------ END INTRO PICTURE -------


// ------ INIT SDL_TTF ------
	toLog("Initializing SDL truetype fonts ... ");
	if(TTF_Init()==-1)
	{
		ostringstream os;
		os << "TTF_Init: " << TTF_GetError();
		toLog(os.str());		
		return 2;
	}
	atexit(TTF_Quit); 
// ------- END INIT SDL_TTF -------


	Main::bar->draw(screen, 5);
	toLog("Initializing graphic engine and core ...");
	Main m(screen);

// ------ CAP FRAMERATE ------
	FPS* fps=new FPS();
	fps->setFramerate(99);
	unsigned int screenshot=100;
	bool screenCapturing=false;
	bool warning = false;
// ------ END CAP FRAMERATE

	Main::bar->draw(screen, 100, "Graphic engine and core initialized");
	toLog("System ready ...");
	ostringstream oss;
	oss << ((float)(startTime - SDL_GetTicks()))/1000000000.0;
	toLog("Initialization time: "+oss.str());
	delete Main::bar;


#if 0
// ------ INTRO ------
	Rect t((clientWindow.w-650)/2 + 10, (clientWindow.h - 750)/2 + 10, 650 - 20, 750 - 20);
	Rect t2((clientWindow.w-600)/2 + 10, (clientWindow.h - 700)/2 + 10, 600 - 20, 700 - 20);
	
UI_StaticText introText(NULL, "$Welcome to Evolution Forge 0.60 BETA Test :)$# # $ABOUT THE BETA TEST:$#- $How can I help?$#$Most important:$ Post your ideas, discuss or report bugs at the forums at $evolutionforge.com$!#- $It's a beta test... so what do I have to do?$#Test the program on different pcs, different configurations, color settings, drivers etc and report back any crashes, bugs etc#Try out many different, especially unusual goal lists to test the core, let the program run some hours, test the command line options, change the settings, ...#$DO NOT MESS WITH THE DATA FILES :D$ The loading do not take notice of any error. In the worst case the programm will crash.# # $ABOUT THE PROGRAM ITSELF:$# # - $What does this program?$#The program $simulates an abstract StarCraft : Broodwar environment$, $calculates the time$ a certain build order needs and $optimizes randomly created build orders$ to a given goal list using $evolutionary algorithms$.#- $Is it accurate, where are the limits?$#Depending on the $map$, the $latency$, your $mouse speed$ and many other small factors there might be slightly faster build orders. Also the program is based on $one second steps$ and minerals/gas are gathered per second. Because of that you have to exercise the build orders and adapt them to your needs.#- $Is it useful?$#Hard question, you have to decide for yourself. I think that is certainly useful for beginners who want to improve their gaming. Personally I look at the program as a well sorted collection of build orders but also as a sort of toy or game :) It is always fun trying something new :D#- $Why Starcraft?$Stupid question.# # NOW HAVE FUN! 8-D", t2, FORMATTED_TEXT_MODE, BRIGHT_TEXT_COLOR, SMALL_MIDDLE_NORMAL_FONT);
	bool done = false;
	while(!done)
	{
		introText.process();
	    screen->SetPen(Pen(Color(screen->GetSurface(), 0, 0, 0), 1, TRANSPARENT_PEN_STYLE));
		screen->SetBrush(Brush(Color(screen->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
		screen->DrawRectangle(Rect(clientWindow.x, clientWindow.y, clientWindow.w-1, clientWindow.h-1));
	    screen->SetPen(*UI_Object::theme.lookUpPen(OUTER_BORDER_PEN));
		screen->SetBrush(*UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		screen->DrawEdgedRoundedRectangle(t,6);
		screen->DrawBitmap(claw, clientWindow.w - claw->w, clientWindow.h - claw->h);
		introText.draw(screen);
        screen->updateScreen();
        fps->delay();
		while (SDL_PollEvent(&event))
		{
        	switch (event.type)
	        {
	            case SDL_QUIT:
    	            return(0);break;
        	    case SDL_KEYDOWN:
//					if(introText.doneWriting())
						done=true;
					break;
			}
		}
	}

// ------ END INTRO ------
#endif	

	bool drawing = true;;
	int oldDynamicFramerate = configuration.getDynamicFramerate();
	bool oldAllowStaticFramerate = configuration.isAllowStaticFramerate();

	while(true)
	{
		if(drawing)
			m.process();
		else			
			m.player[0]->window[STATISTICS_WINDOW]->process();
			
		m.OnIdle();

// ------ DRAWING ------
		if(drawing)
		{
			warning=false;
			if(configuration.isBackgroundBitmap())
			{
				SDL_BlitSurface(*UI_Object::theme.lookUpBitmap(BACKGROUND_BITMAP) , 0, screen->GetSurface(), &clientWindow );
			}
			else
			{
			    screen->SetPen(Pen(Color(screen->GetSurface(), 0, 0, 0), 1, SOLID_PEN_STYLE));
				screen->SetBrush(Brush(Color(screen->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
				screen->DrawRectangle(Rect(clientWindow.x, clientWindow.y, clientWindow.w-1, clientWindow.h-1));
			}
			screen->DrawBitmap(claw, clientWindow.w - claw->w, clientWindow.h - claw->h);
			m.draw(screen);
// ------ MOUSE DRAWING ------
			if(configuration.isSoftwareMouse())
			{
				Point p = UI_Object::mouse - Size(20,10);//Point(90, 140);
				screen->SetFont(UI_Object::theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
				switch(UI_Object::mouseType)
				{
					case 0:screen->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_NONE), p);
						break;
					case 1:
							screen->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_LEFT), p);
							screen->SetTextForeground(toSDL_Color(179,0,0));
							screen->DrawText("Add a unit", p.x-50, p.y+2);
						break;
					case 2:
							screen->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_RIGHT), p);
							screen->SetTextForeground(toSDL_Color(0,177,188));
							screen->DrawText("Remove a unit", p.x+38, p.y+1);
						break;
					case 3:
							screen->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_BOTH), p);
							screen->SetTextForeground(toSDL_Color(179,0,0));
							screen->DrawText("Add a unit", p.x-50, p.y+2);
							screen->SetTextForeground(toSDL_Color(0,177,188));
							screen->DrawText("Remove a unit", p.x+38, p.y+1);
						break;
				}
			}
// ------ END MOUSE DRAWING ------
			
		}
// ------ END DRAWING ------
		else if(!warning)
		{
			screen->SetTextForeground(toSDL_Color(255, 100, 100));
			screen->SetFont(UI_Object::theme.lookUpFont(LARGE_NORMAL_BOLD_FONT));
            screen->DrawText("DRAWING SUSPENDED FOR MAXIMUM SPEED, PRESS A BUTTON OR KEY TO STOP.", 100, 350);	
			//warning = true;
			m.player[0]->window[STATISTICS_WINDOW]->draw(screen);
		}
		
// ------ SCREENCAPTURE ------		
		if(screenCapturing) {
			ostringstream os;os << "shot" << screenshot << ".bmp";
			SDL_SaveBMP(screen->GetSurface() , os.str().c_str());os.str("");screenshot++;
			os << " saved (" << (clientWindow.w * clientWindow.h)/256 << "kb) [" << (screenshot * clientWindow.w * clientWindow.h)/(1024*256)<<"MB total]";
			screen->DrawText(os.str(), 50, 600);
		}
// ------ END SCREENCAPTURE -----

		screen->updateScreen();
		fps->delay();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					delete fps;
					delete screen;
					return(0);
					break;
				case SDL_KEYDOWN:
					if(!drawing)
					{
						configuration.setAllowStaticFramerate(oldAllowStaticFramerate);
						configuration.setDynamicFramerate(oldDynamicFramerate);
						drawing=true;
					}
					else
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							delete fps;fps=NULL;
							delete screen;screen=NULL;
							return(0);
						}break;
						case SDLK_SPACE:
						{
							if(m.isOptimizing())
								m.stopOptimizing();
							else m.startOptimizing();
						}break;					
						case SDLK_p:
							if(event.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL))
								screenCapturing=!screenCapturing;
							break;
						case SDLK_s:
							if(event.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL))
							{
								drawing=false;
								oldAllowStaticFramerate=configuration.isAllowStaticFramerate();
								oldDynamicFramerate=configuration.getDynamicFramerate();
								configuration.setAllowStaticFramerate(false);
							}
							break;
						
						case SDLK_RETURN:
							if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))
								SDL_WM_ToggleFullScreen(screen->GetSurface());
						break;
						case SDLK_PLUS:
								if(UI_Object::theme.getResolution()<RESOLUTION_1280x1024)
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() + 1));
								break;
						case SDLK_MINUS:
								if(UI_Object::theme.getResolution()>RESOLUTION_800x600)
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() - 1));
								break;
						case SDLK_F1:m.mainWindow->forcePressTab(BASIC_TAB);break;
						case SDLK_F2:m.mainWindow->forcePressTab(ADVANCED_TAB);break;
						case SDLK_F3:m.mainWindow->forcePressTab(EXPERT_TAB);break;
						case SDLK_F4:m.mainWindow->forcePressTab(GOSU_TAB);break;
						case SDLK_F5:m.mainWindow->forcePressTab(COMPARE_TAB);break;
						case SDLK_F6:m.mainWindow->forcePressTab(MAP_TAB);break;
						case SDLK_F7:m.mainWindow->forcePressTab(SETTINGS_TAB);break;
						case SDLK_F8:m.mainWindow->forcePressTab(TUTORIAL_TAB);break;
						default:break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
                    if(!drawing)
                    {
                        configuration.setAllowStaticFramerate(oldAllowStaticFramerate);
                        configuration.setDynamicFramerate(oldDynamicFramerate);
                        drawing=true;
                    }
					if(event.button.button == SDL_BUTTON_LEFT)
						m.leftDown();
					else if(event.button.button == SDL_BUTTON_RIGHT)
						m.rightDown();
					break;
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT)
						m.leftUp(Point(event.motion.x, event.motion.y));
					else if(event.button.button == SDL_BUTTON_RIGHT)
						m.rightUp(Point(event.motion.x, event.motion.y));
					break;
				case SDL_MOUSEMOTION:
					m.setMouse(Point(event.motion.x, event.motion.y));break;
					default:break;
			}
		}
	}
}
					

