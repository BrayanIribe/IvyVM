enum CONSOLE_TYPE {
	none,
	info,
	success,
	warn,
	error,
	bold
};

namespace Utils {

	using namespace std;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	byte CONSOLE_CYAN = 11;
	byte CONSOLE_LIME = 10;
	byte CONSOLE_YELLOW = 14;
	byte CONSOLE_RED = 12;
	byte CONSOLE_PINK = 13;
	byte CONSOLE_WHITE = 15;
	byte CONSOLE_DEFAULT = 7;

	wstring GetExecutablePath() {
		HMODULE h = GetModuleHandle(NULL); //self handle
		wchar_t buf[MAX_PATH];
		GetModuleFileName(h, buf, MAX_PATH);
		wstring path(buf);
		size_t found = path.find_last_of('\\');
		path = path.substr(0, found);
		return path;
	}

	vector<string> explode(string str, char delimiter) {
		vector<string> ret;
		istringstream ss(str);
		string ln;
		while (std::getline(ss, ln, delimiter)) {
			ret.push_back(ln);
		}
		return ret;
	}

	int getOccurrences(string base_string, string find) {
		int occurrences = 0;
		string::size_type start = 0;

		while ((start = base_string.find(find, start)) != string::npos) {
			++occurrences;
			start += find.length();
		}
		return occurrences;
	}

	string replace(string haystack, string find, string replace) {
		int count = Utils::getOccurrences(haystack, find);
		for (int i = 0; i < count; i++) {
			size_t ind = haystack.find(find);
			haystack.replace(ind, replace.length(), replace);
		}
		return haystack;
	}

	void Log(char * msg, CONSOLE_TYPE type) {
		char * msg_type = "";
		switch (type) {
			case info:
				SetConsoleTextAttribute(hConsole, CONSOLE_CYAN);
				msg_type = "[INFO] ";
				break;
			case success: 
				SetConsoleTextAttribute(hConsole, CONSOLE_LIME);
				msg_type = "[ OK ] ";
				break;
			case warn:
				SetConsoleTextAttribute(hConsole, CONSOLE_YELLOW);
				msg_type = "[WARN] ";
				break;
			case error:
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				msg_type = "[ !! ] ";
				break;
			case bold:
				SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
				msg_type = "[ ## ] ";
				break;
		default:
			SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
		}
		printf_s("%s%s\n", msg_type, msg);
	}
}