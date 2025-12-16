#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

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
int x, y, b;

/* ================= INPUT CROSS PLATFORM ================= */

#ifdef _WIN32
int getch_cross() { return _getch(); }
bool kbhit_cross() { return _kbhit(); }
void sleep_ms(int ms) { Sleep(ms); }
void clearScreen() { system("cls"); }

// Enable colors on Windows
void enableColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
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

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void enableColors() {}
#endif

/* ================= COLOR CODES ================= */
#define RESET   "\033[0m"
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
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            char c = board[i][j];
            cout << getColor(c) << getBlockChar(c) << RESET;
        }
        cout << endl;
    }
}

void removeLine() {
    for (int i = H-2; i > 0; i--) {
        bool full = true;
        for (int j = 1; j < W-1; j++)
            if (board[i][j] == ' ') {
                full = false;
                break;
            }

        if (full) {
            for (int ii = i; ii > 1; ii--)
                for (int jj = 1; jj < W-1; jj++)
                    board[ii][jj] = board[ii-1][jj];
            i++;
            draw();
            sleep_ms(200);
        }
    }
}

/* ================= MAIN ================= */

int main() {
    enableColors();
    srand(time(0));
    initBoard();
    x = 6; y = 1; b = rand() % 7;

    while (true) {
        boardDelBlock();

        if (kbhit_cross()) {
            char c = getch_cross();
            if (c == 'a' && canMove(-1,0)) x--;
            if (c == 'd' && canMove(1,0))  x++;
            if (c == 's' && canMove(0,1))  y++;
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
        }

        block2Board();
        draw();
        sleep_ms(200);
    }

    return 0;
}