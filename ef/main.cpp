#include "guimain.hpp"

#include "configuration.hpp"
#include "../ui/configuration.hpp"
#include "../core/configuration.hpp"

#include "../core/starcraft.hpp"
#include "../core/broodwar.hpp"
#include "../core/warcraft3.hpp"

#include <sstream>
#include "files.hpp"
class ExitInfo
{
	public:
		ExitInfo();
		~ExitInfo();
		bool smoothExit;
};

ExitInfo::ExitInfo():
	smoothExit(false)
{}

ExitInfo::~ExitInfo()
{
	if(!smoothExit)
		toInitLog("If you need help with the error message please post it on www.clawsoftware.de in the forums or contact me at ghoul@clawsoftware.de");
	else
		toInitLog("Exiting...");
}




int main(int argc, char *argv[])
{
	srand(time(NULL));
	std::list<std::string> arguments;
	for(signed int i = 1;i<argc;++i)
		arguments.push_back(argv[i]);

	std::string current_driver;
	if(getenv("SDL_VIDEODRIVER")!=NULL)
		current_driver = getenv("SDL_VIDEODRIVER");

// ------ LOAD CONFIGURATION FILES ------
//	toInitLog(UI_Object::theme.lookUpString(START_LOAD_LANGUAGE_FILES_STRING));
//	dum, weil sind ja noch gar keine Sprachen geladen
	
	toInitLog("Loading language files...");
	UI_Object::theme.loadStringFiles();
	toInitLog(UI_Object::theme.lookUpString(START_LOAD_UI_CONFIGURATION_STRING));
	uiConfiguration.loadConfigurationFile();
	
	toInitLog(UI_Object::theme.lookUpString(START_SET_LANGUAGE_STRING) + UI_Object::theme.lookUpString((eString)(SETTING_ZERO_LANGUAGE_STRING + uiConfiguration.getLanguage())));
	if(!UI_Object::theme.setLanguage(uiConfiguration.getLanguage()))
	{
		toErrorLog("Could not load language, trying default language english... ");
		if(!UI_Object::theme.setLanguage(ENGLISH_LANGUAGE))
		{
			toErrorLog("ERROR (main()): Cannot set any language, please reinstall language files (in 'data/strings/program') or the whole program.");
			return(EXIT_FAILURE);
		}
	}
	std::list<std::string> string_files = findFiles("data", "strings", "help");
	for(std::list<std::string>::iterator j = string_files.begin(); j != string_files.end(); ++j)
		UI_Object::theme.loadHelpChapterStringFile(*j);
	
	toInitLog(UI_Object::theme.lookUpString(START_LOAD_CORE_CONFIGURATION_STRING));
	coreConfiguration.loadConfigurationFile();
	
	toInitLog(UI_Object::theme.lookUpString(START_LOAD_EF_CONFIGURATION_STRING));
	efConfiguration.loadConfigurationFile();
// ------ END LOAD CONFIGURATION FILES -------

// ------ PARSING COMMAND LINE ------
	toInitLog(UI_Object::theme.lookUpString(START_COMMANDO_LINE_1_STRING));
	toInitLog(UI_Object::theme.lookUpString(START_COMMANDO_LINE_2_STRING));
	toInitLog(UI_Object::theme.lookUpString(START_COMMANDO_LINE_3_STRING));
	toInitLog(UI_Object::theme.lookUpString(START_COMMANDO_LINE_4_STRING));
	toInitLog(UI_Object::theme.lookUpString(START_COMMANDO_LINE_5_STRING));
	toInitLog(UI_Object::theme.lookUpString(START_COMMANDO_LINE_6_STRING));
	toInitLog(UI_Object::theme.lookUpString(START_PARSE_COMMAND_LINE_STRING));
	if(!arguments.empty())
		for(std::list<std::string>::const_iterator i = arguments.begin();i!=arguments.end(); ++i)
		{
			if((*i) == "-vo")
			{
				++i;
				if(i==arguments.end())
					toErrorLog(UI_Object::theme.lookUpString(START_WARNING_VO_ARGUMENT_STRING));
				else current_driver = *i;
			} 
			else if((*i) == "-nosound")
			{
				uiConfiguration.setSound(false);
				uiConfiguration.setMusic(false);
				toInitLog(UI_Object::theme.lookUpString(START_INIT_NOSOUND_STRING));
			} 
			else if((*i) == "-640")
			{
				uiConfiguration.setResolution(RESOLUTION_640x480);
			} else if((*i) == "-800")
			{
				uiConfiguration.setResolution(RESOLUTION_800x600);
			} else if((*i) == "-1024")
			{
				uiConfiguration.setResolution(RESOLUTION_1024x768);
			} else if((*i) == "-1280")
			{
				uiConfiguration.setResolution(RESOLUTION_1280x1024);
			} else if((*i) == "-8bit")
			{
				uiConfiguration.setBitDepth(DEPTH_8BIT);
			} else if((*i) == "-16bit")
			{
				uiConfiguration.setBitDepth(DEPTH_16BIT);
			} else if((*i) == "-24bit")
			{
				uiConfiguration.setBitDepth(DEPTH_24BIT);
			} else if((*i) == "-32bit")
			{
				uiConfiguration.setBitDepth(DEPTH_32BIT);
			} else if((*i) == "-fs")
			{
				efConfiguration.setFullScreen(true);
			} else if((*i) == "-window")
			{
				efConfiguration.setFullScreen(false);
			}
		}
// ------ END PARSING COMMAND LINE ------

	
// ------ INIT SDL AND WINDOW ------
	toInitLog(UI_Object::theme.lookUpString(START_INIT_SDL_STRING));
	
	{
		std::ostringstream os;
		os.str("");
		os << "* " << UI_Object::theme.lookUpString(START_AVAILIBLE_GRAPHIC_DRIVERS_STRING);
		std::list<std::string> s = DC::getAvailibleDrivers();
		for(std::list<std::string>::const_iterator i = s.begin(); i!=s.end(); i++)
			os << *i << " ";
		toInitLog(os.str());
	}

	
	switch(DC::chooseDriver(current_driver))
	{
		case NO_DRIVER_ERROR:toInitLog("* " + UI_Object::theme.lookUpFormattedString(START_SDL_USING_DRIVER_STRING, current_driver));break;
		case NO_VIDEO_DRIVERS_AVAILIBLE:toErrorLog("* " + UI_Object::theme.lookUpString(START_ERROR_NO_DRIVER_AVAILIBLE_STRING));return(EXIT_FAILURE);break;
		case SDL_DRIVER_NOT_SUPPORTED:toErrorLog("* " + UI_Object::theme.lookUpFormattedString(START_ERROR_DRIVER_NOT_SUPPORTED_STRING, current_driver));return(EXIT_FAILURE);break;
	}


	UI_Object::theme.setResolution(uiConfiguration.getResolution());
	UI_Object::setResolution(UI_Object::theme.getCurrentResolutionSize());
	UI_Object::theme.setBitDepth(uiConfiguration.getBitDepth());
	
	toInitLog("* " + UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
	Uint32 flags = SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_HWPALETTE|SDL_SRCCOLORKEY|SDL_RLEACCEL|SDL_SRCALPHA|SDL_PREALLOC|SDL_DOUBLEBUF;
	if(efConfiguration.isFullScreen())
		flags |= SDL_FULLSCREEN;
#ifdef _SDL_MIXER_SOUND
	flags |= SDL_INIT_AUDIO;
#endif
	DC* screen = new DC(UI_Object::theme.getCurrentResolutionSize(), UI_Object::theme.getBitDepth(), flags, SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO);
		
	if(!screen->initializationOK())	{
		toErrorLog(UI_Object::theme.lookUpString(START_UNABLE_TO_INIT_SDL_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");
		delete screen;
		return(EXIT_FAILURE);
	}
	
	if ( !screen->valid() ) {
		toErrorLog(UI_Object::theme.lookUpString(START_ERROR_SETTING_VIDEO_MODE_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");
		delete screen;
		return(EXIT_FAILURE);
	}

	UI_Object::theme.initCursors();
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
	{
		std::ostringstream os;
		os << "EVOLUTION FORGE " << CORE_VERSION << " - www.clawsoftware.de";
		SDL_WM_SetCaption(os.str().c_str(),"");
	}
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 1));
	toInitLog(UI_Object::theme.printHardwareInformation());
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATED_SURFACE_STRING) + " " + UI_Object::theme.printSurfaceInformation(screen));
// ------ END INIT SDL AND WINDOW ------

// ------ INIT SOUND ENGINE -------
	bool sound_not_initialized = true;
	if((uiConfiguration.isSound())||(uiConfiguration.isMusic()))
	{
		toInitLog(UI_Object::theme.lookUpString(START_INIT_SOUND_STRING)); 
		if(!UI_Object::sound.init())
		{
			uiConfiguration.setSound(false);
			uiConfiguration.setMusic(false);
		} else
			sound_not_initialized = false;
	}
// ------ END INIT SOUND ENGINE -------


// ------ INIT SDL_TTF ------
	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_SDL_TRUETYPE_FONTS_STRING));
	if(TTF_Init()==-1) {
		toErrorLog(UI_Object::theme.lookUpString(START_INIT_SDL_TTF_ERROR_STRING) + " [\"" + TTF_GetError() + "\"]");
		delete screen;
		return(EXIT_FAILURE);
	}
	atexit(TTF_Quit); 
