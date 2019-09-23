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
	INSTRUCTION_TYPE type = INS_UNK;
	VARIABLE_TYPE variableType;
	int line;
	string address, opcode, data, raw;
	bool isData = false;

	Instruction(string raw) {

		//parse the raw instruction
		//remove the spaces from start

		raw = Utils::replaceStart(raw);

		if (raw.substr(0, 2) == "//") {
			this->comment = true;
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
		this->address = instruction.substr(0, 4);
		instruction = Utils::replaceStart(instruction.substr(4));
		
		//search for double quotes

		ind = instruction.find("\"");
		if (ind != -1) {
			this->data = instruction.substr(ind);
			this->TryAsData();
			return;
		}

		if (instruction.find(".") != -1) {
			//float, process it as data.
			this->data = instruction;
			this->TryAsData();
			return;
		}

		this->opcode = instruction.substr(0, 2);
		if (instruction.find(" ") != -1) {
			//replace spaces, at this moment
			//instruction = args
			instruction = Utils::replaceStart(instruction.substr(3));
		}

		byte opcode = atoi(this->opcode.c_str());

		switch (opcode) {
		case 1:
			this->type = LOAD_AC;
			break;
		case 2:
			this->type = SAVE_AC;
			break;
		case 3:
			this->type = PUSH_AC;
			requireData = false;
			break;
		case 4:
			this->type = POP_AC;
			requireData = false;
			break;
		case 5:
			this->type = CLEAN_AC;
			requireData = false;
			break;
		case 6:
			this->type = CLEAN_STACK;
			requireData = false;
			break;
		case 7:
			this->type = PRINT_STACK;
			requireData = false;
			break;
		case 10:
			this->type = ADD_AC;
			break;
		case 11:
			this->type = SUB_AC;
			break;
		case 12:
			this->type = MUL_AC;
			break;
		case 13:
			this->type = DIV_AC;
			break;
		case 50:
			this->type = CALL_PRINT;
			requireData = false;
			break;
		case 51:
			this->type = CALL_SPRINTF;
			requireData = false;
			break;
		case 52:
			this->type = CALL_GETCH;
			requireData = false;
			break;
		case 53:
			this->type = CALL_MESSAGEBOX;
			requireData = false;
			break;
		default:
			this->data = instruction;
			this->TryAsData();
			return;
		}

		if (requireData && !instruction.empty() && instruction.length() > 0) {
			//the instruction needs args, process it.
			//at this moment instruction contains the arguments
			this->data = instruction;
		}

		if (this->type == INS_UNK) {
			//process the instruction as data
			this->TryAsData();
			return;
		}

		// printf("0x%s %s -> %s\n\n", this->address.c_str(), this->opcode.c_str(), this->data.c_str());
		this->valid = true;
	}

	string GetInstructionName() {
		string name = "???";
		string addr = this->data;
		switch (this->type) {
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
private:

	void TryAsData() {

		//determine variable type

		if (data.substr(0, 1) == "\"" && data.substr(data.length() - 1) == "\"") {
			this->variableType = V_STRING;
			data = data.substr(1, data.length() - 2);
			data = Utils::replace(data, "\\n", "\n\r");
		}
		else if (data.find('.') != -1) {
			// is float
			this->variableType = V_FLOAT;
		}
		else if (data == "false" || data == "true") {
			//is bool
			this->variableType = V_BOOL;
		}
		else {
			//is numeric
			this->variableType = V_NUMBER;
		}

		if (this->variableType != V_STRING && data.find(" ") != -1) {
			//the variable is not string and spaces were found
			Utils::Log("Data cannot have multiple arguments.", error);
			return;
		}

		this->isData = true;
		this->valid = true;
		// printf("%s-\n\n", this->data.c_str());
	}
};