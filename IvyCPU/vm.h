#pragma once
#include "main.h"
#include "instruction.h"
#include "script.h"
#include "memory.h"
#include "block.h"

using namespace std;

class VM {
public:

	VM(std::wstring script_path, bool verbose, bool memoryMode);
	bool run();

private:
	
	Script * script;
	bool verbose = false;
	bool valid = false;
	bool memoryMode = false;

	int32_t PC = 1;                             //Program Counter (address that is being executed at the moment)
	int32_t RP = 1;                             //Real Program counter (total instructions executed)
	string AC = "";                             //Accumulator A, store temporal information
	string BC = "";                             //Accumulator B, used for comparations, accesible only in internal.
	VARIABLE_TYPE AC_TYPE;                      //Accumulator A VARIABLE_TYPE -> STRING, FLOAT, BOOL, NUMBER
	vector<string> stack;                       //Stack, just like ASM, pop, and push.
	// #### CABADA ADDITIONS
	Memory memory;
	vector<Task> tasks;
	int firstOut;

	string pop(bool showError = true);
	Instruction * GetInstruction(string addr, bool warn = true);
	bool execute(Instruction * ins);
	void logInstruction(string ins);
	bool printStack();
	bool doMath(Instruction * chunk, INSTRUCTION_TYPE instruction_type);
	bool WinApi_printf();
	bool WinApi_sprintf();
	bool WinApi_MessageBox();

	// #### CABADA ADDITIONS
	void printMemory();
	void firstFit();
};