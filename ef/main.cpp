#include "guimain.hpp"
#include "../sdl/framerate.hpp"
#include "savebox.hpp"
#include "configuration.hpp"
#include "../ui/configuration.hpp"
#include "../core/configuration.hpp"

enum eTicks
{
	PROCESS_TICKS,
	DRAW_TICKS,
	GENERATION_TICKS,
	MESSAGE_TICKS,
	IDLE_TICKS,
	
	MAX_TICK_TYPES
};

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
	stringFiles = database.findFiles("settings", "help", "");
	for(std::list<std::string>::iterator j = stringFiles.begin(); j!=stringFiles.end(); ++j)
		UI_Object::theme.loadHelpChapterStringFile(*j);
	
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
	{
		std::ostringstream os;
		os.str("");
		std::list<std::string> s = DC::getAvailibleDrivers();
		for(std::list<std::string>::const_iterator i = s.begin(); i!=s.end(); i++)
			os << *i << " ";
		toLog("Availible drivers: " + os.str());
	}

	
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
	}

		
	if(!screen->initializationOK())	{
		toLog(UI_Object::theme.lookUpString(START_UNABLE_TO_INIT_SDL_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");return(EXIT_FAILURE);		
	}
	
	if ( !screen->valid() ) {
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
	toLog(UI_Object::theme.lookUpString(START_INIT_FRAMERATE_STRING)); 
	FPS* fps = new FPS();
	fps->setDesiredFramerate(efConfiguration.getDesiredFramerate());
	fps->setDesiredCPU(efConfiguration.getDesiredCPU());
	unsigned int refresh = fps->getFramesPerGeneration();
	
	const unsigned int TICK_INTERVALL = 2;
	long unsigned int current_ticks = 500;
	long unsigned int ticks[MAX_TICK_TYPES][TICK_INTERVALL];
	for(unsigned int i = MAX_TICK_TYPES;i--;)
		for(unsigned int j = TICK_INTERVALL;j--;)
			ticks[i][j] = 10;

	unsigned int frames_count[TICK_INTERVALL];

	for(unsigned int j = TICK_INTERVALL;j--;)
		frames_count[j]=5;
	unsigned int frames_per_second = 10;


	signed int percent[MAX_TICK_TYPES];

	for(unsigned int i = MAX_TICK_TYPES; i--;)
		percent[i] = 10;
	
	long unsigned int total_ticks=500;

	unsigned int tick_intervall = 0;

	UI_Object::focus=NULL;
	unsigned int frames_per_generation = 0;
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
// ------ END CAP FRAMERATE


// ------- INIT GRAPHIC ENGINE ------
	toLog(UI_Object::theme.lookUpString(START_INIT_GRAPHIC_ENGINE_CORE_STRING));
	Main m(screen); 
	toLog(UI_Object::theme.lookUpString(START_MAIN_INIT_COMPLETE_STRING));

	unsigned int screenshot = 100;
//	if(efConfiguration.isAutoSaveRuns())
//		m.startAllOptimizing();
//	else
//		m.stopAllOptimizing(); TODO

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

	toLog(UI_Object::theme.lookUpString(START_SYSTEM_READY_STRING));
// MAIN LOOP
	while(true)
	{
//TODO
		UI_Object::redrawnObjects = 0;
// ------ DRAWING ------
//		for(std::list<UI_Object*>::iterator i = UI_Object::processArray.begin(); i!=UI_Object::processArray.end(); ++i)
//			(*i)->process();
//		{
//			std::ostringstream os;os.str("");os << "length :" << UI_Object::processArray.size();toLog(os.str());
//		}
	
//		UI_Object::copyToNextProcessArray();
		
		
		
		current_ticks = SDL_GetTicks();
		bool ignore_rest = false;
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
					if((UI_Object::focus==NULL)||(!UI_Object::focus->addKey(event.key.keysym.sym, event.key.keysym.mod)))
					switch(event.key.keysym.sym)
					{
						case SDLK_TAB://UI_Object::rotateEditField();
							break;
						case SDLK_KP_ENTER:
						case SDLK_RETURN:
							if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))
							{
//								screen->setFullscreen(!efConfiguration.isFullScreen());
								efConfiguration.setFullScreen(!efConfiguration.isFullScreen());
								m.noticeFullscreen();
//								toLog(UI_Object::theme.lookUpString(efConfiguration.isFullScreen()?START_SET_FULLSCREEN_MODE_STRING:START_SET_WINDOW_MODE_STRING));
							}
						break;
						case SDLK_ESCAPE:
							if(UI_Object::focus!=NULL)
								UI_Object::focus=NULL;
							else {
								delete fps;delete screen;screen=NULL;return(EXIT_SUCCESS);
							}
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
									if(ignore_rest) break;
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() + 1));
									screen->setResolution(UI_Object::theme.getResolution());
									uiConfiguration.setResolution(UI_Object::theme.getResolution());
									resolution = screen->getResolutionSize();
									UI_Object::setResolution(resolution);
									std::ostringstream os;os.str("");
 									os << resolution.GetWidth() << "x" << resolution.GetHeight();
									toLog(UI_Object::theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
									m.reloadOriginalSize();
									ignore_rest=true;
								}
								break;
						case SDLK_MINUS:
								if(UI_Object::theme.getResolution()>RESOLUTION_640x480)
								{
									if(ignore_rest) break;
									UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() - 1));
									screen->setResolution(UI_Object::theme.getResolution());
									uiConfiguration.setResolution(UI_Object::theme.getResolution());
									resolution = screen->getResolutionSize();
									UI_Object::setResolution(resolution);
									std::ostringstream os;os.str("");
 									os << resolution.GetWidth() << "x" << resolution.GetHeight();
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
		ticks[MESSAGE_TICKS][tick_intervall] = SDL_GetTicks() - current_ticks;
		current_ticks = SDL_GetTicks();
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
			screen->setResolution(UI_Object::theme.getResolution());
			resolution = screen->getResolutionSize();
			UI_Object::setResolution(resolution);
			std::ostringstream os;
			os.str("");
 			os << resolution.GetWidth() << "x" << resolution.GetHeight();
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

		ticks[PROCESS_TICKS][tick_intervall] = SDL_GetTicks() - current_ticks;
		current_ticks = SDL_GetTicks();	