// ------- END INIT SDL_TTF -------
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));

	BROODWAR bw;
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 1));
	STARCRAFT sc;
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
//	WARCRAFT3 wc3;
//	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 1));

	
// ------- INIT GRAPHIC ENGINE ------
	toInitLog(UI_Object::theme.lookUpString(START_INIT_GRAPHIC_ENGINE_CORE_STRING));
	Main m;
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
	if(!m.initGUI(screen))
	{
		delete screen;
		return(EXIT_FAILURE);
	}
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 1));
	
// ------ CAP FRAMERATE ------
	toInitLog(UI_Object::theme.lookUpString(START_INIT_FRAMERATE_STRING)); 
	unsigned int original_desired_cpu = efConfiguration.getDesiredCPU();
	unsigned int original_desired_framerate = efConfiguration.getDesiredFramerate();
	efConfiguration.setDesiredCPU(99);
	efConfiguration.setDesiredFramerate(15); // for the intro
// ------ END CAP FRAMERATE


	toInitLog(UI_Object::theme.lookUpString(START_SYSTEM_READY_STRING));
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));

//	unsigned int screenshot = 100;
//	unsigned int screenCapturing=0;
// ------ END INIT GRAPHIC ENGINE ------

	
	bool done = false;
	int picture_num = 1100;
	const int INTRO_ANIMATION_FRAMES1 = 64;
	const int INTRO_ANIMATION_FRAMES2 = 128;
	SDL_Event event;
	ExitInfo exitInfo;

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));
// MAIN LOOP
	while(!done)
	{
		bool ignore_rest = false;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					done = true;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if((picture_num!=INTRO_ANIMATION_FRAMES2)&&(picture_num!=INTRO_ANIMATION_FRAMES1)) 
						picture_num = INTRO_ANIMATION_FRAMES1;
					else if(picture_num == INTRO_ANIMATION_FRAMES1)
						picture_num = 9999;

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
					if((picture_num!=INTRO_ANIMATION_FRAMES2)&&(picture_num!=INTRO_ANIMATION_FRAMES1)) 
						picture_num = INTRO_ANIMATION_FRAMES1;
					else if(picture_num == INTRO_ANIMATION_FRAMES1)
					{
						while (SDL_PollEvent(&event));
						picture_num = 9999;
					} else
					if((UI_Object::focus==NULL)||(!UI_Object::focus->addKey(event.key.keysym.sym, event.key.keysym.mod)))
					switch(event.key.keysym.sym)
					{
						case SDLK_r: m.openMenu(OPEN_RACE_MENU);break;
						case SDLK_n: if(event.key.keysym.mod & KMOD_CTRL)
								     m.openMenu(RESTART_CALCULATION);break;
						case SDLK_b: if(event.key.keysym.mod & KMOD_CTRL)
								     m.openMenu(SAVE_BUILD_ORDER);
							     m.openMenu(OPEN_BO_MENU);break;
						case SDLK_g: if(event.key.keysym.mod & KMOD_CTRL)
								     m.openMenu(SAVE_GOAL);
							     else
								     m.openMenu(OPEN_GOAL_MENU);break;
						case SDLK_u: m.openMenu(OPEN_UNIT_MENU);break;
//						case SDLK_a: m.openMenu(ADD_PLAYER);break;
						case SDLK_e: m.openMenu(EDIT_FORCE_LIST);break;
						case SDLK_F1:
						case SDLK_F2:
						case SDLK_F3:
						case SDLK_F4:
						case SDLK_F5:m.mainWindow->activateTabNumber(event.key.keysym.sym - SDLK_F1);break;
							     
						case SDLK_F6:m.mainWindow->activateTab(DATABASE_TAB);break;
						case SDLK_F7:m.mainWindow->activateTab(SETTINGS_TAB);break;
						case SDLK_F8:m.mainWindow->activateTab(HELP_TAB);break;


						case SDLK_HOME:m.wheelToTop();break;
						case SDLK_END:m.wheelToBottom();break;
							  
						case SDLK_PAGEUP:m.wheelUp();m.wheelUp();m.wheelUp();break;
						case SDLK_PAGEDOWN:m.wheelDown();m.wheelDown();m.wheelDown();break;
						case SDLK_UP:m.wheelUp();break;
						case SDLK_DOWN:m.wheelDown();break;
						case SDLK_LEFT:if(!(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT)))
								       break;
						case SDLK_BACKSPACE:m.goBack();break;							
						case SDLK_TAB:
							//UI_Object::rotateEditField();
							break;
						case SDLK_KP_ENTER:
						case SDLK_RETURN:
							if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))
								m.noticeFullscreen();
						break;
						case SDLK_q: 
							if(event.key.keysym.mod & KMOD_CTRL)
								done = true;break;
						case SDLK_ESCAPE:
							if(UI_Object::focus!=NULL)
								UI_Object::focus=NULL;
							else 
								done = true;
							break;
						case SDLK_PAUSE:
								if(m.isAnyOptimizing())
									m.stopAllOptimizing();break;

						case SDLK_SPACE:
								if(m.isAnyOptimizing())
									m.stopAllOptimizing();
								else m.startLastOptimizing();
							break;
