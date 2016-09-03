#include "sound.hpp"
#include "object.hpp"

#ifdef _FMOD_SOUND
#include <fmod_errors.h>
#endif

#include <fstream>
#include <sstream>

#define MUSIC_TRANSITION_DURATION 2000

UI_Sound::UI_Sound() :
#ifdef _FMOD_SOUND
	sound(NULL),
	musicChannel(NULL),
#endif
	configurationFile("sounds.cfg")
{
	for(unsigned int i = MAX_SOUNDS;i--;)
		soundList[i] = NULL;
	for(unsigned int i = MAX_MUSIC;i--;)
		soundList[i] = NULL;

	initSoundAndMusicIdentifier();
}

UI_Sound::~UI_Sound()
{
	toInitLog("* " + UI_Object::theme.lookUpString(END_FREEING_SOUNDS_STRING));
	for(unsigned int i = MAX_SOUNDS; i--;)
	{
#ifdef _FMOD_SOUND
		soundList[i]->release();
#elif _SDL_MIXER_SOUND
		Mix_FreeChunk(soundList[i]);
#endif
		soundList[i] = NULL;
	}
#ifdef _FMOD_SOUND
	if(musicChannel)
		musicChannel->stop();
#elif _SDL_MIXER_SOUND
	Mix_HaltMusic();
#endif
	for(unsigned int i = MAX_MUSIC; i--;)
	{
#ifdef _FMOD_SOUND
		musicList[i]->release();
#elif _SDL_MIXER_SOUND
		Mix_FreeMusic(musicList[i]);
#endif
		musicList[i] = NULL;
	}
	releaseSoundEngine();
}

#ifdef _FMOD_SOUND
bool UI_Sound::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::ostringstream os;
		os << "FMOD error! ( "<< result << ") " << FMOD_ErrorString(result);
		toErrorLog(os.str());
		return(false);
	}
	return(true);
}
#endif

#if 0
void UI_Sound::unloadGraphics()
{
/*	for(std::list<SoundEntry>::iterator i = loadedSounds.begin(); i!=loadedSounds.end(); ++i)
		if(!i->used)
		{
			for(unsigned int j = MAX_SOUNDS;j--;)
				if(i->sound == soundList[j])
					soundList[j] = NULL;
			i->sound->release();
			i->sound = NULL;
			i->used = true;
		} else if(i->sound!=NULL)
			i->used = false;*/
}
#endif


const bool UI_Sound::init()
{
	// TODO start a 'watchdog' thread (FMOD waits indefinitely if the sound is currently used!)
	
#ifdef _FMOD_SOUND
	unsigned int version;
	
        if(!UI_Sound::ERRCHECK(FMOD::System_Create(&UI_Object::sound.sound))) 
		return(false);
	if(!UI_Sound::ERRCHECK(UI_Object::sound.sound->getVersion(&version))) 
		return(false);

	if (version < FMOD_VERSION)
        {
		std::ostringstream os;
		os << UI_Object::theme.lookUpString(START_INIT_FMOD_VERSION_ERROR_STRING) << "[" << version << " < " << FMOD_VERSION << "]";
		toErrorLog(os.str());
	       	return(false);
	}
	UI_Object::sound.printSoundInformation();
	if(!UI_Sound::ERRCHECK(UI_Object::sound.sound->init(32, FMOD_INIT_NONREALTIME, 0)))
		return(false);
#elif _SDL_MIXER_SOUND
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048)==-1) 
	{
		std::ostringstream os;
		os << "ERROR (UI_Sound::init()): " << Mix_GetError();
		toErrorLog(os.str());
		return(false);
	}		
	Mix_ChannelFinished(UI_Sound::channelDone);
#endif
	return(true);
}