// ------ DRAWING AND PROCESSING ------

		m.draw(screen);
		if(uiConfiguration.isUnloadGraphics())
			UI_Object::theme.unloadGraphics();
// ------ END DRAWING ------

//
//
// 
// ------ FPS DEBUG 

		if(efConfiguration.isShowDebug())
		{			
			screen->SetPen(*UI_Object::theme.lookUpPen(NULL_PEN));
			
			Point p = Point(20, resolution.GetHeight() - 110);
			screen->SetTextForeground(DC::toSDL_Color(255, 20, 20));
			screen->SetFont(UI_Object::theme.lookUpFont(MIDDLE_BOLD_FONT));
			screen->SetBrush(Brush(Color(screen->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
			screen->DrawRectangle(Rect(p, Size(200, 80)));
			DC::addRectangle(Rect(p, Size(200, 80)));

			std::ostringstream os;
//			os.str("");
//			os << "Objects: (" << UI_Object::redrawnObjects << "/" << DC::changedRectangles << ")   FPS: " << efConfiguration.getCurrentFramerate();
//			screen->DrawText(os.str(), p + Size(20, 5));
			
			
			os.str("");
		
screen->SetBrush(*UI_Object::theme.lookUpBrush(BRIGHT_UNIT_TYPE_1_BRUSH));screen->DrawRectangle(Rect(p + Size(10, 24), Size(percent[PROCESS_TICKS], 10)));
os << "process() " << percent[PROCESS_TICKS] << "%";screen->DrawText(os.str(), p + Size(20, 25));os.str("");

screen->SetBrush(*UI_Object::theme.lookUpBrush(BRIGHT_UNIT_TYPE_2_BRUSH));screen->DrawRectangle(Rect(p + Size(10, 34), Size(percent[GENERATION_TICKS], 10)));
os << "newGeneration() " << percent[GENERATION_TICKS] << "%";screen->DrawText(os.str(), p + Size(20, 35));os.str("");

screen->SetBrush(*UI_Object::theme.lookUpBrush(BRIGHT_UNIT_TYPE_3_BRUSH));screen->DrawRectangle(Rect(p + Size(10, 44), Size(percent[DRAW_TICKS], 10)));
os << "draw() " << percent[DRAW_TICKS] << "%";screen->DrawText(os.str(), p + Size(20, 45));os.str("");	

screen->SetBrush(*UI_Object::theme.lookUpBrush(BRIGHT_UNIT_TYPE_4_BRUSH));screen->DrawRectangle(Rect(p + Size(10, 54), Size(percent[MESSAGE_TICKS], 10)));
os << "message() " << percent[MESSAGE_TICKS] << "%";screen->DrawText(os.str(), p + Size(20, 55));os.str("");

screen->SetBrush(*UI_Object::theme.lookUpBrush(BRIGHT_UNIT_TYPE_5_BRUSH));screen->DrawRectangle(Rect(p + Size(10, 64), Size(percent[IDLE_TICKS], 10)));
os << "idle() " << percent[IDLE_TICKS] << "%";screen->DrawText(os.str(), p + Size(20, 65));os.str("");	
		
		}
// ------ END FPS DEBUG
// ------ SCREENCAPTURE ------ 
/*                if(screenCapturing==100) {
			std::ostringstream os;os.str("");os << "shot" << screenshot << ".bmp";
                        SDL_SaveBMP(screen->GetSurface() , os.str().c_str());
			++screenshot;
		}
		if(screenCapturing>0) {
			--screenCapturing;
			std::ostringstream os;os.str("");os << "shot" << (screenshot-1) << ".bmp" << " saved (" << (resolution.GetWidth() * resolution.GetHeight() * (int)(screen->GetSurface()->format->BitsPerPixel))/1024 << "kb)";
                        screen->DrawText(os.str(), 50, 300);
                }*/
// ------ END SCREENCAPTURE -----
		screen->updateScreen();
		ticks[DRAW_TICKS][tick_intervall] = SDL_GetTicks() - current_ticks;
		
		current_ticks = SDL_GetTicks();
		
		
// ------ FRAMERATE AND CALCULATION ------	
		frames_per_generation = fps->getFramesPerGeneration();
		frames_count[tick_intervall] = fps->getCurrentFramerate();
	
		fps->setAdaptFramesPerGeneration(m.isAnyOptimizing());
		fps->setDesiredFramerate(efConfiguration.getDesiredFramerate());
		fps->setDesiredCPU(efConfiguration.getDesiredCPU());
		fps->setTotalTicks(1+total_ticks/TICK_INTERVALL);
		fps->delay();
	
		ticks[IDLE_TICKS][tick_intervall] = SDL_GetTicks() - current_ticks;
		current_ticks = SDL_GetTicks();
			
		{
			refresh += 100;
			while((refresh > frames_per_generation))
			{
				m.newGeneration();
				if(frames_per_generation > refresh)
					refresh = 0;
				else
					refresh -= frames_per_generation;
			}
		}
// ------ END FRAMERATE AND CALCULATION 
		ticks[GENERATION_TICKS][tick_intervall] = SDL_GetTicks() - current_ticks;


		
		tick_intervall++;
		if(tick_intervall >= TICK_INTERVALL)
			tick_intervall = 0;
		
		total_ticks = 0;
		unsigned int total_this_ticks[MAX_TICK_TYPES];
		for(unsigned int i = MAX_TICK_TYPES;i--;)
		{
			total_this_ticks[i]=0;
			for(unsigned int j = TICK_INTERVALL;j--;)
				total_this_ticks[i] += ticks[i][j];
			total_ticks+=total_this_ticks[i];
			
		}
		total_ticks+=1;

		unsigned int total_frames = 0;
		for(unsigned int j = TICK_INTERVALL;j--;)
			total_frames += frames_count[j];
		total_frames /= TICK_INTERVALL;

		if(frames_per_second > total_frames)
			frames_per_second -= (frames_per_second - total_frames)/5 + 1;
		else
		if(frames_per_second < total_frames)
			frames_per_second += (total_frames - frames_per_second)/5 + 1;
		if(frames_per_second < 0) frames_per_second = 0;
		

		for(unsigned int i = MAX_TICK_TYPES;i--;)
		{
			if(percent[i] > total_this_ticks[i] * 100 / total_ticks)
				percent[i] -= (percent[i] - total_this_ticks[i] * 100 / total_ticks)/5 + 1;
			else
			if(percent[i] < total_this_ticks[i] * 100 / total_ticks)
				percent[i] += (total_this_ticks[i] * 100 / total_ticks - percent[i])/5 + 1;
			if(percent[i] < 0) percent[i]=0;
			if(percent[i] > 100) percent[i]=100;
		}
	
		efConfiguration.setCurrentFramerate(frames_per_second);
		efConfiguration.setCurrentFramesPerGeneration(frames_per_generation);
	
		
	}
	return(EXIT_SUCCESS);
}
					

