#include "guimain.hpp"
#include "fpssystem.hpp"
#include "savebox.hpp"
#include "configuration.hpp"
#include "../ui/configuration.hpp"
#include "../core/configuration.hpp"


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
	{
		toInitLog("If you need help with the error message please post it on www.clawsoftware.de in the forums or contact me at ghoul@clawsoftware.de");
	} else
		toInitLog("Exiting ...");
}

const bool init_sound()
{
	// TODO start a 'watchdog' thread (FMOD waits indefinitely if the sound is currently used!)
	unsigned int version;
	
        if(!UI_Theme::ERRCHECK(FMOD::System_Create(&UI_Object::theme.sound))) 
		return(false);
	if(!UI_Theme::ERRCHECK(UI_Object::theme.sound->getVersion(&version))) 
		return(false);

	if (version < FMOD_VERSION)
        {
		std::ostringstream os;
		os << "Error!  You are using an old version of FMOD " << version << ". This program requires " << FMOD_VERSION << ".";
		toErrorLog(os.str());
	       	return(false);
	}
	if(!UI_Theme::ERRCHECK(UI_Object::theme.sound->init(32, FMOD_INIT_NONREALTIME, 0)))
		return(false);

	UI_Object::theme.printSoundInformation();
	return(true);
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
	toInitLog("Initializing language files...");
	std::list<std::string> stringFiles = findFiles("settings", "strings", "");
	for(std::list<std::string>::iterator j = stringFiles.begin(); j!=stringFiles.end(); ++j)
		UI_Object::theme.loadStringFile(*j);
	
	toInitLog(UI_Object::theme.lookUpString(START_LOAD_CONFIGURATION_STRING));
	uiConfiguration.loadConfigurationFile();
	if(!UI_Object::theme.setLanguage(uiConfiguration.getLanguage()))
	{
		toErrorLog("Could not load language, trying default language english... ");
		if(!UI_Object::theme.setLanguage(ENGLISH_LANGUAGE))
		{
			toErrorLog("ERROR (main()): Cannot set any language, please reinstall language files (in 'settings/strings/') or the whole program.");
			return(EXIT_FAILURE);
		}
	}
	stringFiles = findFiles("settings", "help", "");
	for(std::list<std::string>::iterator j = stringFiles.begin(); j!=stringFiles.end(); ++j)
		UI_Object::theme.loadHelpChapterStringFile(*j);
	
	coreConfiguration.loadConfigurationFile();
	efConfiguration.loadConfigurationFile();
// ------ END LOAD CONFIGURATION FILES -------

// ------ PARSING COMMAND LINE ------
	toInitLog("You can set some parameters of the sound and graphic engine with the command line:");
	toInitLog("-vo <driver> sets the video driver, see below for a list of availible video drivers");
	toInitLog("-nosound deactivates sound and music");
	toInitLog("-640, -800, -1024, -1280 sets the video resolution");
	toInitLog("-8bit, -16bit, -24bit, -32bit sets the bit depth");
	toInitLog("-fs, -window sets the program to fullscreen or to window mode");
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
			} else 
			if((*i) == "-nosound")
			{
				uiConfiguration.setSound(false);
				uiConfiguration.setMusic(false);
				toInitLog("Parameter -nosound causes the sound engine to be deactivated, To activate sound in the program go to the 'settings'.");
			} else if((*i) == "-640")
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
	bool sound_not_initialized = true;
// ------ INIT SOUND ENGINE -------
	std::list<FMOD::Channel*> sound_channel;
	FMOD::Channel* music_channel;
	eSound current_music = MAX_SOUNDS;
	if((uiConfiguration.isSound())||(uiConfiguration.isMusic()))
	{
		toInitLog(UI_Object::theme.lookUpString(START_INIT_SOUND_STRING)); 
		if(!init_sound())
		{
			uiConfiguration.setSound(false);
			uiConfiguration.setMusic(false);
		} else
			sound_not_initialized = false;
	}