//						case SDLK_PRINT:break;
						case SDLK_EQUALS:
							if(UI_Object::theme.getResolution()<RESOLUTION_1280x1024)
							{
								UI_Button::resetButton();
								UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() + 1));
								screen->setResolution(UI_Object::theme.getCurrentResolutionSize());
								UI_Object::setResolution(UI_Object::theme.getCurrentResolutionSize());
								uiConfiguration.setResolution(UI_Object::theme.getResolution());
								std::ostringstream os;os.str("");
								os << UI_Object::max_x << "x" << UI_Object::max_y;
								toInitLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
									m.reloadOriginalSize();
								while (SDL_PollEvent(&event));
							}
							break;
						case SDLK_MINUS:
							if(UI_Object::theme.getResolution()>RESOLUTION_640x480)
							{
								UI_Button::resetButton();
								UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() - 1));
								screen->setResolution(UI_Object::theme.getCurrentResolutionSize());
								UI_Object::setResolution(UI_Object::theme.getCurrentResolutionSize());
								uiConfiguration.setResolution(UI_Object::theme.getResolution());
								std::ostringstream os;os.str("");
 								os << UI_Object::max_x << "x" << UI_Object::max_y;
								toInitLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
								m.reloadOriginalSize();
								while (SDL_PollEvent(&event));
							}
							break;
						case SDLK_LEFTBRACKET:
							if(screen->getBitDepth() > DEPTH_8BIT)
							{
								UI_Object::theme.setBitDepth((eBitDepth)(screen->getBitDepth()-1));
								screen->setBitDepth(UI_Object::theme.getBitDepth());
								uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
								UI_Object::theme.updateColors(screen->getSurface());
								// TODO bitDepth im theme aendern! ?
								toInitLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->getSurface()->format->BitsPerPixel));
								while (SDL_PollEvent(&event));
							}
							break;
						case SDLK_RIGHTBRACKET:
							if(screen->getBitDepth() < DEPTH_32BIT)
							{
								UI_Object::theme.setBitDepth((eBitDepth)(screen->getBitDepth()+1));
								screen->setBitDepth(UI_Object::theme.getBitDepth());
								uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
								UI_Object::theme.updateColors(screen->getSurface());
								toInitLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->getSurface()->format->BitsPerPixel));
								ignore_rest=true;
								while (SDL_PollEvent(&event));
							}
							break;
							
						default:break;
					}
					break;
				default:break;
			}
		}
		m.poll(MESSAGE_TICKS);
		
		if(picture_num==INTRO_ANIMATION_FRAMES2)
		{
			m.process();
		}

		if(m.hasBitDepthChanged())
		{
			screen->setBitDepth(UI_Object::theme.getBitDepth());
			UI_Object::theme.updateColors(screen->getSurface());
			toInitLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->getSurface()->format->BitsPerPixel));
			m.needRedraw();
			UI_Object::resetWindow();
			
		}
		if(m.hasResolutionChanged())
		{
			SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
			screen->setResolution(UI_Object::theme.getCurrentResolutionSize());
			UI_Object::setResolution(UI_Object::theme.getCurrentResolutionSize());
			std::ostringstream os;os.str("");
 			os << UI_Object::max_x << "x" << UI_Object::max_y;
			toInitLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
			m.reloadOriginalSize();
			UI_Object::resetWindow();
			SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));
		}
		if(m.hasFullScreenChanged())
		{
			screen->setFullscreen(efConfiguration.isFullScreen());
			toInitLog(UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
		}
		if(m.hasThemeChanged())
		{
			m.needRedraw();
		}

		if(m.hasGameTypeChanged())
		{
			SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
			bool new_game_type = true;
			switch(coreConfiguration.getGameType())
			{
				case 1:sc.init();toInitLog("StarCraft");break;
				case 2:bw.init();toInitLog("BroodWar");break;
//				case 3:wc3.init();toInitLog("WarCraft");break;
				default:toErrorLog("DEBUG (main()): Game type not initialized.");new_game_type = false;break; // not initialized
			}
			if(new_game_type)
			{
				std::ostringstream harvest_file;
#ifdef __linux__
				harvest_file << GAME::gameDirectory << "/" << "harvest.hvt";
#elif __WIN32__
				harvest_file << GAME::gameDirectory << "\\" << "harvest.hvt";
#endif 
				if(!FILES::loadHarvestFile(harvest_file.str()))
				{
					toInitLog("Generating harvest data...");
					GAME::initResources();
					FILES::saveHarvestFile(harvest_file.str());
				}
				SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 1));

				if(!GAME::setLanguage(UI_Object::theme.getLanguage()))
				{
					toErrorLog("GAME::setLanguage failed!");
					toErrorLog(UI_Object::theme.getLanguage());
				}
				SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
					
				if(!m.initCore())
				{
					delete screen;
					return(EXIT_FAILURE);
				}
				SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 1));
				m.initializeGame(0);
				SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
				toInitLog(UI_Object::theme.lookUpString(START_MAIN_INIT_COMPLETE_STRING));
			}
			SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));

			UI_Object::sound.playMusic(LALA_MUSIC); 
		}

		m.resetDataChange();

		m.poll(PROCESS_TICKS);