const bool UI_Sound::loadSoundDataFile(const std::string& sound_dir)
{
	std::ifstream pFile((sound_dir + configurationFile).c_str());

	toInitLog("* " + UI_Object::theme.lookUpString(START_LOADING_STRING) + " " + sound_dir + configurationFile);
	
	if(!checkStreamIsOpen(pFile, "UI_Sound::loadSoundDataFile", sound_dir + configurationFile))
		return(false);

	char line[1024];
	bool loading_sounds = false;
	bool loading_music = false;
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "UI_Sound::loadSoundDataFile", sound_dir + configurationFile))
			return(false);
		
		std::string text = line;
		size_t start = text.find_first_not_of("\t ");
		if((start == std::string::npos) || (text[0] == '#') || (text[0] == '\0'))
			continue; // ignore line
		size_t stop = text.find_first_of("\t ", start);
		if(stop == std::string::npos) 
			stop = text.size();
		std::string index = text.substr(start, stop);
		if(index == "@MUSIC")
		{
			if(!loading_music)
			{
				loading_music = true;
				toInitLog("  - " + UI_Object::theme.lookUpString(START_LOAD_MUSIC_STRING));
			}
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING (UI_Sound::loadSoundDataFile()): No concluding @END for @MUSIC block was found in file " + sound_dir + configurationFile + " => trying to parse what we have so far.");
			}
			for(unsigned int j = 0; j < MAX_MUSIC; j++)
			{
				std::map<std::string, std::list<std::string> >::iterator i;
				if((i = block.find(musicIdentifier[j])) != block.end())
				{
					i->second.pop_front(); // Identifier loeschen
					std::string name = i->second.front();
					if((name.size()<4)||(name[name.size()-4]!='.'))
						name = sound_dir + name + ".mp3";
					else name = sound_dir + name;
					bool found_music = false;
					for(std::list<MusicEntry>::iterator l = loadedMusic.begin(); l!=loadedMusic.end(); ++l)
					// already loaded?
						if(l->name == name)
						{
							found_music = true;
							musicAccessTable[j] = &(*l);
							break;
						}
					if(!found_music)
					{
						MusicEntry entry;
						entry.name = name;
						toInitLog(name);
						entry.music = NULL;
						entry.used = false;
						loadedMusic.push_back(entry);
						musicAccessTable[j] = &(loadedMusic.back());
					}
					block.erase(i);
				}
			}
		// TODO nicht gefundene Eintraege bemaengeln
		} else
		if(index == "@SOUNDS")
		{
			if(!loading_sounds)
			{
				loading_sounds = true;
				toInitLog("  - " + UI_Object::theme.lookUpString(START_LOAD_SOUNDS_STRING));
			}
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING (UI_Sound::loadSoundDataFile()): No concluding @END for @SOUNDS block was found in file " + sound_dir + configurationFile + " => trying to parse what we have so far.");
			}
			for(unsigned int j = 0; j < MAX_SOUNDS; j++)
			{
				std::map<std::string, std::list<std::string> >::iterator i;
				if((i = block.find(soundIdentifier[j])) != block.end())
				{
					i->second.pop_front(); // Identifier loeschen
					std::string name = i->second.front();
					if((name.size()<4)||(name[name.size()-4]!='.'))
						name = sound_dir + name + ".mp3";
					else name = sound_dir + name;
					bool found_sound = false;
					for(std::list<SoundEntry>::iterator l = loadedSounds.begin(); l!=loadedSounds.end(); ++l)
					// already loaded?
						if(l->name == name)
						{
							found_sound = true;
							soundAccessTable[j] = &(*l);
							break;
						}
					if(!found_sound)
					{
						SoundEntry entry;
						entry.name = name;
						entry.sound = NULL;
						entry.used = false;
						loadedSounds.push_back(entry);
						soundAccessTable[j] = &(loadedSounds.back());
					}
					block.erase(i);
				}
			}
		// TODO nicht gefundene Eintraege bemaengeln
		}
	}
	return(true);
}

