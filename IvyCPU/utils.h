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

	void Log(wstring msg, CONSOLE_TYPE type) {
		wchar_t buf[500];
		switch (type) {
			case info:
				SetConsoleTextAttribute(hConsole, CONSOLE_CYAN);
				msg = L"[INFO] " + msg;
				break;
			case success: 
				SetConsoleTextAttribute(hConsole, CONSOLE_LIME);
				msg = L"[ OK ] " + msg;
				break;
			case warn:
				SetConsoleTextAttribute(hConsole, CONSOLE_YELLOW);
				msg = L"[WARN] " + msg;
				break;
			case error:
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				msg = L"[ !! ] " + msg;
				break;
			case bold:
				SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
				msg = L"[ ## ] " + msg;
				break;
		default:
			SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
		}
		wsprintf(buf, msg.c_str(), "%s");
		wprintf(L"%s\n", buf);
	}
}