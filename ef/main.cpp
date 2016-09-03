#include "guimain.hpp"
#include "../sdl/framerate.hpp"

#include "../ui/editfield.hpp"

int main(int argc, char *argv[])
{
//	putenv("SDL_VIDEODRIVER=directfb");

// ------ LOAD CONFIGURATION FILES ------
	toLog("Loading messages / Lade Nachrichten ...");
	list<string> stringFiles = settings.findFiles("settings", "strings", "");
	for(list<string>::iterator j = stringFiles.begin(); j!=stringFiles.end(); j++)
		UI_Object::theme.loadStringFile(*j);
	//fillInfluenceList();
	toLog(*UI_Object::theme.lookUpString(START_START_STRING));
	toLog(*UI_Object::theme.lookUpString(START_LOAD_CORE_SETTINGS_STRING));
	configuration.loadConfigurationFile();
	UI_Object::theme.setLanguage(configuration.getLanguage());
// ------ END LOAD CONFIGURATION FILES -------



// ------ INIT SDL AND WINDOW ------
	toLog(*UI_Object::theme.lookUpString(START_INIT_SDL_STRING));
	
 	SDL_Rect clientWindow;
	clientWindow.x=0;clientWindow.y=0;clientWindow.w=1024;clientWindow.h=768;
	DC* screen;
	if (configuration.isFullScreen()) 
	{
		toLog(*UI_Object::theme.lookUpString(START_SET_FULLSCREEN_MODE_STRING));
		screen=new DC(clientWindow.w, clientWindow.h, SDL_DRAW_BPP*8, SDL_HWSURFACE|SDL_ANYFORMAT|SDL_FULLSCREEN|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_HWPALETTE|SDL_SRCCOLORKEY|SDL_RLEACCEL|SDL_SRCALPHA|SDL_PREALLOC|SDL_DOUBLEBUF, SDL_INIT_NOPARACHUTE);
	}
	else 
	{
		toLog(*UI_Object::theme.lookUpString(START_SET_WINDOW_MODE_STRING));
		screen = new DC(clientWindow.w, clientWindow.h, SDL_DRAW_BPP*8, SDL_HWSURFACE|SDL_ANYFORMAT|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_HWPALETTE|SDL_SRCCOLORKEY|SDL_RLEACCEL|SDL_SRCALPHA|SDL_PREALLOC|SDL_DOUBLEBUF, SDL_INIT_NOPARACHUTE);
	}
	
	if(!screen->initializationOK())
	{
		toLog(*UI_Object::theme.lookUpString(START_UNABLE_TO_INIT_SDL_STRING) + SDL_GetError());
		return(EXIT_FAILURE);
	}
	
	if ( screen->GetSurface() == NULL )
	{
		toLog(*UI_Object::theme.lookUpString(START_ERROR_SETTING_VIDEO_MODE_STRING) + SDL_GetError());
		return(EXIT_FAILURE);
	}
	
	screen->setResolution(clientWindow.w, clientWindow.h);
	screen->printInformation();
	
	SDL_Event event;

	UI_Object::max_x=clientWindow.w;UI_Object::max_y=clientWindow.h;

	SDL_WM_SetCaption("EVOLUTION FORGE BETA - www.clawsoftware.de","");
//	if(configuration.isSoftwareMouse())
//		SDL_ShowCursor(SDL_DISABLE);
// ------ END INIT SDL AND WINDOW ------



// ------ INTRO PICTURE ------
	SDL_Surface* progress = SDL_LoadBMP("data/bitmaps/bar.bmp");
//	Bitmap claw("data/bitmaps/clawsoftware.bmp");
	screen->DrawBitmap(progress, (clientWindow.w - progress->w)/2, (clientWindow.h - progress->h)/2-60);
//	screen->DrawBitmap(claw, clientWindow.w - claw->w, clientWindow.h - claw->h);
	screen->SetPen(Pen(Color(screen->GetSurface(), 255, 255, 255), 1, SOLID_PEN_STYLE));
	screen->SetBrush(Brush(Color(screen->GetSurface(), 100, 150, 255), SOLID_BRUSH_STYLE));
	Main::bar = new ProgressBar(Rect((clientWindow.w-progress->w)/2 + 10, (clientWindow.h - progress->h)/2 - 50, progress->w - 20, progress->h - 20));
	Main::bar->draw(screen, 5);
// ------ END INTRO PICTURE -------




// ------ INIT SDL_TTF ------
	toLog(*UI_Object::theme.lookUpString(START_INIT_SDL_TRUETYPE_FONTS_STRING));
	if(TTF_Init()==-1)
	{
		toLog(std::string("TTF_Init: ") + TTF_GetError());
		return(EXIT_FAILURE);
	}
	atexit(TTF_Quit); 
// ------- END INIT SDL_TTF -------

// ------ CAP FRAMERATE ------
	FPS* fps=new FPS();
	fps->setDesiredFramerate(configuration.getStaticFramerate());
	fps->setAllowStaticFramerate(configuration.isAllowStaticFramerate());
// ------ END CAP FRAMERATE


// ------- INIT GRAPHIC ENGINE ------
	Main::bar->draw(screen, 5);
	toLog(*UI_Object::theme.lookUpString(START_INIT_GRAPHIC_ENGINE_CORE_STRING));
	Main m(screen);

	unsigned int refresh = fps->getFramesPerGeneration();
	if(configuration.isAutoSaveRuns())
		m.startOptimizing();
	else
		m.stopOptimizing();

	bool endrun = false;

	Main::bar->draw(screen, 100, START_SYSTEM_READY_STRING);
	delete Main::bar;
// ------ END INIT GRAPHIC ENGINE ------

// ------ INTRO ------
	Rect t((clientWindow.w-650)/2 + 10, (clientWindow.h - 750)/2 + 10, 650 - 20, 650 - 20);
	Rect t2((clientWindow.w-600)/2 + 10, (clientWindow.h - 700)/2 + 10, 600 - 20, 600 - 20);
	
UI_StaticText introText(NULL, "$Welcome to Evolution Forge " + CORE_VERSION + " :)$# # $ABOUT THE BETA TEST:$# #- $How can I help?$# Post your ideas, discuss or report bugs at the forums at $clawsoftware.de$!#- $It's a beta test... so what do I have to do?$#Test the program on different pcs, different configurations, color settings, drivers etc and report back any crashes, bugs etc#Try out many different, especially unusual goal lists to test the core, let the program run some hours, change the settings, ...# Please do not mess with the data files, the loading routines do not take notice of every error. In the worst case the programm will crash.# # $ABOUT THE PROGRAM ITSELF:$# # - $What does this program?$#The program simulates an abstract StarCraft : Broodwar environment, calculates the time a certain build order needs and optimizes randomly created build orders to a given goal list using evolutionary algorithms.# # $USER INTERFACE:$# # $Keyboard$# - $SPACE$: deactivate drawing (less CPU usage / faster calculation)# - $ALT + ENTER$: switch between fullscreen and window mode# - $ESC$: quits the program without saving# - $PAUSE$: stop/continue calculation# # $Mouse$# - $LEFT BUTTON$: activates buttons and adds items# - $RIGHT BUTTON$: removes items (units) or adds very many items (+/- buttons) # # - $Saving/Loading$: Saved build orders are placed in output/bos/<the race>/, goals are placed in settings/goals/<the race>/ # # NOW HAVE FUN! 8-D # # Best regards, # Clemens Lode", t2, BRIGHT_TEXT_COLOR, SMALL_MIDDLE_NORMAL_FONT, FORMATTED_TEXT_MODE);
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
//		screen->DrawBitmap(claw, clientWindow.w - claw->w, clientWindow.h - claw->h);
		introText.draw(screen);
		screen->updateScreen();
		fps->delay();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					return(EXIT_SUCCESS);break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_KEYDOWN:
					done=true;break;
			}
		}
	}

