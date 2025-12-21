#include "../include/ScoreManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

const string ScoreManager::SCORES_FILE = "scores.txt";

void ScoreManager::saveScore(const string& playerName, int score) {
    vector<Score> scores = loadScores();
    
    // Add new score
    scores.push_back({playerName, score});
    
    // Sort scores in descending order
    sort(scores.begin(), scores.end(), 
              [](const Score& a, const Score& b) { return a.score > b.score; });
    
    // Keep only top 10 scores
    if (scores.size() > 10) {
        scores.resize(10);
    }
    
    // Write to file
    ofstream file(SCORES_FILE);
    for (const auto& s : scores) {
        file << s.playerName << " " << s.score << "\n";
    }
    file.close();
}

vector<Score> ScoreManager::loadScores() {
    vector<Score> scores;
    ifstream file(SCORES_FILE);
    
    if (!file.is_open()) {
        return scores;  // Return empty vector if file doesn't exist
    }
    
    string name;
    int score;
    while (file >> name >> score) {
        scores.push_back({name, score});
    }
    file.close();
    
    return scores;
}

void ScoreManager::displayScores() {
    vector<Score> scores = loadScores();
    
    cout << "\n\033[1m\033[36m";  // Bold Cyan
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║            HIGH SCORES (TOP 10)            ║\n";
    cout << "╠════════════════════════════════════════════╣\n";
    
    if (scores.empty()) {
        cout << "║              No scores yet!               ║\n";
    } else {
        for (size_t i = 0; i < scores.size(); i++) {
            cout << "║ " << setw(2) << (i+1) << ". "
                      << setw(20) << left << scores[i].playerName 
                      << setw(10) << right << scores[i].score << " ║\n";
        }
    }
    
    cout << "╚════════════════════════════════════════════╝\n";
    cout << "\033[0m";  // Reset
}
