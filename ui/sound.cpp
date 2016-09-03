#ifndef _NO_FMOD_SOUND

#include "sound.hpp"
#include "object.hpp"

#include <fmod_errors.h>
#include <fstream>
#include <sstream>
//#include <iomanip>

UI_Sound::UI_Sound() :
	sound(NULL),
	configurationFile("sounds.cfg")
{
	for(unsigned int i = MAX_SOUNDS;i--;)
		soundList[i] = NULL;
	initSoundIdentifier();
}

UI_Sound::~UI_Sound()
{
	toInitLog("* " + UI_Object::theme.lookUpString(END_FREEING_SOUNDS_STRING));
	for(unsigned int i=MAX_SOUNDS;i--;)
		soundList[i]->release();

	releaseSoundEngine();
}

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

const bool UI_Sound::loadSoundDataFile(const std::string& sound_dir)
{
	std::ifstream pFile((sound_dir + configurationFile).c_str());

	toInitLog("* " + UI_Object::theme.lookUpString(START_LOADING_STRING) + " " + sound_dir + configurationFile);
	
	if(!checkStreamIsOpen(pFile, "UI_Sound::loadSoundDataFile", sound_dir + configurationFile))
		return(false);

	char line[1024];
	bool loading_sounds = false;
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
				toErrorLog("WARNING (UI_Sound::loadSoundDataFile()): No concluding @END for @SOUND_DATA_TYPE block was found in file " + sound_dir + configurationFile + " => trying to parse what we have so far.");
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
						i->second.pop_front(); // Parameter
						entry.loop = ((i->second.size()>0)&&(i->second.front() == "(LOOP)"));
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

FMOD::Sound* UI_Sound::lookUpSound(const eSound id)
{
	if(!sound)
		return(NULL);
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_SOUNDS)) {
		toErrorLog("ERROR: (UI_Sound::lookUpSound) id out of range.");return(NULL); // TODO
	}
#endif
	if(soundList[id] == NULL)
// reload
	{
		FMOD::Sound* temp = NULL;
		if(soundAccessTable[id]==NULL)
		{
			toErrorLog("ERROR (UI_Sound::lookUpSound()): Sound was not initialized. Check 'settings/ui/default.ui' and 'data/sounds'."); // TODO sound identifier...
			return(NULL);
		} else		
		{
			FMOD_RESULT result;
			result = sound->createSound(soundAccessTable[id]->name.c_str(), (soundAccessTable[id]->loop?FMOD_LOOP_NORMAL:0) | FMOD_SOFTWARE, 0, &temp);
			if(!ERRCHECK(result))
			{
				toErrorLog("ERROR (UI_Sound::lookUpSound()): Could not load " + soundAccessTable[id]->name);
				return(NULL);
			}
			soundAccessTable[id]->sound = temp;
			soundList[id] = temp;
		}
	}
	soundAccessTable[id]->used = true;
	return(soundList[id]);
}

void UI_Sound::playSound(const eSound id, const unsigned int x)
{
	if(!sound)
		return;
	soundsToPlay.push_back(std::pair<FMOD::Sound*, float>(lookUpSound(id), 2*((float)(2*x) - (float)UI_Object::max_x)/(float)(3*UI_Object::max_x)));
}

void UI_Sound::printSoundInformation() const
{
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
		toInitLog(os.str());
	}
	else
	{
		char driver_name[128];
		sound->getDriverName(current_driver, driver_name, 128);
		os << driver_name;
		toInitLog(os.str());
	}
}

void UI_Sound::releaseSoundEngine()
{
	if(!sound)
		return;
	toInitLog("* " + UI_Object::theme.lookUpString(END_CLOSING_SOUND_ENGINE_STRING));
	sound->close();

	toInitLog("* " + UI_Object::theme.lookUpString(END_RELEASING_SOUND_ENGINE_STRING));
	sound->release();
	sound = NULL;
}


void UI_Sound::initSoundIdentifier()
{
	for(unsigned int i = MAX_SOUNDS; i--;)
		soundIdentifier[i] = "null";
	soundIdentifier[NULL_SOUND] = "NULL_SOUND";
	soundIdentifier[LALA_SOUND] = "LALA_SOUND";
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
}
#endif
