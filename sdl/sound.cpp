/***************************************************************************
 *   see header file for more information!
 ***************************************************************************/

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

// BEGIN includes
#include "sound.hpp"
// END includes

/**
 * definition of static variable snd_container
 */
std::vector < SndInfo* > SDL_snd::snd_container;

SDL_snd::SDL_snd() {}

SDL_snd::~SDL_snd()
{
    // pause the audio before deleting the sample data
    SDL_PauseAudio(1);

    // lock Audio, free sample data and unlock audio
    SDL_LockAudio();
    free_sound_data();
    free_array();
    m_map_sound_table.clear();
    m_map_filename_table.clear();
    SDL_UnlockAudio();

    // shut down the audio sub system cleanly
//    printf("SND: Shutting down audio sub system: OK\n"); 
    if (b_init_audio_sub_system == true)
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    SDL_CloseAudio();
}

// BEGIN THE Audio-Callback-FUNCTION - called from the sound-card
void SDL_snd::AudioCallback(void*, Uint8 *audio, int length)
{
    // Clear the audio buffer so we can mix samples into it.
    memset(audio, 0, length);

    // Mix in each sound.
    std::vector <SndInfo*>::iterator it;

    for (it = snd_container.begin(); it != snd_container.end(); ++it)
    {
	    std::vector <SndInfo::SndPlayInfos>::iterator pos;

        for (pos = (*it)->positions.begin(); pos != (*it)->positions.end(); )
        {
            // position and sound variables for mixing
            Uint8 *sound_buf;
            Uint32 sound_len;

            // extra data and position variables
            // needed, when the sound transition in loopmode happens
            Uint8 *snd_loop_buf;
            Uint32 snd_loop_len;

            // Locate this sound's current buffer position.
            sound_buf = (*it)->samples;
            sound_buf += pos->position;

            // need extra handling when bTransition is true, that means in 
            // loopmode a transition happens
            bool bTransition = false;

            // Determine the number of samples to mix OR handle loop
            if ((pos->position + length) > (*it)->length_in_bytes)
            {
                // calculate the difference between position and the end of the sample data
                sound_len = (*it)->length_in_bytes - pos->position;

                // if loop is true: extra handling ;)
                if (pos->loop)
                {
                    bTransition = true;

                    // the length of the difference between pos and end of sample data
                    snd_loop_len = sound_len;

                    // allocate new sample data for the extra handling mixing bla...
                    snd_loop_buf = (Uint8 *) malloc(length);

                    // copy the end of the original sample data
                    memcpy(snd_loop_buf, sound_buf, sound_len);
                    sound_buf = (*it)->samples;

                    // copy the beginning of the origional sample data
                    memcpy(snd_loop_buf + sound_len, sound_buf, length - snd_loop_len);

                    // now the sound_len is again length!
                    sound_len = length;
                }
            }
            else
            {
                sound_len = length;
            }

            if (bTransition)
            {
                // Mix in the sample from extra handling ;) (see above)
                SDL_MixAudio(audio, snd_loop_buf, sound_len, SDL_MIX_MAXVOLUME);		
            }
            else
            {
                // Mix this sound into the stream.
                SDL_MixAudio(audio, sound_buf, sound_len, SDL_MIX_MAXVOLUME);
            }

            // Update the sound buffer's position.
            if (bTransition)
            {
                pos->position = length - snd_loop_len;
                
                // free the extra-sound buffer
                free(snd_loop_buf);
            }
            else
            {
                pos->position += length;
            }

            // Have we reached the end of the sound?
            if (pos->position >= (*it)->length_in_bytes)
            {
                // NOTE: we do not need loop handling here. It was handled already earlier...

                // delete from position-array
                pos = (*it)->positions.erase(pos);
            }
            else
            {
                ++pos;
            }
        }
    }
}
// END THE Audio-Callback-FUNCTION - called from the sound-card

