#include "../include/UIHelper.h"
#include "../include/ConsoleHelper.h"

void printEndGameScreen(int score) {
    string scoreStr = to_string(score);
    if (scoreStr.length() < 8) {
        scoreStr = string(8 - scoreStr.length(), '0') + scoreStr;
    }

    cout << BOLD << RED;
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║                                                  ║\n";
    cout << "║                  GAME OVER!                      ║\n";
    cout << "║                                                  ║\n";
    cout << "║             Your final score: " << scoreStr << "           ║\n";
    cout << "║                                                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n" << RESET;
}