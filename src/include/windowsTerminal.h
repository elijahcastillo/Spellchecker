
// TerminalMode.h
#ifndef TERMINAL_MODE_H
#define TERMINAL_MODE_H

#ifdef _WIN32


#include <iostream>
#include <windows.h>

#define TERMINAL_RED FOREGROUND_RED
#define TERMINAL_GREEN FOREGROUND_GREEN
#define TERMINAL_BLUE FOREGROUND_BLUE
#define TERMINAL_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define TERMINAL_MAGENTA (FOREGROUND_RED | FOREGROUND_BLUE)
#define TERMINAL_CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define TERMINAL_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define TERMINAL_BLACK 0 // No color

// Defining light grey by combining red, green, blue, and intensity
#define BACKGROUND_LIGHT_GREY (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

// A reset color combination (assuming your terminal's default is white text on black background)
// You might adjust these if your default terminal colors are different
#define DEFAULT_FOREGROUND_COLOR TERMINAL_WHITE
#define DEFAULT_BACKGROUND_COLOR 0 // Typically, no background color set means black

class TerminalMode {
public:
    TerminalMode();
    ~TerminalMode();

    void enableRawMode();
    void disableRawMode();
    int readKey();
    void clearScreen();
    void moveCursor(int x, int y);
    void clearLine();
    std::pair<int, int> getCursorPosition();
    std::pair<int, int> getTerminalDimensions();
    void setTextColor(int foreground);

    void setTextAndBackgroundColor(int foregroundColor, int backgroundColor);
private:

    HANDLE hIn;
    HANDLE hOut;
    DWORD dwOriginalInMode, dwOriginalOutMode;
};


#endif // _WIN32 

#endif // TERMINAL_MODE_H
