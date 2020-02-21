#pragma once
#include "main.h"

using namespace std;

enum CONSOLE_TYPE {
	none,
	info,
	success,
	warn,
	error,
	bold
};

namespace Utils {

	extern HANDLE hConsole;

	extern byte CONSOLE_CYAN;
	extern byte CONSOLE_LIME;
	extern byte CONSOLE_YELLOW;
	extern byte CONSOLE_RED;
	extern byte CONSOLE_PINK;
	extern byte CONSOLE_WHITE;
	extern byte CONSOLE_DEFAULT;

	extern char* CONSOLE_RED_INLINE;
	extern char* COSNOLE_LIME_INLINE;
	extern char* CONSOLE_YELLOW_INLINE;
	extern char* CONSOLE_CYAN_INLINE;
	extern char* CONSOLE_MAGENTA_INLINE;

	wstring GetExecutablePath();

	vector<string> explode(string str, char delimiter);

	int getOccurrences(string base_string, string find);

	string replace(string haystack, string find, string replace);

	string replaceStart(string haystack, string find = " ");

	string replaceEnd(string haystack, string find = " ");

	void Log(char * msg, CONSOLE_TYPE type);

	void padString(std::string &str, const size_t num, const char paddingChar = ' ', bool padRight = true);

	string inlineColor(string str, char* color);
}