// ------ SOUND ENGINE -------
		if(((uiConfiguration.isSound())||(uiConfiguration.isMusic()))&&(sound_not_initialized))
		{
			toInitLog(UI_Object::theme.lookUpString(START_INIT_SOUND_STRING)); 
			if(!UI_Object::sound.init())
			{
				uiConfiguration.setSound(false);
				uiConfiguration.setMusic(false);
			} else
			{
				sound_not_initialized = false;
				if(uiConfiguration.isMusic())
					UI_Object::sound.playMusic(LALA_MUSIC);

//				- NO_FACILITY checken... evtl orders darueber pruefen...
				
			}
		} else 
		if((!uiConfiguration.isSound()) && (!uiConfiguration.isMusic()) && (!sound_not_initialized))
		{
			UI_Object::sound.releaseSoundEngine();
			sound_not_initialized = true;
		} else
			
		if(!sound_not_initialized)
		{
			if(uiConfiguration.isMusic())
			{
			/*	float vol;
				
				music_channel->getVolume(&vol);
				if(uiConfiguration.getMusicVolume() > vol*100)
					music_channel->setVolume(vol+0.01);
				else
				if(uiConfiguration.getMusicVolume() < vol*100)
					music_channel->setVolume(vol-0.01);
			
				bool is_playing = false;
				music_channel->isPlaying(&is_playing);
				if((!is_playing)&&(current_music!=MAX_SOUNDS))
					UI_Object::sound.sound->playSound(FMOD_CHANNEL_FREE, UI_Object::sound.lookUpSound(current_music), 0, &music_channel);
		*/
//				UI_Object::sound.playMusic(LALA_MUSIC); // TODO, muss nicht unbedingt LALA sein...
			} else
				UI_Object::sound.stopMusic();

			if(uiConfiguration.isSound())
				UI_Object::sound.processChannels();
			else UI_Object::sound.clearChannels();
			UI_Object::sound.update();
		}
		UI_Object::sound.clearSoundsToPlay();
		m.poll(SOUND_TICKS);

