#pragma once
#include "main.h"

using namespace std;

/*
	Piece of instruction in memory
*/

enum VARIABLE_TYPE {
	V_NUMBER,
	V_BOOL,
	V_FLOAT,
	V_STRING
};

enum INSTRUCTION_TYPE {
	INS_UNK,
	LOAD_AC,
	SAVE_AC,
	PUSH_AC,
	POP_AC,
	CLEAN_AC,
	CLEAN_STACK,
	PRINT_STACK,

	ADD_AC,
	SUB_AC,
	MUL_AC,
	DIV_AC,

	CALL_PRINT,
	CALL_SPRINTF,
	CALL_GETCH,
	CALL_MESSAGEBOX,
};

class Instruction {
public:

	bool valid = false;
	bool comment = false;
	bool requireData = true;
	INSTRUCTION_TYPE type;
	VARIABLE_TYPE variableType;
	int line;
	string address, opcode, data, raw;
	bool isData = false;

	Instruction(string raw);
	string GetInstructionName();

private:

	void TryAsData();

};