// ------ END INIT SOUND ENGINE -------

	
// ------ INIT SDL AND WINDOW ------
	toInitLog(UI_Object::theme.lookUpString(START_INIT_SDL_STRING));
	
	{
		std::ostringstream os;
		os.str("");
		std::list<std::string> s = DC::getAvailibleDrivers();
		for(std::list<std::string>::const_iterator i = s.begin(); i!=s.end(); i++)
			os << *i << " ";
		toInitLog("* Availible graphic drivers: " + os.str());
	}

	
	switch(DC::chooseDriver(current_driver))
	{
		case NO_DRIVER_ERROR:toInitLog("* " + UI_Object::theme.lookUpFormattedString(START_SDL_USING_DRIVER_STRING, current_driver));break;
		case NO_VIDEO_DRIVERS_AVAILIBLE:toErrorLog("* " + UI_Object::theme.lookUpString(START_ERROR_NO_DRIVER_AVAILIBLE_STRING));return(EXIT_FAILURE);break;
		case SDL_DRIVER_NOT_SUPPORTED:toErrorLog("* " + UI_Object::theme.lookUpFormattedString(START_ERROR_DRIVER_NOT_SUPPORTED_STRING, current_driver));return(EXIT_FAILURE);break;
	}


	UI_Object::theme.setResolution(uiConfiguration.getResolution());
	UI_Object::setResolution(UI_Object::theme.getResolutionSize());
	UI_Object::theme.setBitDepth(uiConfiguration.getBitDepth());
	
	toInitLog("* " + UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
	
	DC* screen = new DC(UI_Object::theme.getResolutionSize(), UI_Object::theme.getBitDepth(), (efConfiguration.isFullScreen()?SDL_FULLSCREEN:0)|SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_HWPALETTE|SDL_SRCCOLORKEY|SDL_RLEACCEL|/*SDL_SRCALPHA|*/SDL_PREALLOC|SDL_DOUBLEBUF, SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO/* | SDL_INIT_AUDIO*/); // TODO ueber Flag setzen ob Audio initialisiert werden soll


		
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

	{
		std::ostringstream os;
		os << "EVOLUTION FORGE " << CORE_VERSION << " - www.clawsoftware.de";
		SDL_WM_SetCaption(os.str().c_str(),"");
	}
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_ShowCursor(SDL_DISABLE);
	toInitLog(DC::printHardwareInformation());
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATED_SURFACE_STRING) + " " + DC::printSurfaceInformation(screen));
// ------ END INIT SDL AND WINDOW ------

// ------ INIT SDL_TTF ------
	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_SDL_TRUETYPE_FONTS_STRING));
	if(TTF_Init()==-1) {
		toErrorLog(std::string("TTF_Init: ") + " [TTF ERROR: \"" + TTF_GetError() + "\"]");
		delete screen;
		return(EXIT_FAILURE);
	}
	atexit(TTF_Quit); 
// ------- END INIT SDL_TTF -------

	
// ------- INIT GRAPHIC ENGINE ------
	toInitLog(UI_Object::theme.lookUpString(START_INIT_GRAPHIC_ENGINE_CORE_STRING));
	Main m;
	if((!m.initGUI(screen))||(!m.initCore()))
	{
		delete screen;
		return(EXIT_FAILURE);
	}

	toInitLog(UI_Object::theme.lookUpString(START_MAIN_INIT_COMPLETE_STRING));	
	m.initializeGame(0);

	unsigned int screenshot = 100;
//	if(efConfiguration.isAutoSaveRuns())
//		m.startAllOptimizing();
//	else
//		m.stopAllOptimizing(); TODO

	unsigned int screenCapturing=0;
	SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));

//	toLog(SDL_SetGamma(1.2, 1.5, 1.5)); TODO?
// ------ END INIT GRAPHIC ENGINE ------

// ------ CAP FRAMERATE ------
	toInitLog(UI_Object::theme.lookUpString(START_INIT_FRAMERATE_STRING)); 
	unsigned int original_desired_cpu = efConfiguration.getDesiredCPU();
	unsigned int original_desired_framerate = efConfiguration.getDesiredFramerate();
	efConfiguration.setDesiredCPU(99);
	efConfiguration.setDesiredFramerate(15); // for the intro
	FPS_SYSTEM* fps = new FPS_SYSTEM();
