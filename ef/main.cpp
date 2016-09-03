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
		toLog("If you need help with the error message please post it on www.clawsoftware.de in the forums or contact me at ghoul@clawsoftware.de");
	} else
		toLog("Exiting ...");
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
	toLog("Initializing language files...");
	std::list<std::string> stringFiles = findFiles("settings", "strings", "");
	for(std::list<std::string>::iterator j = stringFiles.begin(); j!=stringFiles.end(); ++j)
		UI_Object::theme.loadStringFile(*j);
	
	toLog(UI_Object::theme.lookUpString(START_LOAD_CONFIGURATION_STRING));
	uiConfiguration.loadConfigurationFile();
	if(!UI_Object::theme.setLanguage(uiConfiguration.getLanguage()))
	{
		toLog("Trying default language english... ");
		if(!UI_Object::theme.setLanguage(ENGLISH_LANGUAGE))
		{
			toLog("ERROR (main()): Cannot set any language, please reinstall language files (in 'settings/strings/') or the whole program.");
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
	toLog(UI_Object::theme.lookUpString(START_PARSE_COMMAND_LINE_STRING));
	if(!arguments.empty())
		for(std::list<std::string>::const_iterator i = arguments.begin();i!=arguments.end(); ++i)
		{
			toLog(*i);
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

// ------ INIT SOUND ENGINE -------
	toLog(UI_Object::theme.lookUpString(START_INIT_SOUND_STRING)); 
	
	unsigned int version;
	std::list<FMOD::Channel*> channel;
	
        if(!UI_Theme::ERRCHECK(FMOD::System_Create(&UI_Object::theme.sound))) 
		return(EXIT_FAILURE);
	if(!UI_Theme::ERRCHECK(UI_Object::theme.sound->getVersion(&version))) 
		return(EXIT_FAILURE);

	if (version < FMOD_VERSION)
        {
		std::ostringstream os;
		os << "Error!  You are using an old version of FMOD " << version << ". This program requires " << FMOD_VERSION << ".";
		toLog(os.str());
	        return(EXIT_FAILURE);
	}
	if(!UI_Theme::ERRCHECK(UI_Object::theme.sound->init(32, FMOD_INIT_NONREALTIME, 0)))
		return(EXIT_FAILURE);

	UI_Object::theme.printSoundInformation();

// ------ END INIT SOUND ENGINE -------

	
// ------ INIT SDL AND WINDOW ------
	toLog(UI_Object::theme.lookUpString(START_INIT_SDL_STRING));
	
	{
		std::ostringstream os;
		os.str("");
		std::list<std::string> s = DC::getAvailibleDrivers();
		for(std::list<std::string>::const_iterator i = s.begin(); i!=s.end(); i++)
			os << *i << " ";
		toLog("* Availible graphic drivers: " + os.str());
	}

	
	switch(DC::chooseDriver(current_driver))
	{
		case NO_DRIVER_ERROR:toLog("* " + UI_Object::theme.lookUpFormattedString(START_SDL_USING_DRIVER_STRING, current_driver));break;
		case NO_VIDEO_DRIVERS_AVAILIBLE:toLog("* " + UI_Object::theme.lookUpString(START_ERROR_NO_DRIVER_AVAILIBLE_STRING));return(EXIT_FAILURE);break;
		case SDL_DRIVER_NOT_SUPPORTED:toLog("* " + UI_Object::theme.lookUpFormattedString(START_ERROR_DRIVER_NOT_SUPPORTED_STRING, current_driver));return(EXIT_FAILURE);break;
	}


	UI_Object::theme.setResolution(uiConfiguration.getResolution());
	UI_Object::setResolution(UI_Object::theme.getResolutionSize());
	UI_Object::theme.setBitDepth(uiConfiguration.getBitDepth());
	
	toLog("* " + UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
	
	DC* screen = new DC(UI_Object::theme.getResolutionSize(), UI_Object::theme.getBitDepth(), (efConfiguration.isFullScreen()?SDL_FULLSCREEN:0)|SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_HWPALETTE|SDL_SRCCOLORKEY|SDL_RLEACCEL|SDL_SRCALPHA|SDL_PREALLOC|SDL_DOUBLEBUF, SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO | SDL_INIT_AUDIO); // TODO ueber Flag setzen ob Audio initialisiert werden soll


		
	if(!screen->initializationOK())	{
		toLog(UI_Object::theme.lookUpString(START_UNABLE_TO_INIT_SDL_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");
		delete screen;
		return(EXIT_FAILURE);
	}
	
	if ( !screen->valid() ) {
		toLog(UI_Object::theme.lookUpString(START_ERROR_SETTING_VIDEO_MODE_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");
		delete screen;
		return(EXIT_FAILURE);
	}
	

	SDL_WM_SetCaption("EVOLUTION FORGE BETA v1.70 - www.clawsoftware.de","");
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_ShowCursor(SDL_DISABLE);
	toLog(DC::printHardwareInformation());
	toLog("* " + UI_Object::theme.lookUpString(START_CREATED_SURFACE_STRING) + " " + DC::printSurfaceInformation(screen));

//	SDL_SetCursor(UI_Object::theme.lookUpCursor(HAND_CURSOR, 0));
// ------ END INIT SDL AND WINDOW ------

// ------ INIT SDL_TTF ------
	toLog("* " + UI_Object::theme.lookUpString(START_INIT_SDL_TRUETYPE_FONTS_STRING));
	if(TTF_Init()==-1) {
		toLog(std::string("TTF_Init: ") + " [TTF ERROR: \"" + TTF_GetError() + "\"]");
		delete screen;
		return(EXIT_FAILURE);
	}
	atexit(TTF_Quit); 
// ------- END INIT SDL_TTF -------

	
// ------- INIT GRAPHIC ENGINE ------
	toLog(UI_Object::theme.lookUpString(START_INIT_GRAPHIC_ENGINE_CORE_STRING));
	Main m;
	if((!m.initGUI(screen))||(!m.initCore()))
	{
		delete screen;
		return(EXIT_FAILURE);
	}

	toLog(UI_Object::theme.lookUpString(START_MAIN_INIT_COMPLETE_STRING));	
	m.initializeGame(0);

	unsigned int screenshot = 100;
//	if(efConfiguration.isAutoSaveRuns())
//		m.startAllOptimizing();
//	else
//		m.stopAllOptimizing(); TODO

	unsigned int screenCapturing=0;


// ------ END INIT GRAPHIC ENGINE ------



// ------ INTRO PICTURE ------
//	SDL_Surface* progress = SDL_LoadBMP("data/bitmaps/bar.bmp");
//	Bitmap claw("data/bitmaps/clawsoftware.bmp");
//	screen->DrawBitmap(progress, (UI_Object::max_x - progress->w)/2, (UI_Object::max_y - progress->h)/2-60);
//	screen->DrawBitmap(claw, UI_Object::max_x - claw->w, UI_Object::max_y - claw->h);
//	screen->SetPen(Pen(Color(screen->GetSurface(), 255, 255, 255), 1, SOLID_PEN_STYLE));
//	screen->SetBrush(Brush(Color(screen->GetSurface(), 100, 150, 255), SOLID_BRUSH_STYLE));
// ------ END INTRO PICTURE -------



// ------ CAP FRAMERATE ------
	toLog(UI_Object::theme.lookUpString(START_INIT_FRAMERATE_STRING)); 
	unsigned int original_desired_cpu = efConfiguration.getDesiredCPU();
	unsigned int original_desired_framerate = efConfiguration.getDesiredFramerate();
	efConfiguration.setDesiredCPU(99);
	efConfiguration.setDesiredFramerate(15); // for the intro
	FPS_SYSTEM* fps = new FPS_SYSTEM();
// ------ END CAP FRAMERATE
	
	toLog(UI_Object::theme.lookUpString(START_SYSTEM_READY_STRING));
	UI_Theme::ERRCHECK(UI_Object::theme.lookUpSound(LALA_SOUND)->setLoopCount(-1));
	

	
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
					if(picture_num==INTRO_ANIMATION_FRAMES-1) picture_num = 9999;
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
					if(picture_num==INTRO_ANIMATION_FRAMES-1) picture_num = 9999;
					if((UI_Object::focus==NULL)||(!UI_Object::focus->addKey(event.key.keysym.sym, event.key.keysym.mod)))
					switch(event.key.keysym.sym)
					{
						case SDLK_TAB:
							//UI_Object::theme.sound->playSound(FMOD_CHANNEL_FREE, UI_Object::theme.lookUpSound(MOUSEOVER_SOUND), 0, &channel[1]);
							//UI_Object::rotateEditField();
							break;
						case SDLK_KP_ENTER:
						case SDLK_RETURN:
							if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))
							{
//								screen->setFullscreen(!efConfiguration.isFullScreen());
//								efConfiguration.setFullScreen(!efConfiguration.isFullScreen());
								m.noticeFullscreen();
//								toLog(UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
							}
						break;
						case SDLK_ESCAPE:
							if(UI_Object::focus!=NULL)
								UI_Object::focus=NULL;
							else 
								done = true;
							break;
						case SDLK_PAUSE:
						{
//							if(m.isOptimizing())
//								m.stopAllOptimizing();
//							else m.startAllOptimizing();
						}break;
						case SDLK_SPACE:
								screenCapturing=100;
//								if(m.isOptimizing())
//									m.stopAllOptimizing();
//								else m.startAllOptimizing();
							break;
//						case SDLK_PRINT:break;
						case SDLK_EQUALS:
								if(UI_Object::theme.getResolution()<RESOLUTION_1280x1024)
								{
									if(ignore_rest) 
										break;
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() + 1));
									screen->setResolution(UI_Object::theme.getResolutionSize());
									UI_Object::setResolution(UI_Object::theme.getResolutionSize());
									uiConfiguration.setResolution(UI_Object::theme.getResolution());
									std::ostringstream os;os.str("");
 									os << UI_Object::max_x << "x" << UI_Object::max_y;
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
									m.reloadOriginalSize();
									ignore_rest=true;
								}
								break;
						case SDLK_MINUS:
								if(UI_Object::theme.getResolution()>RESOLUTION_640x480)
								{
									if(ignore_rest) 
										break;
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() - 1));
									screen->setResolution(UI_Object::theme.getResolutionSize());
									UI_Object::setResolution(UI_Object::theme.getResolutionSize());
									uiConfiguration.setResolution(UI_Object::theme.getResolution());
									std::ostringstream os;os.str("");
 									os << UI_Object::max_x << "x" << UI_Object::max_y;
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
									m.reloadOriginalSize();
									ignore_rest=true;
								}
								break;
						case SDLK_LEFTBRACKET:
								if(screen->getBitDepth() > DEPTH_8BIT)
								{
									if(ignore_rest) break;
									UI_Object::theme.setBitDepth((eBitDepth)(screen->getBitDepth()-1));
									screen->setBitDepth(UI_Object::theme.getBitDepth());
									uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
									UI_Object::theme.updateColors(screen->GetSurface());
									// TODO bitDepth im theme aendern!
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->GetSurface()->format->BitsPerPixel));
									ignore_rest=true;
								}
								break;
						case SDLK_RIGHTBRACKET:
								if(screen->getBitDepth() < DEPTH_32BIT)
								{
									if(ignore_rest) break;
									UI_Object::theme.setBitDepth((eBitDepth)(screen->getBitDepth()+1));
									screen->setBitDepth(UI_Object::theme.getBitDepth());
									uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
									UI_Object::theme.updateColors(screen->GetSurface());
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->GetSurface()->format->BitsPerPixel));
									ignore_rest=true;
									
								}
								break;
							
