// g++ -o plaympeg plaympeg.c -I/usr/include/SDL -lSDL -lsmpeg


#ifndef _UI_ANIMATION_HPP
#define _UI_ANIMATION_HPP

#include "object.hpp"
#include "smpeg.h"

class UI_Animation : public UI_Object
{
        public:
                UI_Animation(UI_Object* animation_parent,
                                const Rect& animation_rect,
                                const Size distance_bottom_right,
                                const std::string animation_file,
                                const ePositionMode bitmap_position_mode = DO_NOT_ADJUST);
                ~UI_Animation();
                void reloadOriginalSize();
                void process();
                void object_info();
                void draw() const;
                const unsigned int getAnimationWidth() const;
                const unsigned int getAnimationHeight() const;
                const Size getAnimationSize() const;
                void setAnimationFile(const std::string animation_file);
//              void nextFrame();
        private:
                std::string animationFile;
		SMPEG *mpeg;
		SMPEG_Info info;	
		bool loop;
		
};

inline const unsigned int UI_Animation::getAnimationWidth() const {
        return(currentPicture->w);
}

inline const unsigned int UI_Animation::getAnimationHeight() const {
        return(currentPicture->h);
}

inline const Size UI_Animation::getAnimationSize() const {
        return(Size(currentPicture->w, currentPicture->h));
}

#endif // _UI_ANIMATION_HPP


