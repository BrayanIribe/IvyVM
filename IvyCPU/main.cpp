/*==============================================
IvyCPU
Developed by Brayan Iribe.
This software is open-source.
https://www.github.com/BrayanIribe/IvyCPU
===============================================*/
#include "main.h"

using namespace std;

int main(int argc, char * argv) {

	bool verbose = true;
	wstring script_path = Utils::GetExecutablePath() + L"\\hello-world.ivy";

	VM * vm = new VM(script_path, verbose);
	vm->run();
	getchar();
	return 1;

}