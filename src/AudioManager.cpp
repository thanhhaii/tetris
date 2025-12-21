#include "../include/AudioManager.h"

#ifndef _WIN32
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <sys/wait.h>

static pid_t bgMusicPid = -1;

static void cleanup() {
    if (bgMusicPid > 0) {
        kill(bgMusicPid, SIGTERM);
        waitpid(bgMusicPid, nullptr, 0);
        bgMusicPid = -1;
    }
}

static void onExit(int) {
    cleanup();
    _exit(0);
}

void AudioManager::init() {
    signal(SIGINT, onExit);
    signal(SIGTERM, onExit);
    atexit(cleanup);
}

void AudioManager::playSound(const char* filepath) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("afplay", "afplay", filepath, nullptr);
        _exit(0);
    }
}

void AudioManager::playBackgroundMusic(const char* filepath, bool loop) {
    stopBackgroundMusic(); // đảm bảo không chồng nhạc

    bgMusicPid = fork();
    if (bgMusicPid == 0) {
        if (loop) {
            while (true) {
                pid_t p = fork();
                if (p == 0) {
                    execlp("afplay", "afplay", filepath, nullptr);
                    _exit(0);
                }
                waitpid(p, nullptr, 0); // chờ afplay xong
            }
        } else {
            execlp("afplay", "afplay", filepath, nullptr);
        }
        _exit(0);
    }
}

void AudioManager::stopBackgroundMusic() {
    cleanup();
}

#endif
