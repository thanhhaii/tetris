#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "./include/AudioManager.h"
#include "./include/ConsoleHelper.h"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

using namespace std;

#define H 22
#define W 16

char board[H][W];
int x, y, b, score, speed = 300;

/* ================= INPUT CROSS PLATFORM ================= */

#ifdef _WIN32
int getch_cross() { return _getch(); }
bool kbhit_cross() { return _kbhit(); }
void sleep_ms(int ms) { Sleep(ms); }
#else
int getch_cross() {
    termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

bool kbhit_cross() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
#endif

/* ================= COLOR CODES ================= */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"

/* ================= TETRIS DATA ================= */

char blocks[][4][4] = {
    {{' ','I',' ',' '},{' ','I',' ',' '},{' ','I',' ',' '},{' ','I',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','T',' ',' '},{'T','T','T',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','S','S',' '},{'S','S',' ',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'Z','Z',' ',' '},{' ','Z','Z',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'J',' ',' ',' '},{'J','J','J',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ',' ','L',' '},{'L','L','L',' '},{' ',' ',' ',' '}}
};

/* ================= HELPER FUNCTIONS ================= */

string getColor(char c) {
    switch(c) {
        case 'I': return CYAN;
        case 'O': return YELLOW;
        case 'T': return MAGENTA;
        case 'S': return GREEN;
        case 'Z': return RED;
        case 'J': return BLUE;
        case 'L': return WHITE;
        case '#': return GRAY;
        default: return RESET;
    }
}

string getBlockChar(char c) {
    if (c == ' ') return "  ";
    if (c == '#') return "██";  // Viền
    return "▓▓";  // Khối Tetris
}

/* ================= GAME LOGIC ================= */

void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++) {
            board[i][j] = (i == 0 || i == H-1 || j == 0 || j == W-1) ? '#' : ' ';
        }
}

bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt <= 0 || xt >= W-1 || yt >= H-1) return false;
                if (board[yt][xt] != ' ') return false;
            }
    return true;
}

bool canRotate(char rotated[4][4]){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (rotated[i][j] != ' '){
                int xt = x + j;
                int yt = y + i;
                if (xt <= 0 || xt >= W-1 || yt >= H-1 || yt <= 0) return false;
                if (board[yt][xt] != ' ') return false;
            }
        }
    }

    return true;
}

void rotateBlock() {
    char rotated[4][4];
    
    // Xoay ma trận 90 độ: rotated[i][j] = blocks[b][3-j][i]
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            rotated[i][j] = blocks[b][3-j][i];
    
    // Kiểm tra xem có thể xoay không
    if (canRotate(rotated)) {
        // Áp dụng rotation
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                blocks[b][i][j] = rotated[i][j];
    }
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = blocks[b][i][j];
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = ' ';
}

void draw() {
    clearScreen();

    cout << BOLD << GREEN << "Score: " << score << "\t" << BOLD << WHITE << "Speed: " << speed << "ms" << "\n\n" << RESET;

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            char c = board[i][j];
            cout << getColor(c) << getBlockChar(c) << RESET;
        }
        cout << endl;
    }
}

void lineClearAnimation(int row) {
    for (int t = 0; t < 3; t++) {
        // Nhấp nháy
        for (int j = 1; j < W-1; j++)
            board[row][j] = (t % 2 == 0) ? '#' : ' ';

        draw();
        sleep_ms(80);
    }
}

void calculateScore(int& score, int linesCleared) {
    switch (linesCleared) {
        case 1: score += 100; break;
        case 2: score += 300; break;
        case 3: score += 500; break;
        case 4: score += 800; break;
        default: break;
    }
};

void removeLine() {
    int clearedCount = 0;

    for (int i = H-2; i > 0; i--) {
        bool full = true;
        for (int j = 1; j < W-1; j++)
            if (board[i][j] == ' ') {
                full = false;
                break;
            }

        if (full) {
            AudioManager::playSound("./assets/collect-points.mp3");
            lineClearAnimation(i);
            for (int ii = i; ii > 1; ii--)
                for (int jj = 1; jj < W-1; jj++)
                    board[ii][jj] = board[ii-1][jj];

            // Xoá dòng trên cùng
            for (int jj = 1; jj < W-1; jj++)
                board[1][jj] = ' ';

            i++; // kiểm tra lại dòng vừa rơi xuống
            clearedCount++;
            
            speed = max(50, speed - 10);
            draw();
            sleep_ms(80);
        }

    }

    if (clearedCount > 0) {
        calculateScore(score, clearedCount);
    };
}

void screenShake(int times = 10, int strength = 2, int delay = 30) {
    for (int i = 0; i < times; i++) {
        clearScreen();

        int offset = (i % 2 == 0) ? strength : -strength;

        for (int y = 0; y < H; y++) {
            if (offset > 0)
                cout << string(offset * 2, ' ');

            for (int x = 0; x < W; x++) {
                char c = board[y][x];
                cout << getColor(c) << getBlockChar(c) << RESET;
            }
            cout << endl;
        }
        sleep_ms(delay);
    }
}

/* ================= MAIN ================= */

int main() {
    AudioManager::init();
    enableColors();
    srand(time(0));
    initBoard();
    x = 6; y = 1; b = rand() % 7;
    score = 0;
    
    hideCursor(); 
    disableEcho();

    AudioManager::playBackgroundMusic("./assets/background-music.mp3", false);

    cout << BOLD << CYAN;
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║                                                  ║\n";
    cout << "║           WELCOME TO TETRIS GAME!                ║\n";
    cout << "║                                                  ║\n";
    cout << "║         Press any key to start...                ║\n";
    cout << "║                                                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n" << RESET;
    getch_cross();
    AudioManager::stopBackgroundMusic();
    sleep_ms(300);

    bool isPlayingBackgroundMusic = false;

    while (true) {
        if (isPlayingBackgroundMusic == false) {
            isPlayingBackgroundMusic = true;
            AudioManager::playBackgroundMusic("./assets/game-sound.mp3", true);
        }
        

        boardDelBlock();

        if (kbhit_cross()) {
            char c = getch_cross();
            if (c == 'a' && canMove(-1,0)) x--;
            if (c == 'd' && canMove(1,0))  x++;
            if (c == 's' && canMove(0,1))  y++;
            if (c == 'w') rotateBlock();
            if (c == 'q') break;

            block2Board();
            draw();
            sleep_ms(50);
            continue;
        }

        if (canMove(0,1)) y++;
        else {
            block2Board();
            removeLine();
            x = 6; y = 1; b = rand() % 7;

            if(!canMove(0,1)){
                AudioManager::stopBackgroundMusic();
                block2Board();
                screenShake(12, 1, 25);
                clearScreen();
                cout << "\n" << BOLD << RED;
                cout << "  ╔════════════════════════════════════════════\n";
                cout << "  ║           GAME OVER!                       \n";
                cout << "  ║         Final Score:     " << score << "   \n";
                cout << "  ╚════════════════════════════════════════════\n" << RESET;
                break;
            }

        }

        block2Board();
        draw();
        sleep_ms(speed);
    }

    resetTerminal();
    return 0;
}