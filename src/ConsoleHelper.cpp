#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif
#include <iostream>

#include "../include/AudioManager.h"

using namespace std;

#ifdef _WIN32 
void clearScreen() { system("cls"); }
void enableColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#else 
void clearScreen() {
    cout << "\033[2J\033[1;1H" << flush;
}

void enableColors() {}
#endif

void disableEcho() {
#ifndef _WIN32
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHO;  // Tắt echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
}

void enableEcho() {
#ifndef _WIN32
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHO;  // Bật echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
}

void resetTerminal() {
    cout << "\033[0m";        // Reset colors
    cout << "\033[?25h";      // Show cursor
    enableEcho();
    cout.flush();
}

void hideCursor() {
    cout << "\033[?25l";
}