// ------ END INTRO ------
	
	while(true)
	{
// ------ FRAMERATE AND CALCULATION ------	
		unsigned int frames_per_generation = fps->getFramesPerGeneration();
		unsigned int frames_per_second = fps->getCurrentFramerate();
		
		if((!endrun)&&(((UI_Object::editTextField==NULL)&&(m.isOptimizing()))||(configuration.isAllowStaticFramerate())))
		{
			if(configuration.isAllowStaticFramerate())
				fps->setDesiredFramerate(configuration.getStaticFramerate());
			else
				fps->setDesiredFramerate(configuration.getDynamicFramerate());
			fps->setAllowStaticFramerate(configuration.isAllowStaticFramerate());
			fps->delay();
	
			configuration.setCurrentFramerate(frames_per_second);
			configuration.setCurrentFramesPerGeneration(frames_per_generation);

			refresh+=100;
			while((refresh > frames_per_generation) && (!(endrun = settings.getIsNewRun())))
			{
				m.OnIdle();
				if(frames_per_generation > refresh)
					refresh = 0;
				else
					refresh -= frames_per_generation;
			}
		}
		if(endrun)
			endrun = m.newRun();
// ------ END FRAMERATE AND CALCULATION 

// ------ DRAWING ------
		UI_Object::redrawnObjects = 0;
		m.process();
		m.draw(screen);
		
// ------ END DRAWING ------


// ------ FPS DEBUG
//		screen->SetTextForeground(DC::toSDL_Color(255, 20, 20));
//		screen->SetFont(UI_Object::theme.lookUpFont(LARGE_NORMAL_BOLD_FONT));
//		screen->SetBrush(Brush(Color(screen->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
//		screen->DrawRectangle(110,350,200,20);

//		ostringstream os;
//		os << "Objects: " << UI_Object::redrawnObjects << "   FPS: " << configuration.getCurrentFramerate();
//		screen->DrawText(os.str(), 100, 350);	
// ------ END FPS DEBUG

		screen->updateScreen();
	

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					delete fps;
					delete screen;
					return(EXIT_SUCCESS);
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT)
						m.leftDown();
					else if(event.button.button == SDL_BUTTON_RIGHT)
						m.rightDown();
					else if(event.button.button == SDL_BUTTON_WHEELUP)
						m.wheelUp();
					else if(event.button.button == SDL_BUTTON_WHEELDOWN)
						m.wheelDown();
					break;
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT)
						m.leftUp(Point(event.motion.x, event.motion.y));
					else if(event.button.button == SDL_BUTTON_RIGHT)
						m.rightUp(Point(event.motion.x, event.motion.y));
                                        else if(event.button.button == SDL_BUTTON_WHEELUP)
                                                m.wheelUp();
                                        else if(event.button.button == SDL_BUTTON_WHEELDOWN)
                                                m.wheelDown();
					break;
				case SDL_MOUSEMOTION:
					m.setMouse(Point(event.motion.x, event.motion.y));break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_BACKSPACE:
						if(UI_Object::editTextField!=NULL)
							UI_Object::editTextField->removeCharBackspace();
						break;
						case SDLK_TAB:break;//TODO
						case SDLK_CLEAR:break;//TODO
						case SDLK_KP_ENTER:
						case SDLK_RETURN:
							if(UI_Object::editTextField==NULL)
							{
								if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))
								{
									screen->setFullscreen(!configuration.isFullScreen());
									configuration.setFullScreen(!configuration.isFullScreen());
									m.noticeFullscreen();
								}
							} else
								UI_Object::editTextField->forceDone();
							// else: OK bestaetigen!
						break;
						case SDLK_ESCAPE:
							if(UI_Object::editTextField==NULL)
							{
								delete fps;fps=NULL;
								delete screen;screen=NULL;
								return(EXIT_SUCCESS);
							} else
								UI_Object::editTextField->forceCancel();
							break;
						case SDLK_PAUSE:
						{
							if(m.isOptimizing())
								m.stopOptimizing();
							else m.startOptimizing();
						}break;
						case SDLK_SPACE:
							if(UI_Object::editTextField==NULL)
							{
								if(m.isOptimizing())
									m.stopOptimizing();
								else m.startOptimizing();
							} else
							UI_Object::editTextField->addChar(' ');
							break;
						case SDLK_EXCLAIM:break;
						case SDLK_QUOTEDBL:break;
						case SDLK_HASH:break;
						case SDLK_DOLLAR:break;
						case SDLK_AMPERSAND:break;
						case SDLK_QUOTE:break;
						case SDLK_LEFTPAREN:break;
						case SDLK_RIGHTPAREN:break;
						case SDLK_KP_MULTIPLY:
						case SDLK_ASTERISK:break; // TODO
						case SDLK_KP_PLUS:
						case SDLK_PLUS:break;
						case SDLK_COMMA:break;
						case SDLK_KP_MINUS:break;
						case SDLK_MINUS:
							if((event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))&&(UI_Object::editTextField!=NULL))
							{
								UI_Object::editTextField->addChar('_');
								break;
							}break;
						case SDLK_KP_PERIOD:
						case SDLK_PERIOD:break;//TODO
						case SDLK_KP_DIVIDE:break;
						case SDLK_SLASH:break;//TODO

						case SDLK_COLON:break;
						case SDLK_SEMICOLON:break;
						case SDLK_LESS:break;//TODO
						case SDLK_KP_EQUALS:break;
						case SDLK_EQUALS:break;
						case SDLK_GREATER:break;//TODO
						case SDLK_QUESTION:break;
						case SDLK_AT:break;
						case SDLK_LEFTBRACKET:break;
						case SDLK_BACKSLASH:break;
						case SDLK_RIGHTBRACKET:break;
						case SDLK_CARET:break;
						case SDLK_UNDERSCORE:
							if(UI_Object::editTextField!=NULL)
								UI_Object::editTextField->addChar('_');
							break;
						case SDLK_BACKQUOTE:break;
						case SDLK_DELETE:
							if(UI_Object::editTextField!=NULL)
								UI_Object::editTextField->removeCharDelete();
							break;
						case SDLK_KP0:
						case SDLK_KP1:
						case SDLK_KP2:
						case SDLK_KP3:
						case SDLK_KP4:
						case SDLK_KP5:
						case SDLK_KP6:
						case SDLK_KP7:
						case SDLK_KP8:
						case SDLK_KP9:
							if(UI_Object::editTextField!=NULL)
								UI_Object::editTextField->addChar('0'+event.key.keysym.sym-SDLK_0);
							break;