#ifdef _FMOD_SOUND
FMOD::Sound* UI_Sound::lookUpMusic(const eMusic id)
#elif _SDL_MIXER_SOUND
Mix_Music* UI_Sound::lookUpMusic(const eMusic id)
#endif
{
#ifdef _FMOD_SOUND
	if(!sound)
		return(NULL);
#endif

#ifdef _SCC_DEBUG
	if((id < 0) || (id >= MAX_MUSIC)) {
		toErrorLog("ERROR: (UI_Sound::lookUpMusic) id out of range.");return(NULL); // TODO
	}
#endif
	if(musicList[id] == NULL)
// reload
	{
#ifdef _FMOD_SOUND
		FMOD::Sound* temp = NULL;
#elif _SDL_MIXER_SOUND
		Mix_Music* temp = NULL;
#endif
		if(musicAccessTable[id]==NULL)
		{
			toErrorLog("ERROR (UI_Sound::lookUpMusic()): Music was not initialized. Check 'settings/ui/default.ui' and 'data/sounds'."); // TODO sound identifier...
			return(NULL);
		} else		
		{
#ifdef _FMOD_SOUND
			FMOD_RESULT result;
			result = sound->createSound(musicAccessTable[id]->name.c_str(), FMOD_LOOP | FMOD_SOFTWARE, 0, &temp);
			if(!ERRCHECK(result))
			{
				toErrorLog("ERROR (UI_Sound::lookUpMusic()): Could not load " + musicAccessTable[id]->name);
				return(NULL);
			}
#elif _SDL_MIXER_SOUND
			temp = Mix_LoadMUS(musicAccessTable[id]->name.c_str());
			if(!temp) 
			{
				std::ostringstream os;
				os << "ERROR (UI_Sound::lookUpMusic()): Could not load " << musicAccessTable[id]->name.c_str() << " : " << Mix_GetError();
				toErrorLog(os.str());
				return(NULL);
			}
#endif
			musicAccessTable[id]->music = temp;
			musicList[id] = temp;
		}
	}
	musicAccessTable[id]->used = true;
	return(musicList[id]);
}

#ifdef _FMOD_SOUND
FMOD::Sound* UI_Sound::lookUpSound(const eSound id)
#elif _SDL_MIXER_SOUND
Mix_Chunk* UI_Sound::lookUpSound(const eSound id)
#endif
{
#ifdef _FMOD_SOUND
	if(!sound)
		return(NULL);
#endif

#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_SOUNDS)) {
		toErrorLog("ERROR: (UI_Sound::lookUpSound) id out of range.");return(NULL); // TODO
	}
#endif
	if(soundList[id] == NULL)
// reload
	{
#ifdef _FMOD_SOUND
		FMOD::Sound* temp = NULL;
#elif _SDL_MIXER_SOUND
		Mix_Chunk* temp = NULL;
#endif
		if(soundAccessTable[id]==NULL)
		{
			toErrorLog("ERROR (UI_Sound::lookUpSound()): Sound " + soundIdentifier[id] + " was not initialized. Check 'data/sounds/sounds.cfg' and 'data/sounds'.");
			toErrorLog(id);
			return(NULL);
		} else		
		{
#ifdef _FMOD_SOUND
			FMOD_RESULT result;
			result = sound->createSound(soundAccessTable[id]->name.c_str(), FMOD_SOFTWARE, 0, &temp);
			if(!ERRCHECK(result))
			{
				toErrorLog("ERROR (UI_Sound::lookUpSound()): Could not load " + soundAccessTable[id]->name);
				return(NULL);
			}
#elif _SDL_MIXER_SOUND
			temp = Mix_LoadWAV(soundAccessTable[id]->name.c_str());
			if(!temp) 
			{
				std::ostringstream os;
				os << "ERROR (UI_Sound::lookUpSound()): Could not load " << soundAccessTable[id]->name.c_str() << " : " << Mix_GetError();
				toErrorLog(os.str());
				return(NULL);
			}
#endif
			soundAccessTable[id]->sound = temp;
			soundList[id] = temp;
		}
	}
	soundAccessTable[id]->used = true;
	return(soundList[id]);
}

void UI_Sound::playSound(const eSound id, const unsigned int x)
{
#ifdef _FMOD_SOUND
	if(!sound)
		return;
	soundsToPlay.push_back(std::pair<FMOD::Sound*, float>(lookUpSound(id), 2*((float)(2*x) - (float)UI_Object::max_x)/(float)(3*UI_Object::max_x)));
#elif _SDL_MIXER_SOUND
//	toErrorLog("play sound: ");
//	toErrorLog(id);
	soundsToPlay.push_back(std::pair<Mix_Chunk*, float>(lookUpSound(id), 2*((float)(2*x) - (float)UI_Object::max_x)/(float)(3*UI_Object::max_x)));
#endif
}



#ifdef _SDL_MIXER_SOUND
static void transitionMusic(void)
{
	UI_Sound::transitionMusic();
}

