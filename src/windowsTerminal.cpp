
#ifdef _WIN32

#include "include/windowsTerminal.h"

TerminalMode::TerminalMode() {
}

TerminalMode::~TerminalMode() {
    disableRawMode();
}

void TerminalMode::enableRawMode() {
    // Raw mode lets us type in without having the prompt or
    // errors when the command does not exist

    // Saving the state before changing
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);


    // FANCY Windows stuff to get the current mode of the console
    if (!GetConsoleMode(hIn, &dwOriginalInMode) || !GetConsoleMode(hOut, &dwOriginalOutMode)) {
        std::cerr << "Error getting console mode" << std::endl;
        return;
    }

    // FANCY Windows stuff disable the standard terminal input rules
    DWORD dwNewInMode = dwOriginalInMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_WINDOW_INPUT) | ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!SetConsoleMode(hIn, dwNewInMode)) {
        std::cerr << "Error setting console input mode" << std::endl;
        return;
    }

    // FANCY Windows stuff disable the standard terminal output rules
    DWORD dwNewOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    if (!SetConsoleMode(hOut, dwNewOutMode)) {
        std::cerr << "Error setting console output mode" << std::endl;
        return;
    }
}

void TerminalMode::disableRawMode() {
    // Restore back to what mode it was in before
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
        // Windows coordinates are 0 based, so convert to 1 based cause microsoft
        return {csbi.dwCursorPosition.X + 1, csbi.dwCursorPosition.Y + 1};
    }
    return {0, 0}; // Return an error state so it dont break
}


int TerminalMode::readKey() {

    // Yo why this windows stuff so complicated like simplicity can go a long way
    DWORD read;
    INPUT_RECORD ir;
    do {
        ReadConsoleInput(hIn, &ir, 1, &read);
    } while (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown);


    // Weird Uppercase names not a big fan
    int asciiValue = ir.Event.KeyEvent.uChar.AsciiChar;
    /* std::cout << "Read ASCII: " << asciiValue << std::endl;  // Debug output */
    return asciiValue;

}

void TerminalMode::clearScreen() {
    DWORD n; 
    DWORD size;
    COORD coord = {0}; 
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Get the number of character cells in the current buffer
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;
    size = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire buffer with spaces
    FillConsoleOutputCharacter(hOut, (TCHAR) ' ', size, coord, &n);
    GetConsoleScreenBufferInfo(hOut, &csbi);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, size, coord, &n);

    // Move the cursor back to the top left position
    SetConsoleCursorPosition(hOut, coord);

}

void TerminalMode::moveCursor(int x, int y) {
    // Allows us to mvoe the cursors to draw what we want
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, pos);

}



void TerminalMode::setTextColor(int foreground) {
    // Changes the colors
    if (foreground >= 0) {
        SetConsoleTextAttribute(hOut, foreground);
    }

}


std::pair<int, int> TerminalMode::getTerminalDimensions() {
    // This is so that we can see when we have to go to the next line
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    // More complicated windows stuff with weird names
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return {columns, rows};
    }
    return {0, 0}; // Unable to get the terminal area
}




#endif