// BEGIN THE PUBLIC FUNCTIONS FOR THE USER
bool SDL_snd::init(int freq, bool stereo)
{
    b_init_audio_sub_system = false;

    // check whether the user has already initialized the audio sub system
    if (!( SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO ))
    {
        // initialize the sub-system now
        SDL_InitSubSystem(SDL_INIT_AUDIO);
        b_init_audio_sub_system = true;
//        printf("SND: SDL audio sub system initialized.\n");
    }

    // Open the audio device. The sound driver will try to give us
    // the requested format, but it might not succeed. The 'obtained'
    // structure will be filled in with the actual format data.

    m_desired.freq      = freq;                  // desired output sample rate
    m_desired.format    = AUDIO_S16;             // request signed 16-bit samples
    m_desired.samples   = 1024;                  // this is more or less discretionary
    m_desired.channels  = (stereo==true)?2:1;    // ask for stereo
    m_desired.callback  = SDL_snd::AudioCallback;
    m_desired.userdata  = NULL;                  // we don't need this

    // Open the AudioDevice with the requested format
    if (SDL_OpenAudio(&m_desired, &m_obtained) < 0)
    {
        printf("SND: Unable to open audio device: '%s'\n", SDL_GetError());
        return false;
    }

    // Stop audio at the beginning
    SDL_PauseAudio(1);

    // set free_id to 0 because no id is used yet
    m_free_id = 0;

    // initialization successful
//    printf("SND: Audio device opened\n");
    return true;
}

const SndInfo* SDL_snd::load(const std::string& file_name)
{
	std::map < std::string, SndInfo* >::iterator it;

    // check whether the file is loaded already because I want to avoid to load a sound twice
    it = m_map_filename_table.find(file_name);
    if ( it == m_map_filename_table.end() )
    {
        // create a new soundinfo first and put in the file_name
        SndInfo* new_sound_info = new SndInfo;
        new_sound_info->file_name = file_name;

        // initialize samples as NULL because if load_and_convert_sound failes
        // the "else­routine" tests whether samples is NULL or not
        new_sound_info->samples = NULL;

        // try to load and convert sound
        if (load_and_convert_sound(new_sound_info) == true)
        {
            // set the sound's id to m_free_id
            new_sound_info->snd_id = m_free_id++;

            // add new sound data to the sound-array
            snd_container.push_back(new_sound_info);

            // add the new sound to the assignment tables
            m_map_sound_table    [new_sound_info->snd_id   ] = new_sound_info;
            m_map_filename_table [new_sound_info->file_name] = new_sound_info;

            return new_sound_info;
        }
        else
        {
            if ( new_sound_info != NULL )
            {
                if (new_sound_info->samples != NULL)
                    free( new_sound_info->samples );
                delete new_sound_info;
            }

            return NULL;
        }
    }
    else
    {
        // the sound is already loaded => simply return it's pointer
//        printf("SND: using already loaded file: '%s'\n", file_name.c_str());
        return it->second;
    }
}

void SDL_snd::unload(const SndInfo* info)
{
    // if info && inf exists
    if (info != NULL)
    {
        SndInfo* inf = info_from_id(info->snd_id);

        if (inf != NULL)
        {
            // Do not call AudioCallback
            SDL_LockAudio();

            // free sample-data
            // remove entries from both maps
            // remove sound from container
            free(inf->samples);
            m_map_sound_table.erase(inf->snd_id);
            m_map_filename_table.erase(inf->file_name);

            // clear position array
            inf->positions.clear();

	    std::vector <SndInfo*>::iterator it = find(snd_container.begin(), snd_container.end(), inf);

            if (it != snd_container.end())
            {
//                printf("SND: Freeing memory from: '%s'\n", (*it)->file_name.c_str());

                delete *it;
                snd_container.erase(it);
            }

            // allow call AudioCallback
            SDL_UnlockAudio();
        }
    }
}

void SDL_snd::play(const SndInfo* info, bool loop)
{
    // add new info to the "position-list"
    if (info != NULL)
    {
        // create a new "Info-table"
        SndInfo::SndPlayInfos snd_play_infos;

        // set standard values
        snd_play_infos.position = 0;
        snd_play_infos.loop = loop;

        SndInfo* inf = info_from_id(info->snd_id);
        if ( inf != NULL )
        {
            SDL_LockAudio();
            inf->positions.push_back(snd_play_infos);
            SDL_UnlockAudio();
        }
    } 
}

const SndInfo* SDL_snd::play(const std::string& file_name, bool loop)
{
    const SndInfo* info = load(file_name);
    play(info, loop);
    
    return info;
}