//						case SDLK_F6:m.mainWindow->forcePressTab(MAP_TAB);break;
//						case SDLK_F7:m.mainWindow->forcePressTab(SETTINGS_TAB);break;
//						case SDLK_F8:m.mainWindow->forcePressTab(HELP_TAB);break;
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
			UI_Object::theme.updateColors(screen->GetSurface());
			toLog(UI_Object::theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)screen->GetSurface()->format->BitsPerPixel));
			m.needRedraw();
			UI_Object::resetWindow();
			
		}
		if(m.hasResolutionChanged())
		{
			screen->setResolution(UI_Object::theme.getResolutionSize());
			UI_Object::setResolution(UI_Object::theme.getResolutionSize());
			std::ostringstream os;os.str("");
 			os << UI_Object::max_x << "x" << UI_Object::max_y;
			toLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
			m.reloadOriginalSize();
			UI_Object::resetWindow();
		}
		if(m.hasFullScreenChanged())
		{
			screen->setFullscreen(efConfiguration.isFullScreen());
			toLog(UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
		}
		if(m.hasThemeChanged())
		{
			m.needRedraw();
		}

		m.resetDataChange();

		fps->poll(PROCESS_TICKS);

		float vol;
		channel.front()->getVolume(&vol);
		if(!uiConfiguration.isMusic())
		{
			if(vol < 0.1)
				channel.front()->setVolume(0.0);
			else
				channel.front()->setVolume(vol - 0.1);
		} else
		{
			if(uiConfiguration.getMusicVolume() > vol*100)
				channel.front()->setVolume(vol+0.01);
			else
			if(uiConfiguration.getMusicVolume() < vol*100)
				channel.front()->setVolume(vol-0.01);
		}

		for(std::list<FMOD::Channel*>::iterator i = channel.begin(); i!=channel.end(); i++)
		{
			bool isplaying = false;
			(*i)->isPlaying(&isplaying);
			if(!isplaying)
				i = channel.erase(i);
		}

		for(std::list<std::pair<FMOD::Sound*,float> >::iterator i = UI_Object::theme.soundsToPlay.begin(); i != UI_Object::theme.soundsToPlay.end(); )
		{
			if((uiConfiguration.isSound()) && (channel.size() < uiConfiguration.getChannels()))
			{
				FMOD::Channel* mychannel = NULL;
				UI_Object::theme.sound->playSound(FMOD_CHANNEL_FREE, i->first, 0, &mychannel);
				mychannel->setPan(i->second);
				mychannel->setVolume((float)(uiConfiguration.getSoundVolume())/100.0);
				channel.push_back(mychannel);
			}
			i = UI_Object::theme.soundsToPlay.erase(i);
		}

		UI_Object::theme.sound->update();

		fps->poll(SOUND_TICKS);



// ------ DRAWING AND PROCESSING ------
	
		if(picture_num==INTRO_ANIMATION_FRAMES)
		{
			m.draw(screen);
			if(uiConfiguration.isUnloadGraphics())
				UI_Object::theme.unloadGraphicsAndSounds();
		}

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
				screen->SetTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
				screen->SetFont(UI_Object::theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
				screen->DrawText("Brought to you by...", UI_Object::max_x/2 - picture->w/3, p.y - 15);
				DC::addRectangle(Rect(UI_Object::max_x/2 - picture->w/3, p.y - 20, picture->w, 20));
			}
			
//			screen->SetBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
//			screen->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
//			screen->DrawEdgedRoundedRectangle(Rect(p - Size(5,5), s + Size(10,10)), 4);
//			screen->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
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
			
			screen->SetBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
			screen->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(5,5), s + Size(10,10)), 4);
			screen->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(3,3), s + Size(6,6)), 4);
			screen->DrawBitmap(picture, p);
			DC::addRectangle(Rect(p - Size(5,5), s + Size(10,10)));
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

			screen->SetBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
			screen->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(5,5), s + Size(10,10)), 4);
			screen->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(3,3), s + Size(6,6)), 4);
			screen->DrawBitmap(picture, p);
			DC::addRectangle(Rect(p - Size(5,5), s + Size(10,10)));
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
			screen->SetBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
			screen->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(5,5), s + Size(10,10)), 4);
			screen->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
			screen->DrawEdgedRoundedRectangle(Rect(p - Size(3,3), s + Size(6,6)), 4);
			screen->DrawBitmap(picture, p);
			DC::addRectangle(Rect(p - Size(5,5), s + Size(10,10)));
			picture_num++;

			SDL_FreeSurface(picture);				
		}
		else if(picture_num<(INTRO_ANIMATION_FRAMES-1))
		{
			picture_num++;
//			if(picture_num == (INTRO_ANIMATION_FRAMES-1))
//				picture_num = 9999; // ~
		}
			
		if(picture_num == 9999)
		{
			picture_num = INTRO_ANIMATION_FRAMES;
			UI_Object::theme.playSound(LALA_SOUND, UI_Object::max_x/2);
			efConfiguration.setDesiredCPU(original_desired_cpu);
			efConfiguration.setDesiredFramerate(original_desired_framerate);
			std::ostringstream os;
			os << efConfiguration.getDesiredFramerate();
			toLog("* Setting desired framerate to " + os.str());
			os.str("");	
			os << efConfiguration.getDesiredCPU();
			toLog("* Setting desired CPU usage to " + os.str() + "%");
			SDL_ShowCursor(SDL_ENABLE);
		}

/// ------ END DRAWING ------

//
//
// 
// ------ FPS DEBUG 

		if(efConfiguration.isShowDebug())
			fps->draw(screen);
// ------ END FPS DEBUG
// ------ SCREENCAPTURE ------ 
/*                if(screenCapturing==100) {
			std::ostringstream os;os.str("");os << "shot" << screenshot << ".bmp";
                        SDL_SaveBMP(screen->GetSurface() , os.str().c_str());
			++screenshot;
		}
		if(screenCapturing>0) {
			--screenCapturing;
			std::ostringstream os;os.str("");os << "shot" << (screenshot-1) << ".bmp" << " saved (" << (UI_Object::max_x * UI_Object::max_y * (int)(screen->GetSurface()->format->BitsPerPixel))/1024 << "kb)";
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
	toLog("* Closing SDL...");
	delete screen;
	exitInfo.smoothExit = true;
	return(EXIT_SUCCESS);
}				

