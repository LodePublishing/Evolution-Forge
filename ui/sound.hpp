#ifndef _UI_SOUND_HPP
#define _UI_SOUND_HPP

enum eSound
{
	NULL_SOUND,
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

enum eMusic
{
	NULL_MUSIC,
	LALA_MUSIC,
	DIABLO_MUSIC,
	MAX_MUSIC
};

#ifdef _FMOD_SOUND
#include <fmod.hpp>
#elif _SDL_MIXER_SOUND
#include <SDL_mixer.h>
#endif

#include <string>
#include <list>

struct SoundEntry
{
	std::string name;

#ifdef _FMOD_SOUND
	FMOD::Sound* sound;
#elif _SDL_MIXER_SOUND
	Mix_Chunk* sound;
#endif
	bool used;
};

struct MusicEntry
{
	std::string name;
#ifdef _FMOD_SOUND
	FMOD::Sound* music;
#elif _SDL_MIXER_SOUND
	Mix_Music* music;
#endif
	bool used;
};

class UI_Sound
{
	public:
		UI_Sound();
		~UI_Sound();
		
		const bool init();
		
		const bool loadSoundDataFile(const std::string& sound_dir);
#ifdef _FMOD_SOUND
		FMOD::Sound* lookUpSound(const eSound id);
		FMOD::Sound* lookUpMusic(const eMusic id);
#elif _SDL_MIXER_SOUND
		Mix_Chunk* lookUpSound(const eSound id);
		Mix_Music* lookUpMusic(const eMusic id);
#endif
		void printSoundInformation() const;
		void playSound(const eSound id, const unsigned int x);
		void playMusic(const eMusic id);
		
		void releaseSoundEngine();

		void clearChannels();
		void clearSoundsToPlay();
		void update();
		void processChannels();
		void stopMusic();
	private:
#ifdef _FMOD_SOUND
		FMOD::Channel* musicChannel;
		std::list<std::pair<FMOD::Sound*, float> > soundsToPlay;
		FMOD::System* sound;
		FMOD::Sound* soundList[MAX_SOUNDS];
		FMOD::Sound* musicList[MAX_MUSIC];
		std::list<FMOD::Channel*> soundChannel;
#elif _SDL_MIXER_SOUND
		static Mix_Music* music;
		static Mix_Music* nextMusic;
		std::list<std::pair<Mix_Chunk*, float> > soundsToPlay;
		Mix_Chunk* soundList[MAX_SOUNDS];
		Mix_Music* musicList[MAX_MUSIC];
		static std::list<int> soundChannel;
		static void transitionMusic();
		static void channelDone(int channel);
#endif
		static eMusic currentMusic;
		
		void initSoundAndMusicIdentifier();
		
		std::string soundIdentifier[MAX_SOUNDS];
		std::string musicIdentifier[MAX_MUSIC];
		std::list<SoundEntry> loadedSounds;
		std::list<MusicEntry> loadedMusic;
		SoundEntry* soundAccessTable[MAX_SOUNDS];
		MusicEntry* musicAccessTable[MAX_MUSIC];
		
		std::string configurationFile;
};

#endif // _UI_SOUND_HPP

