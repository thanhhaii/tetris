#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#ifndef _WIN32
#include <sys/types.h>
#endif

class AudioManager {
public:
    static void init();
    static void playSound(const char* filepath);
    static void playBackgroundMusic(const char* filepath, bool loop = false);
    static void stopBackgroundMusic();
};

#endif