// ------ END CAP FRAMERATE
	
	toInitLog(UI_Object::theme.lookUpString(START_SYSTEM_READY_STRING));
	
//				- Introwindow mit languageauswahl (bzw. schwarzer Bildschirm mit raceMenu :o )
	
// MAIN LOOP
	bool done = false;
	int picture_num = 1100;
	const int INTRO_ANIMATION_FRAMES = 65;
	SDL_Event event;
	ExitInfo exitInfo;
	while(!done)
	{
//TODO
//		for(std::list<UI_Object*>::iterator i = UI_Object::processArray.begin(); i!=UI_Object::processArray.end(); ++i)
//			(*i)->process();
//		{
//			std::ostringstream os;os.str("");os << "length :" << UI_Object::processArray.size();toLog(os.str());
//		}
	
//		UI_Object::copyToNextProcessArray();
	
		bool ignore_rest = false;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					done = true;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(picture_num!=INTRO_ANIMATION_FRAMES) picture_num = 9999;
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
					if(picture_num!=INTRO_ANIMATION_FRAMES) 
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
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() + 1));
									screen->setResolution(UI_Object::theme.getResolutionSize());
									UI_Object::setResolution(UI_Object::theme.getResolutionSize());
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
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() - 1));
									screen->setResolution(UI_Object::theme.getResolutionSize());
									UI_Object::setResolution(UI_Object::theme.getResolutionSize());
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
		fps->poll(MESSAGE_TICKS);
		
		
		if(picture_num==INTRO_ANIMATION_FRAMES)
			m.process();

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
			screen->setResolution(UI_Object::theme.getResolutionSize());
			UI_Object::setResolution(UI_Object::theme.getResolutionSize());
			std::ostringstream os;os.str("");
 			os << UI_Object::max_x << "x" << UI_Object::max_y;
			toInitLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
			m.reloadOriginalSize();
			UI_Object::resetWindow();
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

		m.resetDataChange();

		fps->poll(PROCESS_TICKS);

// ------ SOUND ENGINE -------
		if(((uiConfiguration.isSound())||(uiConfiguration.isMusic()))&&(sound_not_initialized))
		{
			toInitLog(UI_Object::theme.lookUpString(START_INIT_SOUND_STRING)); 
			if(!init_sound())
			{
				uiConfiguration.setSound(false);
				uiConfiguration.setMusic(false);
			} else
			{
				sound_not_initialized = false;
				if(uiConfiguration.isMusic())
					current_music = LALA_SOUND;

//				- NO_FACILITY checken... evtl orders darueber pruefen...
				
			}
		} else 
		if((!uiConfiguration.isSound())&&(!uiConfiguration.isMusic())&&(!sound_not_initialized))
		{
			UI_Object::theme.releaseSoundEngine();
			sound_not_initialized = true;
		} else
			
		if(!sound_not_initialized)
		{
			if(uiConfiguration.isMusic())
			{
				float vol;
				music_channel->getVolume(&vol);
				if(uiConfiguration.getMusicVolume() > vol*100)
					music_channel->setVolume(vol+0.01);
				else
				if(uiConfiguration.getMusicVolume() < vol*100)
					music_channel->setVolume(vol-0.01);
				bool is_playing = false;
				music_channel->isPlaying(&is_playing);
				if((!is_playing)&&(current_music!=MAX_SOUNDS))
					UI_Object::theme.sound->playSound(FMOD_CHANNEL_FREE, UI_Object::theme.lookUpSound(current_music), 0, &music_channel);
			} else
				music_channel->stop();

			if(uiConfiguration.isSound())
			{
				for(std::list<FMOD::Channel*>::iterator i = sound_channel.begin(); i!=sound_channel.end(); i++)
				{
					bool is_playing = false;
					(*i)->isPlaying(&is_playing);
					if(!is_playing)
						i = sound_channel.erase(i);
				}

				for(std::list<std::pair<FMOD::Sound*,float> >::iterator i = UI_Object::theme.soundsToPlay.begin(); i != UI_Object::theme.soundsToPlay.end(); ++i)
					if((sound_channel.size() < uiConfiguration.getChannels()))
					{
						FMOD::Channel* mychannel = NULL;
						UI_Object::theme.sound->playSound(FMOD_CHANNEL_FREE, i->first, 0, &mychannel);
						mychannel->setPan(i->second);
						mychannel->setVolume((float)(uiConfiguration.getSoundVolume())/100.0);
						sound_channel.push_back(mychannel);
					}
			} else
				sound_channel.clear();
			UI_Object::theme.sound->update();
		}
		UI_Object::theme.soundsToPlay.clear();
		fps->poll(SOUND_TICKS);