int main(int argc, char *argv[])
{
    int use_audio, use_video;
    int fullscreen;
    int scalesize;
    int scale_width, scale_height;
    int i, pause;
    int volume;
    Uint32 seek;

    char *basefile;
    SDL_version sdlver;
    int fd;
    char buf[32];
    int status;

    /* Get the command line options */
    use_audio = 1;
    use_video = 1;
    fullscreen = 0;
    scalesize = 1;
    scale_width = 0;
    scale_height = 0;
    volume = 100;
    seek = 0;
    bilinear_filtering = 0;
    fd = 0;

    /* Play the mpeg files! */
    status = 0;
	
        /* Create the MPEG stream */
	{
	  if(strcmp(argv[i], "-") == 0) /* Use stdin for input */
	    mpeg = SMPEG_new_descr(0, &info, use_audio);
	  else
	    mpeg = SMPEG_new(argv[i], &info, use_audio);
	}

        if ( SMPEG_error(mpeg) ) {
            fprintf(stderr, "%s: %s\n", argv[i], SMPEG_error(mpeg));
            SMPEG_delete(mpeg);
            status = -1;
            continue;
        }
        SMPEG_enableaudio(mpeg, use_audio);
        SMPEG_enablevideo(mpeg, use_video);
        SMPEG_setvolume(mpeg, volume);

        /* Enable software bilinear filtering, if desired */
        if ( bilinear_filtering ) {
            SMPEG_Filter *filter;
            filter = SMPEGfilter_bilinear();
            filter = SMPEG_filter( mpeg, filter );
            filter->destroy(filter);
        }

        /* Print information about the video */
        basefile = strrchr(argv[i], '/');
        if ( basefile ) {
            ++basefile;
        } else {
            basefile = argv[i];
        }
        if ( info.has_audio && info.has_video ) {
            printf("%s: MPEG system stream (audio/video)\n", basefile);
        } else if ( info.has_audio ) {
            printf("%s: MPEG audio stream\n", basefile);
        } else if ( info.has_video ) {
            printf("%s: MPEG video stream\n", basefile);
        }
        if ( info.has_video ) {
            printf("\tVideo %dx%d resolution\n", info.width, info.height);
        }
        if ( info.has_audio ) {
	    printf("\tAudio %s\n", info.audio_string);
        }
        if ( info.total_size ) {
	    printf("\tSize: %d\n", info.total_size);
        }
        if ( info.total_time ) {
	    printf("\tTotal time: %f\n", info.total_time);
        }

        /* Set up video display if needed */
        if ( info.has_video && use_video ) {
            const SDL_VideoInfo *video_info;
            Uint32 video_flags;
            int video_bpp;
            int width, height;

            if ( scale_width ) {
                width = scale_width;
            } else {
                width = info.width;
            }
            width *= scalesize;
            if ( scale_height ) {
                height = scale_height;
            } else {
                height = info.height;
            }
            height *= scalesize;

            SMPEG_setdisplay(mpeg, dc->getSurface(), NULL, update);
            SMPEG_scaleXY(mpeg, screen->w, screen->h);
        } else {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        /* Set any special playback parameters */
        if ( loop_play ) {
            SMPEG_loop(mpeg, 1);
        }

	/* Seek starting position */
	if(seek) SMPEG_seek(mpeg, seek);

	/* Skip seconds to starting position */
	if(skip) SMPEG_skip(mpeg, skip);
	
        /* Play it, and wait for playback to complete */
        SMPEG_play(mpeg);
        done = 0;
	pause = 0;
        while ( ! done && ( pause || (SMPEG_status(mpeg) == SMPEG_PLAYING) ) ) {
            SDL_Event event;

            while ( use_video && SDL_PollEvent(&event) ) {
                switch (event.type) {
                    case SDL_VIDEORESIZE: {
                        SDL_Surface *old_screen = screen;
                        SMPEG_pause(mpeg);
                        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, screen->format->BitsPerPixel, screen->flags);
			if ( old_screen != screen ) {
                            SMPEG_setdisplay(mpeg, screen, NULL, update);
                        }
                        SMPEG_scaleXY(mpeg, screen->w, screen->h);
                        SMPEG_pause(mpeg);
                    } break;
                    case SDL_KEYDOWN:
                        if ( (event.key.keysym.sym == SDLK_ESCAPE) || (event.key.keysym.sym == SDLK_q) ) {
			  // Quit
			  done = 1;
                        } else if ( event.key.keysym.sym == SDLK_RETURN ) {
			  // toggle fullscreen
			  if ( event.key.keysym.mod & KMOD_ALT ) {
                            SDL_WM_ToggleFullScreen(screen);
                            fullscreen = (screen->flags & SDL_FULLSCREEN);
                            SDL_ShowCursor(!fullscreen); 
                          }
                        } else if ( event.key.keysym.sym == SDLK_UP ) {
			  // Volume up
			  if ( volume < 100 ) {
			    if ( event.key.keysym.mod & KMOD_SHIFT ) {   // 10+
			      volume += 10;
			    } else if ( event.key.keysym.mod & KMOD_CTRL ) { // 100+
			      volume = 100;
			    } else {                                     // 1+
			      volume++;
			    }
			    if ( volume > 100 ) 
			      volume = 100;
			    SMPEG_setvolume(mpeg, volume);
			  }
                        } else if ( event.key.keysym.sym == SDLK_DOWN ) {
			  // Volume down
			  if ( volume > 0 ) {
			    if ( event.key.keysym.mod & KMOD_SHIFT ) {
			      volume -= 10;
			    } else if ( event.key.keysym.mod & KMOD_CTRL ) {
			      volume = 0;
			    } else {
			      volume--;
			    }
			    if ( volume < 0 ) 
			      volume = 0;
			    SMPEG_setvolume(mpeg, volume);
			  }
                        } else if ( event.key.keysym.sym == SDLK_PAGEUP ) {
			  // Full volume
			  volume = 100;
			  SMPEG_setvolume(mpeg, volume);
                        } else if ( event.key.keysym.sym == SDLK_PAGEDOWN ) {
			  // Volume off
			  volume = 0;
			  SMPEG_setvolume(mpeg, volume);
                        } else if ( event.key.keysym.sym == SDLK_SPACE ) {
			  // Toggle play / pause
			  if ( SMPEG_status(mpeg) == SMPEG_PLAYING ) {
			    SMPEG_pause(mpeg);
			    pause = 1;
			  } else {
			    SMPEG_play(mpeg);
			    pause = 0;
			  }
			} else if ( event.key.keysym.sym == SDLK_RIGHT ) {
			  // Forward
			  if ( event.key.keysym.mod & KMOD_SHIFT ) {
			    SMPEG_skip(mpeg, 100);
			  } else if ( event.key.keysym.mod & KMOD_CTRL ) {
			    SMPEG_skip(mpeg, 50);
			  } else {
			    SMPEG_skip(mpeg, 5);
			  }
                        } else if ( event.key.keysym.sym == SDLK_LEFT ) {
			  // Reverse
			  if ( event.key.keysym.mod & KMOD_SHIFT ) {

			  } else if ( event.key.keysym.mod & KMOD_CTRL ) {

			  } else {

			  }
                        } else if ( event.key.keysym.sym == SDLK_KP_MINUS ) {
			  // Scale minus
			  if ( scalesize > 1 ) {
			    scalesize--;
			  }
                        } else if ( event.key.keysym.sym == SDLK_KP_PLUS ) {
			  // Scale plus
			  scalesize++;
			} else if ( event.key.keysym.sym == SDLK_f ) {
			  // Toggle filtering on/off
			  if ( bilinear_filtering ) {
			    SMPEG_Filter *filter = SMPEGfilter_null();
			    filter = SMPEG_filter( mpeg, filter );
			    filter->destroy(filter);
			    bilinear_filtering = 0;
			  } else {
			    SMPEG_Filter *filter = SMPEGfilter_bilinear();
			    filter = SMPEG_filter( mpeg, filter );
			    filter->destroy(filter);
			    bilinear_filtering = 1;
			  }
			}
                        break;
                    case SDL_QUIT:
                        done = 1;
                        break;
                    default:
                        break;
                }
            }
            SDL_Delay(1000/2);
        }
        SMPEG_delete(mpeg);
    }
    SDL_Quit();

    return(status);
}
