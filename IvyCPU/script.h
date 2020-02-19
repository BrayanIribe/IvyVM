#pragma once
#include "main.h"
#include "instruction.h"

using namespace std;

class Script {
public:

	bool valid = false;
	vector<Instruction> memory;

	Script(wstring script_path);
};