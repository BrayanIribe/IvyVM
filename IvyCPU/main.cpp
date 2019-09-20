/*==============================================
IvyVM
Developed by Brayan Iribe.
This software is open-source.
https://www.github.com/BrayanIribe/IvyVM
===============================================*/
#include "main.h"

using namespace std;

int main(int argc, char ** argv) {

	bool verbose = false;
	wstring script_path = Utils::GetExecutablePath();

	for (int i = 0; i < argc; i++) {
		string argument = argv[i];
		if (argument == "--verbose") {
			verbose = true;
		}
		size_t ivyExt = argument.find(".ivy");
		if (ivyExt != -1) {
			size_t isDrive = argument.find(":");
			wstring path(argument.begin(), argument.end());
			if (isDrive != -1) {
				script_path = path;
			}
			else {
				script_path = script_path + L"\\" + path;
			}
		}
	}

	VM * vm = new VM(script_path, verbose);
	vm->run();
	SetConsoleTextAttribute(Utils::hConsole, Utils::CONSOLE_DEFAULT);
	return 1;

}