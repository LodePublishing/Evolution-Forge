/*************************************************
 *
 * file : sdl_snd.h
 * class: SDL_snd
 * dependencies: SDL, C++ (STL)
 *
 * Autor: Dominik Haumann
 * eMail: dopsball@gmx.de
 * Date : 2002/07/27 - 2004/05/03
 * Version: 1.0
 *
 *************************************************/

/***************************************************************************
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 ***************************************************************************/

#ifndef SDL_SND_H
#define SDL_SND_H

#ifdef WIN32
#pragma warning(disable: 4786)
#endif

/**
 * use stl
 */
#include <vector>
#include <string>
#include <map>

#include <SDL/SDL.h>


/**
 * This class contains all information about the represented object:
 *   - filename
 *   - length in ms
 *   - length in bytes (size)
 *   - sample data (raw)
 *   - positions array (you may play a sound as often as you wish)
 * @author Dominik Haumann
 * @version 1.0
 */
class SndInfo
{
public: // classes

    /**
     * struct SndPlayInfos
     * position: position in the snd_data
     * loop: if true: sound will be looped
     */
    class SndPlayInfos
    {
        public:
            /**
             * contaings the current position in the sound-buffer
             */
            Uint32   position;

            /**
             * if true the sound will be looped
             */
            bool     loop;
    };


public: // variables
    /**
     * the sound's id
     */
    unsigned int snd_id;

    /**
     * length in seconds
     */
    double length_in_ms;

    /** 
     *size in byte
     */
    Uint32 length_in_bytes;

    /**
     * the file_name in which the sound is saved
     */
    std::string file_name;

    /**
     * raw PCM sample data
     */
    Uint8 *samples;

    /**
     * if true: loop
     */
    bool loop;

    /**
     *store the current positions in the sound-buffer and the loop-option. A vector, because it is possible to play the sound often at the same time
     */
    std::vector <SndPlayInfos> positions;
};


/**
 * This class is THE sound handling object. Initialize it with init(),
 * load sounds with load(), play with play(), stop with stop(), unload with unload()
 * and look for running sounds with is_running().
 * Usually this sound class should be robust which means playing object that do
 * not exist does not harm at all. => You cannot make anything wrong :)
 *
 * Supported data types: wave files (.wav, .pcm)
 * Dependencies: SDL only!
 * @author Dominik Haumann
 * @version 1.0
 */
class SDL_snd
{
public: // user functions

    /**
     * constructor (is empty)
     */
    SDL_snd();

    /**
     * destructor frees the sound data and shot down the audio sub system cleanly
     */
    ~SDL_snd();

    /**
     * some necessary audio initializations
     * @param freq the desired frequency
     * @param stereo stereo or mono sound
     */
    bool init(int freq = 44100, bool stereo = true);

    /**
     * load a sound and retrieve a const SndInfo*.
     * If SndInfo is NULL the loading failed!
     * @note you may load a file as often as you wish! Internally it is only loaded 1 time!
     * @param file_name the filename of the soundfile
     * @return SndInfo const pointer of the SndInfo object
     */
    const SndInfo* load(const std::string& file_name);

    /**
     * the user can play a sound...
     * @param info the SndInfo (that you get by calling load)
     * @param loop if true, the sound loops
     */
    void play(const SndInfo* info, bool loop = false);
    
    /**
     * load and play a sound
     * @param file_name the filename of the soundfile
     * @param loop if true, the sound loops
     * @return const pointer of the SndInfo object
     */
    const SndInfo* play(const std::string& file_name, bool loop = false);

    /**
     * the user can stop a sound. If the sound is not played nothing happens.
     * @param info the SndInfo that should stop playing.
     */
    void stop(const SndInfo* info);

    /**
     * unload sound to free memory. Usually you call this if you have *big* sound files and few memory.
     * @param info const pointer of the SndInfo object that should be freed.
     */
    void unload(const SndInfo* info); 

    /**
     * check whether a sound is active
     * @param info const pointer of the SndInfo object
     */
    bool is_running(const SndInfo* info);

public: // static "globals"

    /**
     * container-array for all sounds.
     * (static because it is used by AudioCallback)
     */
    static std::vector <SndInfo*> snd_container;

    /**
     * This is called by the sound-card only!
     * @param user_data not used
     * @param audio raw data
     * @param length length in ms
     */
    static void AudioCallback(void* user_data, Uint8 *audio, int length);


private: // intern functions

    /**
     * frees all sound data
     */
    void free_sound_data();

    /**
     * frees the container array
     */
    void free_array();

    /**
     * load file and convert sample data;
     * used by load_sound
     * @param info pointer of the SndInfo object
     * @return true on success
     */
    bool load_and_convert_sound(SndInfo* info);

    /**
     * map id to corresponding SndInfo object.
     * @param id id of the SndInfo object
     * @return pointer of the SndInfo object with id id.
     */
    inline SndInfo* info_from_id(unsigned int id) const;


private: // member variables

    /**
     * if the Audio sub system was not initialized: true
     */
    bool b_init_audio_sub_system;

    /**
     * "saves" the number of used ids
     */
    unsigned int m_free_id;

    /**
     * the desired format
     */
    SDL_AudioSpec m_desired;

    /**
     * the real format we will use
     */
    SDL_AudioSpec m_obtained;

    /**
     * m_map_sound_table is a table that returns the SndInfo* by its id
     * (It is very fast!)
     */
    mutable std::map < unsigned int, SndInfo* > m_map_sound_table;

    /**
     * m_map_filename_table is a table that returns the SndInfo* by its filename
     * (It is very fast!)
     */
    std::map < std::string, SndInfo* > m_map_filename_table;
};

#endif // SDL_SND_H

