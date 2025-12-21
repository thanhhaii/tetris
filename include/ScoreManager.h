#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <string>
#include <vector>

struct Score {
    std::string playerName;
    int score;
};

class ScoreManager {
public:
    static void saveScore(const std::string& playerName, int score);
    static std::vector<Score> loadScores();
    static void displayScores();
    
private:
    static const std::string SCORES_FILE;
};

#endif // SCORE_MANAGER_H