void UI_Sound::transitionMusic(void)
{
	toInitLog("callback called");
	if(music != NULL)
	{
		Mix_HaltMusic(); // Just in case there was a misunderstanding, halt the music
		Mix_FreeMusic(music);
		music = nextMusic;
		nextMusic = NULL;
		if(music != NULL)
		{
			int success = Mix_FadeInMusic(music, -1, MUSIC_TRANSITION_DURATION/2);
#ifdef _SCC_DEBUG
			if(success != 0) {
				std::ostringstream os;
				os << "ERROR (UI_Sound::playMusic()): " << Mix_GetError();
				toErrorLog(os.str());
			}
#endif
		}
	}
}
#endif

void UI_Sound::playMusic(const eMusic id, const bool loop)
{
#ifdef _FMOD_SOUND
	if(!sound)
		return;
	currentMusic = id;
	if(musicChannel)
		musicChannel->stop();
	if(id != MAX_MUSIC)
		sound->playSound(FMOD_CHANNEL_FREE, lookUpMusic(currentMusic), 0, &musicChannel);
#elif _SDL_MIXER_SOUND
	int success = -1;
	if(music != NULL)
	{
		if(id != MAX_MUSIC)
		{
			currentMusic = id;
			if(loop)
				nextMusic = lookUpMusic(currentMusic);
			else 
				nextMusic = NULL;
			Mix_HookMusicFinished(::transitionMusic);
//			toErrorLog("fading out");
			success = Mix_FadeOutMusic(MUSIC_TRANSITION_DURATION/2);
		} else 
		{
//			toErrorLog("fading out");
			success = Mix_FadeOutMusic(MUSIC_TRANSITION_DURATION);
		}
	} else {
		if(id != MAX_MUSIC)
		{
			currentMusic = id;
			music = lookUpMusic(id);
			if(loop)
				nextMusic = lookUpMusic(currentMusic);
			else 
				nextMusic = NULL;
			Mix_HookMusicFinished(::transitionMusic);
//			toErrorLog("fading in");
			success = Mix_FadeInMusic(music, -1, MUSIC_TRANSITION_DURATION);
		}
		else success = 0;
	}
#ifdef _SCC_DEBUG
	if(success != 0) {
		std::ostringstream os;
		os << "ERROR (UI_Sound::playMusic()): " << Mix_GetError();
		toErrorLog(os.str());
	}
#endif
#endif
}

void UI_Sound::stopMusic()
{
	playMusic(MAX_MUSIC);
}

void UI_Sound::printSoundInformation() const
{
#ifdef _FMOD_SOUND
	int driver_num;
	sound->getNumDrivers(&driver_num);
	std::ostringstream os; os << "* Availible sound drivers: ";
	for(unsigned int i = driver_num; i--;)
	{
		char driver_name[128];
		sound->getDriverName(i, driver_name, 128);
		os << driver_name << " ";
	}
	toInitLog(os.str());
	os.str("");
	int current_driver;
	sound->getDriver(&current_driver);

	os << "* Driver used: ";
	if(current_driver == -1)
	{
		os << "Primary or main sound device as selected by the operating system settings";
		if(driver_num == 1)
		{
			char driver_name[128];
			sound->getDriverName(current_driver, driver_name, 128);
			os << "(probably '" << driver_name << "')";
		}
	}
	else
	{
		char driver_name[128];
		sound->getDriverName(current_driver, driver_name, 128);
		os << driver_name;
	}
	toInitLog(os.str());
#elif _SDL_MIXER_SOUND
	int audio_rate, audio_channels;
	Uint16 audio_format;
	int success = Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
#ifdef _SCC_DEBUG
	if(!success) {
		std::ostringstream os;
		os << "ERROR (UI_Sound::printSoundInformation()): " << Mix_GetError();
		toErrorLog(os.str());
	}
#endif
	int bits = audio_format & 0xFF;
	std::ostringstream os; 
	os << "* Opened audio at " << audio_rate << "Hz " << bits << "bit " << ((audio_channels>1)?"stereo":"mono");// << ", " << audio_buffers << " bytes audio buffer";
	toInitLog(os.str());
#endif
}

void UI_Sound::update()
{
#ifdef _FMOD_SOUND
	sound.update();	
#elif _SDL_MIXER_SOUND
#endif
}

void UI_Sound::clearChannels()
{
	soundChannel.clear();
}

void UI_Sound::clearSoundsToPlay()
{
	soundsToPlay.clear();
}

#ifdef _SDL_MIXER_SOUND
void UI_Sound::channelDone(int channel)
{
	for(std::list<int>::iterator i = soundChannel.begin(); i!=soundChannel.end();)
	{
		if(*i == channel)
			i = soundChannel.erase(i);
		else ++i;
	}
}
#endif

