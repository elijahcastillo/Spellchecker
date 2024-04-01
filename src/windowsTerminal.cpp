
#ifdef _WIN32

#include "include/windowsTerminal.h"

TerminalMode::TerminalMode() {
}

TerminalMode::~TerminalMode() {
    disableRawMode();
}

void TerminalMode::enableRawMode() {
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!GetConsoleMode(hIn, &dwOriginalInMode) || !GetConsoleMode(hOut, &dwOriginalOutMode)) {
        std::cerr << "Error getting console mode" << std::endl;
        return;
    }

    DWORD dwNewInMode = dwOriginalInMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_WINDOW_INPUT) | ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!SetConsoleMode(hIn, dwNewInMode)) {
        std::cerr << "Error setting console input mode" << std::endl;
        return;
    }

    DWORD dwNewOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    if (!SetConsoleMode(hOut, dwNewOutMode)) {
        std::cerr << "Error setting console output mode" << std::endl;
        return;
    }
}

void TerminalMode::disableRawMode() {
    SetConsoleMode(hIn, dwOriginalInMode);
    SetConsoleMode(hOut, dwOriginalOutMode);
}



void TerminalMode::setTextAndBackgroundColor(int foregroundColor, int backgroundColor) {
    // Combine foreground and background colors
    DWORD color = foregroundColor | backgroundColor;
    SetConsoleTextAttribute(hOut, color);
}



std::pair<int, int> TerminalMode::getCursorPosition() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        // windows coordinates are 0 based, so convert to 1 based 
        return {csbi.dwCursorPosition.X + 1, csbi.dwCursorPosition.Y + 1};
    }
    return {0, 0}; // Return an error state
}


int TerminalMode::readKey() {
    DWORD read;
    INPUT_RECORD ir;
    do {
        ReadConsoleInput(hIn, &ir, 1, &read);
    } while (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown);
    return ir.Event.KeyEvent.uChar.AsciiChar;

}

void TerminalMode::clearScreen() {
    DWORD n; 
    DWORD size;
    COORD coord = {0}; 
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;
    size = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire buffer with spaces
    FillConsoleOutputCharacter(hOut, (TCHAR) ' ', size, coord, &n);
    GetConsoleScreenBufferInfo(hOut, &csbi);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, size, coord, &n);

    // Move the cursor back to the top left position.
    SetConsoleCursorPosition(hOut, coord);

}

void TerminalMode::moveCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, pos);

}



void TerminalMode::setTextColor(int foreground) {
    // Use the color defines directly instead of indexing into COLOR_MAP
    if (foreground >= 0) {
        // Here, we assume `foreground` is one of the defined color macros like TERMINAL_RED
        SetConsoleTextAttribute(hOut, foreground);
    }

}


std::pair<int, int> TerminalMode::getTerminalDimensions() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return {columns, rows};
    }
    return {0, 0}; // Unable to get 
}




#endif
