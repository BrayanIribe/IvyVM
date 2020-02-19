#include "instruction.h"
#include "utils.h"

Instruction::Instruction(string raw) {

	//parse the raw instruction
	//remove the spaces from start

	raw = Utils::replaceStart(raw);

	if (raw.substr(0, 2) == "//") {
		comment = true;
		return;
	}

	size_t ind;
	string instruction = raw;

	//remove inline comments

	ind = instruction.find("//");
	if (ind != -1) {
		instruction = instruction.substr(0, ind);
	}

	instruction = Utils::replaceEnd(instruction);

	//find spaces before address
	address = instruction.substr(0, 4);
	instruction = Utils::replaceStart(instruction.substr(4));

	//search for double quotes

	ind = instruction.find("\"");
	if (ind != -1) {
		data = instruction.substr(ind);
		TryAsData();
		return;
	}

	if (instruction.find(".") != -1) {
		//float, process it as data.
		data = instruction;
		TryAsData();
		return;
	}

	opcode = instruction.substr(0, 2);
	if (instruction.find(" ") != -1) {
		//replace spaces, at this moment
		//instruction = args
		instruction = Utils::replaceStart(instruction.substr(3));
	}

	byte opcode = atoi(Instruction::opcode.c_str());

	switch (opcode) {
	case 1:
		type = LOAD_AC;
		break;
	case 2:
		type = SAVE_AC;
		break;
	case 3:
		type = PUSH_AC;
		requireData = false;
		break;
	case 4:
		type = POP_AC;
		requireData = false;
		break;
	case 5:
		type = CLEAN_AC;
		requireData = false;
		break;
	case 6:
		type = CLEAN_STACK;
		requireData = false;
		break;
	case 7:
		type = PRINT_STACK;
		requireData = false;
		break;
	case 10:
		type = ADD_AC;
		break;
	case 11:
		type = SUB_AC;
		break;
	case 12:
		type = MUL_AC;
		break;
	case 13:
		type = DIV_AC;
		break;
	case 50:
		type = CALL_PRINT;
		requireData = false;
		break;
	case 51:
		type = CALL_SPRINTF;
		requireData = false;
		break;
	case 52:
		type = CALL_GETCH;
		requireData = false;
		break;
	case 53:
		type = CALL_MESSAGEBOX;
		requireData = false;
		break;
	default:
		data = instruction;
		TryAsData();
		return;
	}

	if (requireData && !instruction.empty() && instruction.length() > 0) {
		//the instruction needs args, process it.
		//at this moment instruction contains the arguments
		data = instruction;
	}

	if (type == INS_UNK) {
		//process the instruction as data
		TryAsData();
		return;
	}

	// printf("0x%s %s -> %s\n\n", address.c_str(), opcode.c_str(), data.c_str());
	valid = true;
}

string Instruction::GetInstructionName() {
	string name = "???";
	string addr = data;
	switch (type) {
	case LOAD_AC: name = "MOV       AC, " + addr; break;
	case SAVE_AC: name = "MOV       " + addr + ", AC"; break;
	case PUSH_AC: name = "PUSH      AC"; break;
	case POP_AC:  name = "POP       AC"; break;
	case CLEAN_AC: name = "XOR      AC, AC"; break;
	case CLEAN_STACK: name = "CALL      ClearStack"; break;
	case PRINT_STACK: name = "CALL      PrintStack"; break;
	case ADD_AC:  name = "ADD       AC, " + addr; break;
	case SUB_AC:  name = "SUB       AC, " + addr; break;
	case MUL_AC:  name = "MUL       AC, " + addr; break;
	case DIV_AC:  name = "DIV       AC, " + addr; break;
	case CALL_PRINT: name = "CALL      print"; break;
	case CALL_SPRINTF: name = "CALL      sprintf"; break;
	case CALL_GETCH: name = "CALL      getch"; break;
	case CALL_MESSAGEBOX: name = "CALL      MessageBox"; break;
	}
	return name;
}

void Instruction::TryAsData() {

	//determine variable type

	if (data.substr(0, 1) == "\"" && data.substr(data.length() - 1) == "\"") {
		variableType = V_STRING;
		data = data.substr(1, data.length() - 2);
		data = Utils::replace(data, "\\n", "\n\r");
	}
	else if (data.find('.') != -1) {
		// is float
		variableType = V_FLOAT;
	}
	else if (data == "false" || data == "true") {
		//is bool
		variableType = V_BOOL;
	}
	else {
		//is numeric
		variableType = V_NUMBER;
	}

	if (variableType != V_STRING && data.find(" ") != -1) {
		//the variable is not string and spaces were found
		Utils::Log("Data cannot have multiple arguments.", error);
		return;
	}

	isData = true;
	valid = true;
	// printf("%s-\n\n", data.c_str());
}