// ------ END SOUND ENGINE -------

// ------ DRAWING AND PROCESSING ------
	
		if(picture_num==INTRO_ANIMATION_FRAMES2)
		{
			m.draw(screen);
			if(uiConfiguration.isUnloadGraphics())
				UI_Object::theme.unloadGraphics();
		} else

		if((picture_num>=990)&&(picture_num<=1100))
		{
			std::ostringstream os; os.str("");
			os << "intro_ani/ani" << picture_num << ".bmp";
			SDL_Surface* picture = IMG_Load(os.str().c_str());
			picture_num--;
			Point p = Point((UI_Object::max_x - picture->w)/2, (UI_Object::max_y - picture->h)/2);
			Size s = Size(picture->w, picture->h);
			if(picture_num==1010)
			{
				UI_Object::sound.playSound(INTRO_SOUND, UI_Object::max_x/2);
				screen->setTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
				screen->setFont(UI_Object::theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
				screen->DrawText("Brought to you by...", UI_Object::max_x/2 - picture->w/3, p.y - 15);
				DC::addRectangle(Rect(UI_Object::max_x/2 - picture->w/3, p.y - 20, picture->w, 20));
			}
			
//			screen->setBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
//			screen->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
//			screen->DrawEdgedRoundedRectangle(Rect(p - Size(5,5), s + Size(10,10)), 4);
//			screen->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
//			screen->DrawEdgedRoundedRectangle(Rect(p - Size(3,3), s + Size(6,6)), 4);
			screen->DrawBitmap(picture, p);
			DC::addRectangle(Rect(p - Size(5,5), s + Size(10, 10)));
			SDL_FreeSurface(picture);
		} else if(picture_num==989)
			picture_num=1;
		else if(picture_num<=16)
		{
			std::ostringstream os; os.str("");
			os << "intro_ani/claw_gui" << picture_num << ".png";
			SDL_Surface* picture = IMG_Load(os.str().c_str());
			Point p = Point(picture->w-5, (UI_Object::max_y - 2*picture->h));
			Size s = Size(picture->w, picture->h);
			if(picture_num==1)
				UI_Object::sound.playSound(RING_SOUND, p.x + picture->w/2);
			
			screen->setBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
			screen->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(5,5), s + Size(10,10)), 4);
			screen->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(3,3), s + Size(6,6)), 4);
			screen->DrawBitmap(picture, p);
			if(picture_num == 1)
			{
				screen->setTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
				screen->setFont(UI_Object::theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
				screen->DrawText("ClawGUI", p.x + 5, p.y + 10 + picture->h);
				screen->DrawText("http://www.clawsoftware.de/", p.x + 5, p.y + 20 + picture->h);
			}
			
			DC::addRectangle(Rect(p - Size(5,5), s + Size(75,40)));
			picture_num++;
			SDL_FreeSurface(picture);
		}  else if(picture_num<=32)
		{
			std::ostringstream os; os.str("");
			os << "intro_ani/sdl" << picture_num-16 << ".png";
			SDL_Surface* picture = IMG_Load(os.str().c_str());
	
			Point p = Point((UI_Object::max_x+5 - 2*picture->w), (UI_Object::max_y - 2*picture->h));
			Size s = Size(picture->w, picture->h);
		
			if(picture_num==17)
				UI_Object::sound.playSound(RING_SOUND, p.x + picture->w/2);
			
			screen->setBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
			screen->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(5,5), s + Size(10,10)), 4);
			screen->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(3,3), s + Size(6,6)), 4);
			screen->DrawBitmap(picture, p);

			if(picture_num == 17)
			{
				screen->setTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
				screen->setFont(UI_Object::theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
				screen->DrawText("Simple Directmedia Library", p.x + 5, p.y + 10 + picture->h);
				screen->DrawText("http://libsdl.org/", p.x + 5, p.y + 20 + picture->h);
			}
			
			DC::addRectangle(Rect(p - Size(5,5), s + Size(75,40)));
			picture_num++;

			SDL_FreeSurface(picture);
		} 
		else if(picture_num<=48)
		{
#ifdef _FMOD_SOUND
			std::ostringstream os; os.str("");
			os << "intro_ani/fmod" << picture_num-32 << ".png";
			SDL_Surface* picture = IMG_Load(os.str().c_str());

			Point p = Point((UI_Object::max_x - picture->w)/2, (UI_Object::max_y - 2*picture->h));
			Size s = Size(picture->w, picture->h);
			
			if(picture_num==33)
				UI_Object::sound.playSound(RING_SOUND, p.x + picture->w/2);
			screen->setBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
			screen->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(5,5), s + Size(10,10)), 4);
			screen->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(3,3), s + Size(6,6)), 4);
			screen->DrawBitmap(picture, p);
			if(picture_num == 33)
			{
				screen->setTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
				screen->setFont(UI_Object::theme.lookUpFont(SMALL_FONT));
				screen->DrawText("FMOD Sound System http://www.fmod.org", p.x-20, p.y+10+picture->h);
				screen->DrawText("(C) Firelight Technologies Pty, Ltd., 1994-2005", p.x-20, p.y+20+picture->h);
			}
			DC::addRectangle(Rect(p - Size(10,5), s + Size(80,40)));
			SDL_FreeSurface(picture);				
