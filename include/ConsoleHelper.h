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

void resetTerminal();
void disableEcho();
void enableEcho();
void clearScreen();
void enableColors();
void hideCursor();