void UI_Sound::processChannels()
{
#ifdef _FMOD_SOUND
	for(std::list<FMOD::Channel*>::iterator i = soundChannel.begin(); i!=soundChannel.end();)
	{
		bool is_playing = false;
		(*i)->isPlaying(&is_playing);
		if(!is_playing)
			i = soundChannel.erase(i);
		else ++i;
	}
	for(std::list<std::pair<FMOD::Sound*, float> >::iterator i = soundsToPlay.begin(); i != soundsToPlay.end(); ++i)
		if((soundChannel.size() < uiConfiguration.getChannels()))
		{
			FMOD::Channel* mychannel = NULL;
			sound->playSound(FMOD_CHANNEL_FREE, i->first, 0, &mychannel);
			mychannel->setPan(i->second);
			mychannel->setVolume((float)(uiConfiguration.getSoundVolume())/100.0);
			soundChannel.push_back(mychannel);
		}
#elif _SDL_MIXER_SOUND
	for(std::list<std::pair<Mix_Chunk*, float> >::iterator i = soundsToPlay.begin(); i != soundsToPlay.end(); ++i)
		if((soundChannel.size() < uiConfiguration.getChannels()))
		{
			int mychannel = -1;
			mychannel = Mix_PlayChannel(-1, i->first, 0);
			if(mychannel == -1) {
#ifdef _SCC_DEBUG
				std::ostringstream os;
				os << "ERROR (UI_Sound::processChannels()): " << Mix_GetError();
				toErrorLog(os.str());
#endif
				continue;
			} else
			{
//				mychannel->setPan(i->second);
//				mychannel->setVolume((float)(uiConfiguration.getSoundVolume())/100.0);
				soundChannel.push_back(mychannel);
			}
		}
#endif
}





void UI_Sound::releaseSoundEngine()
{
#ifdef _FMOD_SOUND
	if(!sound)
		return;
	toInitLog("* " + UI_Object::theme.lookUpString(END_CLOSING_SOUND_ENGINE_STRING));
	sound->close();
	toInitLog("* " + UI_Object::theme.lookUpString(END_RELEASING_SOUND_ENGINE_STRING));
	sound->release();
	sound = NULL;
#elif _SDL_MIXER_SOUND
	toInitLog("* " + UI_Object::theme.lookUpString(END_CLOSING_SOUND_ENGINE_STRING));
	Mix_CloseAudio();
#endif
}


void UI_Sound::initSoundAndMusicIdentifier()
{
	for(unsigned int i = MAX_SOUNDS; i--;)
		soundIdentifier[i] = "null";
	for(unsigned int i = MAX_MUSIC; i--;)
		musicIdentifier[i] = "null";

	soundIdentifier[NULL_SOUND] = "NULL_SOUND";
	soundIdentifier[MOUSEOVER_SOUND] = "MOUSEOVER_SOUND";
	soundIdentifier[SWISHIN_SOUND] = "SWISHIN_SOUND";
	soundIdentifier[SWISHOUT_SOUND] = "SWISHOUT_SOUND";
	soundIdentifier[SWISHLOCK_SOUND] = "SWISHLOCK_SOUND";
	soundIdentifier[CLICKED_SOUND] = "CLICKED_SOUND";
	soundIdentifier[CLICK_SOUND] = "CLICK_SOUND";
	soundIdentifier[COMPLETE_SOUND] = "COMPLETE_SOUND";
	soundIdentifier[ERROR_SOUND] = "ERROR_SOUND";
	soundIdentifier[RING_SOUND] = "RING_SOUND";
	soundIdentifier[INTRO_SOUND] = "INTRO_SOUND";
	
	musicIdentifier[NULL_MUSIC] = "NULL_MUSIC";
	musicIdentifier[LALA_MUSIC] = "LALA_MUSIC";
	musicIdentifier[DIABLO_MUSIC] = "DIABLO_MUSIC";
}

eMusic UI_Sound::currentMusic = MAX_MUSIC;
#ifdef _SDL_MIXER_SOUND
Mix_Music* UI_Sound::music = NULL;
Mix_Music* UI_Sound::nextMusic = NULL;
std::list<int> UI_Sound::soundChannel;
#endif