// ------ END SOUND ENGINE -------


// ------ DRAWING AND PROCESSING ------
	
		if(picture_num==INTRO_ANIMATION_FRAMES)
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
				UI_Object::theme.playSound(INTRO_SOUND, UI_Object::max_x/2);
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
				UI_Object::theme.playSound(RING_SOUND, p.x + picture->w/2);
			
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
				UI_Object::theme.playSound(RING_SOUND, p.x + picture->w/2);

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
		} else if(picture_num<=48)
		{
			std::ostringstream os; os.str("");
			os << "intro_ani/fmod" << picture_num-32 << ".png";
			SDL_Surface* picture = IMG_Load(os.str().c_str());

			Point p = Point((UI_Object::max_x - picture->w)/2, (UI_Object::max_y - 2*picture->h));
			Size s = Size(picture->w, picture->h);
			
			if(picture_num==33)
				UI_Object::theme.playSound(RING_SOUND, p.x + picture->w/2);
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
				screen->DrawText("FMOD Sound System http://www.fmod.org", p.x, p.y+10+picture->h);
				screen->DrawText("(C) Firelight Technologies Pty, Ltd., 1994-2005", p.x, p.y+20+picture->h);
			}
			DC::addRectangle(Rect(p - Size(10,5), s + Size(80,40)));
			picture_num++;

			SDL_FreeSurface(picture);				
		}
		else if(picture_num<(INTRO_ANIMATION_FRAMES-1))
		{
			picture_num++;
//			if(picture_num == (INTRO_ANIMATION_FRAMES-1))
//				picture_num = 9999; // ~
		} else
			
		if(picture_num == 9999)
		{
			picture_num = INTRO_ANIMATION_FRAMES;
			current_music = LALA_SOUND;
			efConfiguration.setDesiredCPU(original_desired_cpu);
			efConfiguration.setDesiredFramerate(original_desired_framerate);
			std::ostringstream os;
			os << efConfiguration.getDesiredFramerate();
			toInitLog("* setting desired framerate to " + os.str());
			os.str("");	
			os << efConfiguration.getDesiredCPU();
			toInitLog("* setting desired CPU usage to " + os.str() + "%");
			SDL_ShowCursor(SDL_ENABLE);
		}

/// ------ END DRAWING ------

//
//
// 
// ------ FPS DEBUG 

		if((efConfiguration.isShowDebug())&&(picture_num==INTRO_ANIMATION_FRAMES)&&(!m.isIntro()))
			fps->draw(screen);
// ------ END FPS DEBUG
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
		fps->poll(DRAW_TICKS);
		
		
		
// ------ FRAMERATE AND CALCULATION ------	
		fps->delay(m.isAnyOptimizing());
		fps->poll(IDLE_TICKS);
		while(fps->allowCalculation())
			m.newGeneration();
		
// ------ END FRAMERATE AND CALCULATION 
		fps->poll(GENERATION_TICKS);

		fps->process();

		fps->updateConfiguration();
	
		
	}
	delete fps;
	toInitLog("* Closing SDL...");
	delete screen;
	exitInfo.smoothExit = true;
	return(EXIT_SUCCESS);
}				