void SDL_snd::stop(const SndInfo* info)
{
    if (info != NULL)
    {
        SndInfo* inf = info_from_id(info->snd_id);
        if ( inf != NULL )
        {
            SDL_LockAudio();
            inf->positions.clear();
            SDL_UnlockAudio();
        }
    }
}

bool SDL_snd::is_running(const SndInfo* info)
{
    if (info != NULL)
    {
        SndInfo* inf = info_from_id(info->snd_id);
        if ( inf != NULL )
        {
            if (inf->positions.size() > 0) return true;
        }
    }
    return false;
}
// END THE PUBLIC FUNCTIONS FOR THE USER

// BEGIN THE PRIVATE FUNCTIONS FOR INTERN USE
void SDL_snd::free_sound_data()
{
    // delete all samples... (really important - otherwise: memory leaks)
	std::vector <SndInfo*>::iterator it;

    for (it = snd_container.begin(); it != snd_container.end(); ++it)
    {
        if ( (*it)->samples != NULL )
        {
            free ((*it)->samples);
//            printf("SND: Freeing memory from: '%s'\n", (*it)->file_name.c_str());
        }
    }
}

void SDL_snd::free_array()
{
	std::vector < SndInfo* >::iterator it;

    for ( it = snd_container.begin(); it != snd_container.end(); ++it )
    {
        // delete the pointers...
        if (*it)
            delete (*it);
    }

    snd_container.clear();
}

bool SDL_snd::load_and_convert_sound(SndInfo* info)
{
    SDL_AudioCVT cvt;     // audio format conversion structure
    SDL_AudioSpec loaded; // format of the loaded data
    Uint8 *new_buf;       // new buffer for audio conversion

    // Load the WAV file in its original sample format.
    if ( SDL_LoadWAV(info->file_name.c_str(), &loaded, &info->samples, &info->length_in_bytes ) == NULL)
    {
        printf("SND: Unable to load sound: '%s'\n", SDL_GetError());
        return false;
    }

    // Build a conversion structure for converting the samples.
    // This structure contains the data SDL needs to quickly
    // convert between sample formats.
    if (SDL_BuildAudioCVT(&cvt, loaded.format,
                loaded.channels,
                loaded.freq,
                m_obtained.format, m_obtained.channels, m_obtained.freq) < 0)
    {
        printf("SND: Unable to convert sound: '%s'\n", SDL_GetError());
        return false;
    }

    // Since converting PCM samples can result in more data
    // (for instance, converting 8-bit mono to 16-bit stereo),
    // we need to allocate a new buffer for the converted data.
    // Fortunately SDL_BuildAudioCVT supplied the necessary
    // information.
    cvt.len = info->length_in_bytes;
    new_buf = (Uint8 *) malloc(cvt.len * cvt.len_mult);

    if (new_buf == NULL)
    {
        printf("SND: Memory allocation failed.\n");
        SDL_FreeWAV(info->samples);
        return false;
    }

    // Copy the sound samples into the new buffer.
    memcpy(new_buf, info->samples, info->length_in_bytes);

    // Perform the conversion on the new buffer.
    cvt.buf = new_buf;
    if (SDL_ConvertAudio(&cvt) < 0)
    {
        printf("SND: Audio conversion error: '%s'\n", SDL_GetError());
        free(new_buf);
        SDL_FreeWAV(info->samples);
        return false;
    }

    // Swap the converted data for the original.
    SDL_FreeWAV(info->samples);
    info->samples = new_buf;
    info->length_in_bytes *= cvt.len_mult;

    // set the property: length in seconds
    // multibly by two because we use always the sample bit rate 16 bit: AUDIO_S16
    info->length_in_ms = (static_cast<double>(info->length_in_bytes) / static_cast<double>(( m_obtained.freq * m_obtained.channels * 2 ))) * 1000.0;

     // heidiho! Success :)
//    printf("SND: load & convert ok: '%s'\n", info->file_name.c_str());

    return true;
}

inline SndInfo* SDL_snd::info_from_id(unsigned int id) const
{
    return m_map_sound_table[id];
}

// END THE PRIVATE FUNCTIONS FOR INTERN USE