#endif
			picture_num++;

		}
		else if(picture_num<(INTRO_ANIMATION_FRAMES1-1))
			picture_num++;
		else if(picture_num == INTRO_ANIMATION_FRAMES1)
		{
				picture_num = 9999; // ~
		} else
		if(picture_num == 9999)
		{
			picture_num = INTRO_ANIMATION_FRAMES2;
			UI_Object::sound.playMusic(DIABLO_MUSIC);
			efConfiguration.setDesiredCPU(original_desired_cpu);
			efConfiguration.setDesiredFramerate(original_desired_framerate);
			SDL_ShowCursor(SDL_ENABLE);
		}

/// ------ END DRAWING ------

// ------ SCREENCAPTURE ------ 
/*                if(screenCapturing==100) {
			std::ostringstream os;os.str("");os << "shot" << screenshot << ".bmp";
                        SDL_SaveBMP(screen->getSurface() , os.str().c_str());
			++screenshot;
		}
		if(screenCapturing>0) {
			--screenCapturing;
			std::ostringstream os;os.str("");os << "shot" << (screenshot-1) << ".bmp" << " saved (" << (UI_Object::max_x * UI_Object::max_y * (int)(screen->getSurface()->format->BitsPerPixel))/1024 << "kb)";
                        screen->DrawText(os.str(), 50, 300);
                }*/
// ------ END SCREENCAPTURE -----
		screen->updateScreen();
		m.poll(DRAW_TICKS);
// ------ FRAMERATE AND CALCULATION ------	
		m.newGeneration();
// ------ END FRAMERATE AND CALCULATION 
	}
	toInitLog("* " + UI_Object::theme.lookUpString(END_CLOSING_SDL_STRING));
	delete screen;
	exitInfo.smoothExit = true;
	return(EXIT_SUCCESS);
}				

