#ifndef _UI_SOUND_HPP
#define _UI_SOUND_HPP

#ifndef _NO_FMOD_SOUND
enum eSound
{
	NULL_SOUND,
	LALA_SOUND,
	MOUSEOVER_SOUND,
	SWISHIN_SOUND,
	SWISHOUT_SOUND,
	SWISHLOCK_SOUND,
	CLICKED_SOUND,
	CLICK_SOUND,
	COMPLETE_SOUND,
	ERROR_SOUND,
	RING_SOUND,
	INTRO_SOUND,
	MAX_SOUNDS
};

#include <fmod.hpp>
#include <string>
#include <list>
struct SoundEntry
{
	std::string name;
	FMOD::Sound* sound;
	bool used;
	bool loop;
};


class UI_Sound
{
	public:
		UI_Sound();
		~UI_Sound();
		
		const bool loadSoundDataFile(const std::string& sound_dir);
		FMOD::Sound* lookUpSound(const eSound id);
		void printSoundInformation() const;
		void playSound(const eSound id, const unsigned int x);
		std::list<std::pair<FMOD::Sound*, float> > soundsToPlay;
		static bool ERRCHECK(FMOD_RESULT result);
		FMOD::System* sound;
		void releaseSoundEngine();
	private:
		void initSoundIdentifier();
		std::string soundIdentifier[MAX_SOUNDS];
		FMOD::Sound* soundList[MAX_SOUNDS];
		std::list<SoundEntry> loadedSounds;
		SoundEntry* soundAccessTable[MAX_SOUNDS];
		std::string configurationFile;
};
#endif // nosound
#endif // _UI_SOUND_HPP

