#pragma once

#ifdef _WIN32
#include <windows.h>
#include <wincon.h>
const unsigned FORE_BLACK = 0;
const unsigned FORE_RED = FOREGROUND_RED;
const unsigned FORE_GREEN = FOREGROUND_GREEN;
const unsigned FORE_BLUE = FOREGROUND_BLUE;
const unsigned FORE_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;
const unsigned FORE_PURPLE = FOREGROUND_RED | FOREGROUND_BLUE;
const unsigned FORE_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE;
const unsigned FORE_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

const unsigned BACK_BLACK = 0;
const unsigned BACK_RED = BACKGROUND_RED;
const unsigned BACK_GREEN = BACKGROUND_GREEN;
const unsigned BACK_BLUE = BACKGROUND_BLUE;
const unsigned BACK_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN;
const unsigned BACK_PURPLE = BACKGROUND_RED | BACKGROUND_BLUE;
const unsigned BACK_CYAN = BACKGROUND_GREEN | BACKGROUND_BLUE;
const unsigned BACK_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
#else
#include <iostream>
const std::string FORE_BLACK = "\u001B[30m";
const std::string FORE_RED = "\u001B[31m";
const std::string FORE_GREEN = "\u001B[32m";
const std::string FORE_YELLOW = "\u001B[33m";
const std::string FORE_BLUE = "\u001B[34m";
const std::string FORE_PURPLE = "\u001B[35m";
const std::string FORE_CYAN = "\u001B[36m";
const std::string FORE_WHITE = "\u001B[37m";
const std::string BACK_BLACK = "\u001B[40m";
const std::string BACK_RED = "\u001B[41m";
const std::string BACK_GREEN = "\u001B[42m";
const std::string BACK_YELLOW = "\u001B[43m";
const std::string BACK_BLUE = "\u001B[44m";
const std::string BACK_PURPLE = "\u001B[45m";
const std::string BACK_CYAN = "\u001B[46m";
const std::string BACK_WHITE = "\u001B[47m";
#endif

struct ConColors
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
	HANDLE hCon;
	ConColors() {
		hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hCon, &csbiScreenInfo);
	}
	void setColour(const unsigned colour) {
		SetConsoleTextAttribute(hCon, colour);
	}
	}
	void reset() {
		SetConsoleTextAttribute(hCon, csbiScreenInfo.wAttributes);
	}
#else
    ConColors() {}
    void setColour(const std::string colour) {
        std::cout << colour;
	}

    void reset() {
        const std::string ANSI_RESET = "\u001B[0m";
        std::cout << ANSI_RESET;
    }
#endif

};