//						case SDLK_PRINT:break;
/*						case SDLK_PLUS:
								if(UI_Object::theme.getResolution()<RESOLUTION_1280x1024)
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() + 1));
								break;
						case SDLK_MINUS:
								if(UI_Object::theme.getResolution()>RESOLUTION_800x600)
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() - 1));
								break;*/
//						case SDLK_F1:m.mainWindow->forcePressTab(BASIC_TAB);break;
//						case SDLK_F2:m.mainWindow->forcePressTab(ADVANCED_TAB);break;
//						case SDLK_F3:m.mainWindow->forcePressTab(EXPERT_TAB);break;
//						case SDLK_F4:m.mainWindow->forcePressTab(GOSU_TAB);break;
//						case SDLK_F5:m.mainWindow->forcePressTab(COMPARE_TAB);break;
//						case SDLK_F6:m.mainWindow->forcePressTab(MAP_TAB);break;
//						case SDLK_F7:m.mainWindow->forcePressTab(SETTINGS_TAB);break;
//						case SDLK_F8:m.mainWindow->forcePressTab(TUTORIAL_TAB);break;
						case SDLK_a:
						case SDLK_b:
						case SDLK_c:
						case SDLK_d:
						case SDLK_e:
						case SDLK_f:
						case SDLK_g:
						case SDLK_h:
						case SDLK_i:
						case SDLK_j:
						case SDLK_k:
						case SDLK_l:
						case SDLK_m:
						case SDLK_n:
						case SDLK_o:
						case SDLK_p:
						case SDLK_q:
						case SDLK_r:
						case SDLK_s:
						case SDLK_t:
						case SDLK_u:
						case SDLK_v:
						case SDLK_w:
						case SDLK_x:
						case SDLK_y:
						case SDLK_z:
							if(UI_Object::editTextField!=NULL)
							{
								if(event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))
									UI_Object::editTextField->addChar('A'+event.key.keysym.sym-SDLK_a);
								else
									UI_Object::editTextField->addChar('a'+event.key.keysym.sym-SDLK_a);
							}
							break;
						case SDLK_0:
						case SDLK_1:
						case SDLK_2:
						case SDLK_3:
						case SDLK_4:
						case SDLK_5:
						case SDLK_6:
						case SDLK_7:
						case SDLK_8:
						case SDLK_9:
							if(UI_Object::editTextField!=NULL)
								UI_Object::editTextField->addChar('0'+event.key.keysym.sym-SDLK_0);
							break;

						case SDLK_LEFT:
							if(UI_Object::editTextField!=NULL)
								UI_Object::editTextField->moveLeft();
							break;

						case SDLK_RIGHT:
							if(UI_Object::editTextField!=NULL)
								UI_Object::editTextField->moveRight();
							break;

						default:break;
					}
					break;
				default:break;
			}
		}
	}
}
					

