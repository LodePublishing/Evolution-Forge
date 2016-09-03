#include "guimain.hpp"
#include "../sdl/framerate.hpp"

#include "../ui/editfield.hpp"
#include "configuration.hpp"
#include "../ui/configuration.hpp"
#include "../core/configuration.hpp"


int main(int argc, char *argv[])
{
	std::list<std::string> arguments;
	for(signed int i = 1;i<argc;++i)
		arguments.push_back(argv[i]);

	std::string current_driver;
	if(getenv("SDL_VIDEODRIVER")!=NULL)
		current_driver = getenv("SDL_VIDEODRIVER");

// ------ LOAD CONFIGURATION FILES ------
	uiConfiguration.loadConfigurationFile();
	UI_Object::theme.setLanguage(uiConfiguration.getLanguage());
	std::list<std::string> stringFiles = database.findFiles("settings", "strings", "");
	for(std::list<std::string>::iterator j = stringFiles.begin(); j!=stringFiles.end(); ++j)
		UI_Object::theme.loadStringFile(*j);
	toLog(UI_Object::theme.lookUpString(START_LOAD_CONFIGURATION_STRING));
	coreConfiguration.loadConfigurationFile();
	efConfiguration.loadConfigurationFile();
// ------ END LOAD CONFIGURATION FILES -------

// ------ PARSING COMMAND LINE ------
	toLog(UI_Object::theme.lookUpString(START_PARSE_COMMAND_LINE_STRING));
	if(!arguments.empty())
		for(std::list<std::string>::const_iterator i = arguments.begin();i!=arguments.end(); ++i)
		{
			if((*i) == "-vo")
			{
				++i;
				if(i==arguments.end())
					toLog(UI_Object::theme.lookUpString(START_WARNING_VO_ARGUMENT_STRING));
				else current_driver = *i;
				break;
			}
		}
// ------ END PARSING COMMAND LINE ------


// ------ INIT SDL AND WINDOW ------
	switch(DC::chooseDriver(current_driver))
	{
		case NO_DRIVER_ERROR:toLog(UI_Object::theme.lookUpFormattedString(START_SDL_USING_DRIVER_STRING, current_driver));break;
		case NO_VIDEO_DRIVERS_AVAILIBLE:toLog(UI_Object::theme.lookUpString(START_ERROR_NO_DRIVER_AVAILIBLE_STRING));return(EXIT_FAILURE);break;
		case SDL_DRIVER_NOT_SUPPORTED:toLog(UI_Object::theme.lookUpFormattedString(START_ERROR_DRIVER_NOT_SUPPORTED_STRING, current_driver));return(EXIT_FAILURE);break;
	}

	toLog(UI_Object::theme.lookUpString(START_INIT_SDL_STRING));

	UI_Object::theme.setResolution(uiConfiguration.getResolution());
	UI_Object::theme.setBitDepth(uiConfiguration.getBitDepth());
	
	toLog(UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
	
	DC* screen = new DC(UI_Object::theme.getResolution(), UI_Object::theme.getBitDepth(), (efConfiguration.isFullScreen()?SDL_FULLSCREEN:0)|SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_HWPALETTE|SDL_SRCCOLORKEY|SDL_RLEACCEL|SDL_SRCALPHA|SDL_PREALLOC|SDL_DOUBLEBUF, SDL_INIT_NOPARACHUTE);

	{
		std::ostringstream os;
		os.str("");
		std::list<std::string> s = screen->getAvailibleDrivers();
		for(std::list<std::string>::const_iterator i = s.begin(); i!=s.end(); i++)
			os << *i << " ";
		toLog("Availible drivers: " + os.str());		
	}

		
	if(!screen->initializationOK())	{
		toLog(UI_Object::theme.lookUpString(START_UNABLE_TO_INIT_SDL_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");return(EXIT_FAILURE);		
	}
	
	if ( screen->GetSurface() == NULL ) {
		toLog(UI_Object::theme.lookUpString(START_ERROR_SETTING_VIDEO_MODE_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");return(EXIT_FAILURE);
	}
	
	toLog(DC::printHardwareInformation());

	Size resolution = screen->getResolutionSize();
	UI_Object::setResolution(resolution);

	toLog(DC::printSurfaceInformation(screen));
	
	SDL_Event event;
	SDL_WM_SetCaption("EVOLUTION FORGE BETA - www.clawsoftware.de","");
// ------ END INIT SDL AND WINDOW ------


// ------ INTRO PICTURE ------
//	SDL_Surface* progress = SDL_LoadBMP("data/bitmaps/bar.bmp");
//	Bitmap claw("data/bitmaps/clawsoftware.bmp");
//	screen->DrawBitmap(progress, (resoluton.GetWidth() - progress->w)/2, (resolution.GetHeight() - progress->h)/2-60);
//	screen->DrawBitmap(claw, resolution.GetWidth() - claw->w, resolution.GetHeight() - claw->h);
//	screen->SetPen(Pen(Color(screen->GetSurface(), 255, 255, 255), 1, SOLID_PEN_STYLE));
//	screen->SetBrush(Brush(Color(screen->GetSurface(), 100, 150, 255), SOLID_BRUSH_STYLE));
// ------ END INTRO PICTURE -------


// ------ INIT SDL_TTF ------
	toLog(UI_Object::theme.lookUpString(START_INIT_SDL_TRUETYPE_FONTS_STRING));
	if(TTF_Init()==-1) {
		toLog(std::string("TTF_Init: ") + " [TTF ERROR: \"" + TTF_GetError() + "\"]");return(EXIT_FAILURE);
	}
	atexit(TTF_Quit); 
// ------- END INIT SDL_TTF -------

// ------ CAP FRAMERATE ------
	FPS* fps = new FPS();
	fps->setDesiredFramerate(efConfiguration.getStaticFramerate());
	fps->setAllowStaticFramerate(efConfiguration.isAllowStaticFramerate());
// ------ END CAP FRAMERATE


// ------- INIT GRAPHIC ENGINE ------
	toLog(UI_Object::theme.lookUpString(START_INIT_GRAPHIC_ENGINE_CORE_STRING));
	Main m(screen);

	unsigned int screenshot = 100;
	unsigned int refresh = fps->getFramesPerGeneration();
//	if(efConfiguration.isAutoSaveRuns())
//		m.startAllOptimizing();
//	else
//		m.stopAllOptimizing(); TODO

	bool endrun = false;
	unsigned int screenCapturing=0;

// ------ END INIT GRAPHIC ENGINE ------

// ------ INTRO ------
//	Rect t((resolution.GetWidth()-650)/2 + 10, (resolution.GetHeight() - 750)/2 + 10, 650 - 20, 650 - 20);
//	Rect t2((resolution.w-600)/2 + 10, (resolution.h - 700)/2 + 10, 600 - 20, 600 - 20);
	
//UI_StaticText introText(NULL, "$Welcome to Evolution Forge " + CORE_VERSION + " :)$# # $ABOUT THE BETA TEST:$# #- $How can I help?$# Post your ideas, discuss or report bugs at the forums at $clawsoftware.de$!#- $It's a beta test... so what do I have to do?$#Test the program on different pcs, different configurations, color settings, drivers etc and report back any crashes, bugs etc#Try out many different, especially unusual goal lists to test the core, let the program run some hours, change the settings, ...# Please do not mess with the data files, the loading routines do not take notice of every error. In the worst case the programm will crash.# # $ABOUT THE PROGRAM ITSELF:$# # - $What does this program?$#The program simulates an abstract StarCraft : Broodwar environment, calculates the time a certain build order needs and optimizes randomly created build orders to a given goal list using evolutionary algorithms.# # $USER INTERFACE:$# # $Keyboard$# - $SPACE$: deactivate drawing (less CPU usage / faster calculation)# - $ALT + ENTER$: switch between fullscreen and window mode# - $ESC$: quits the program without saving# - $PAUSE$: stop/continue calculation# # $Mouse$# - $LEFT BUTTON$: activates buttons and adds items# - $RIGHT BUTTON$: removes items (units) or adds very many items (+/- buttons) # # - $Saving/Loading$: Saved build orders are placed in output/bos/<the race>/, goals are placed in settings/goals/<the race>/ # # NOW HAVE FUN! 8-D # # Best regards, # Clemens Lode", t2, Size(0,0), BRIGHT_TEXT_COLOR, SMALL_MIDDLE_NORMAL_FONT, FORMATTED_TEXT_MODE); // TODO
/*	bool done = false;
	while(!done)
	{
		introText.process();
		screen->SetPen(Pen(Color(screen->GetSurface(), 0, 0, 0), 1, TRANSPARENT_PEN_STYLE));
		screen->SetBrush(Brush(Color(screen->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
		screen->DrawRectangle(Rect(resolution.x, resolution.y, resolution.w-1, resolution.h-1));
		screen->SetPen(*UI_Object::theme.lookUpPen(OUTER_BORDER_PEN));
		screen->SetBrush(*UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		screen->DrawEdgedRoundedRectangle(t,6);
//		screen->DrawBitmap(claw, resolution.w - claw->w, resolution.h - claw->h);
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
	}*/

// ------ END INTRO ------


// MAIN LOOP
	while(true)
	{
// ------ DRAWING ------
		UI_Object::redrawnObjects = 0;
		m.process();
		screen->updateScreen();
		m.draw(screen);
// ------ END DRAWING ------

// ------ SCREENCAPTURE ------ 
                if(screenCapturing==100) {
			std::ostringstream os;os.str("");os << "shot" << screenshot << ".bmp";
                        SDL_SaveBMP(screen->GetSurface() , os.str().c_str());
			++screenshot;
		}
		if(screenCapturing>0) {
			--screenCapturing;
			std::ostringstream os;os.str("");os << "shot" << (screenshot-1) << ".bmp" << " saved (" << (resolution.GetWidth() * resolution.GetHeight() * (int)(screen->GetSurface()->format->BitsPerPixel))/1024 << "kb)";
                        screen->DrawText(os.str(), 50, 300);
                }
// ------ END SCREENCAPTURE -----

	
// ------ FRAMERATE AND CALCULATION ------	
		unsigned int frames_per_generation = fps->getFramesPerGeneration();
		unsigned int frames_per_second = fps->getCurrentFramerate();
		
		if((!endrun)&&(((UI_Object::editTextField==NULL)&&(m.isAnyOptimizing()))||(efConfiguration.isAllowStaticFramerate()))) // TODO
		{
			if(efConfiguration.isAllowStaticFramerate())
				fps->setDesiredFramerate(efConfiguration.getStaticFramerate());
			else
				fps->setDesiredFramerate(efConfiguration.getDynamicFramerate());
			fps->setAllowStaticFramerate(efConfiguration.isAllowStaticFramerate());
			fps->delay();
	
			efConfiguration.setCurrentFramerate(frames_per_second);
			efConfiguration.setCurrentFramesPerGeneration(frames_per_generation);

			refresh+=100;
			while((refresh > frames_per_generation))// && (!(endrun = database.getIsNewRun()))) // TODO
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
		endrun=false; // TODO
// ------ END FRAMERATE AND CALCULATION 


// ------ FPS DEBUG
/*		{
			Point p = Point(20, resolution.GetHeight() - 40);
			screen->SetTextForeground(DC::toSDL_Color(255, 20, 20));
			screen->SetFont(UI_Object::theme.lookUpFont(LARGE_NORMAL_BOLD_FONT));
			screen->SetBrush(Brush(Color(screen->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
			screen->DrawRectangle(Rect(p, Size(200, 20)));

			std::ostringstream os;
			os.str("");
			os << "Objects: " << UI_Object::redrawnObjects << "   FPS: " << efConfiguration.getCurrentFramerate();
			screen->DrawText(os.str(), p + Size(20, 10));	
		}*/
// ------ END FPS DEBUG

	

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					delete fps;delete screen;return(EXIT_SUCCESS);
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
//									screen->setFullscreen(!efConfiguration.isFullScreen());
									efConfiguration.setFullScreen(!efConfiguration.isFullScreen());
									m.noticeFullscreen();
//									toLog(UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
								}
							} else
								UI_Object::editTextField->forceDone();
							// else: OK bestaetigen!
						break;
						case SDLK_ESCAPE:
							if(UI_Object::editTextField==NULL) {
								delete fps;delete screen;screen=NULL;return(EXIT_SUCCESS);
							} else
								UI_Object::editTextField->forceCancel();
							break;
						case SDLK_PAUSE:
						{
//							if(m.isOptimizing())
//								m.stopAllOptimizing();
//							else m.startAllOptimizing();
						}break;
						case SDLK_SPACE:
							if(UI_Object::editTextField==NULL)
							{
								screenCapturing=100;
//								if(m.isOptimizing())
//									m.stopAllOptimizing();
//								else m.startAllOptimizing();
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
//						case SDLK_PLUS:break;
						case SDLK_COMMA:break;
						case SDLK_KP_MINUS:break;
//						case SDLK_MINUS:
//							if((event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))&&(UI_Object::editTextField!=NULL))
//							{
//								UI_Object::editTextField->addChar('_');
//								break;
//							}break;
						case SDLK_KP_PERIOD:
						case SDLK_PERIOD:break;//TODO
						case SDLK_KP_DIVIDE:break;
						case SDLK_SLASH:break;//TODO

						case SDLK_COLON:break;
						case SDLK_SEMICOLON:break;
						case SDLK_LESS:break;//TODO
						case SDLK_KP_EQUALS:break;
//						case SDLK_EQUALS:break;
						case SDLK_GREATER:break;//TODO
						case SDLK_QUESTION:break;
						case SDLK_AT:break;
						case SDLK_BACKSLASH:break;
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
						case SDLK_EQUALS:
								if(UI_Object::theme.getResolution()<RESOLUTION_1280x1024)
								{
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() + 1));
									screen->setResolution(UI_Object::theme.getResolution());
									uiConfiguration.setResolution(UI_Object::theme.getResolution());
									resolution = screen->getResolutionSize();
									UI_Object::setResolution(resolution);
									std::ostringstream os;os.str("");
 									os << resolution.GetWidth() << "x" << resolution.GetHeight();
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
									m.reloadOriginalSize();
								}
								break;
						case SDLK_MINUS:
								if(UI_Object::theme.getResolution()>RESOLUTION_640x480)
								{
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() - 1));
									screen->setResolution(UI_Object::theme.getResolution());
									uiConfiguration.setResolution(UI_Object::theme.getResolution());
									resolution = screen->getResolutionSize();
									UI_Object::setResolution(resolution);
									std::ostringstream os;os.str("");
 									os << resolution.GetWidth() << "x" << resolution.GetHeight();
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
									m.reloadOriginalSize();
								}
								break;
						case SDLK_LEFTBRACKET:
								if(screen->getBitDepth() > DEPTH_8BIT)
								{
									UI_Object::theme.setBitDepth((eBitDepth)(screen->getBitDepth()-1));
									screen->setBitDepth(UI_Object::theme.getBitDepth());
									uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
									UI_Object::theme.updateColors(screen->GetSurface());
									// TODO bitDepth im theme aendern!
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->GetSurface()->format->BitsPerPixel));
								}
								break;
						case SDLK_RIGHTBRACKET:
								if(screen->getBitDepth() < DEPTH_32BIT)
								{
									UI_Object::theme.setBitDepth((eBitDepth)(screen->getBitDepth()+1));
									screen->setBitDepth(UI_Object::theme.getBitDepth());
									uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
									UI_Object::theme.updateColors(screen->GetSurface());
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->GetSurface()->format->BitsPerPixel));
								}
								break;
							
//						case SDLK_F6:m.mainWindow->forcePressTab(MAP_TAB);break;
//						case SDLK_F7:m.mainWindow->forcePressTab(SETTINGS_TAB);break;
//						case SDLK_F8:m.mainWindow->forcePressTab(HELP_TAB);break;
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

		if(m.hasBitDepthChanged())
		{
			screen->setBitDepth(UI_Object::theme.getBitDepth());
			UI_Object::theme.updateColors(screen->GetSurface());
			toLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->GetSurface()->format->BitsPerPixel));
		} else
	
		if(m.hasResolutionChanged())
		{
			screen->setResolution(UI_Object::theme.getResolution());
			resolution = screen->getResolutionSize();
			UI_Object::setResolution(resolution);
			std::ostringstream os;
			os.str("");
 			os << resolution.GetWidth() << "x" << resolution.GetHeight();
			toLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
			m.reloadOriginalSize();
		} else 

		if(m.hasFullScreenChanged())
		{
			screen->setFullscreen(efConfiguration.isFullScreen());
			toLog(UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
		}


		
	}
}
					

