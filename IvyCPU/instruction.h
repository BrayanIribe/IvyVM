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
	LOAD_AC,
	SAVE_AC,
	SUM_AC,
	SUB_AC,
	MUL_AC,
	DIV_AC,
	PUSH_AC,
	CALL_MESSAGEBOX,
	PRINT_AC
};

class Instruction {
public:

	bool valid = false;
	INSTRUCTION_TYPE type;
	VARIABLE_TYPE variableType;
	string raw, ins, data;
	bool isData = false;

	Instruction(string instruction, bool isData = false) {
		this->raw = instruction;

		if (isData) {
			this->constructData();
			return;
		}

		string code = instruction.substr(0, 2);
		string args = instruction.substr(instruction.find(' ') + 1);
		byte opcode = atoi(code.c_str());
		int data = atoi(args.c_str());

		if (opcode == 0) {
			Utils::Log(L"Invalid opcode provided.", error);
			return;
		}

		if (data == 0) {
			Utils::Log(L"Invalid arguments provided.", error);
			return;
		}

		switch (opcode) {
		case 1:
			this->type = LOAD_AC;
			break;
		case 2:
			this->type = SAVE_AC;
			break;
		case 3:
			this->type = SUM_AC;
			break;
		case 4:
			this->type = SUB_AC;
			break;
		case 5:
			this->type = MUL_AC;
			break;
		case 6:
			this->type = DIV_AC;
			break;
		case 7:
			this->type = PUSH_AC;
			break;
		case 8:
			this->type = CALL_MESSAGEBOX;
			break;
		case 9:
			this->type = PRINT_AC;
			break;
		default:
			this->valid = false;
			return;
		}

		// printf("%i -> %i\n\n", opcode, this->type);

		this->ins = code;
		this->data = args;
		this->valid = true;
	}
private:

	void constructData() {
		this->isData = true;
		this->valid = true;

		string keyword = " > ";
		size_t ind = this->raw.find(keyword);
		string addr = this->raw.substr(0, ind);
		string data = this->raw.substr(ind + keyword.length());

		//determine variable type

		if (data.substr(0, 1) == "\"" && data.substr(data.length() - 1) == "\"") {
			this->variableType = V_STRING;
			data = data.substr(1, data.length() - 2);
		}
		else if (data.find('.') != -1) {
			// is float
			this->variableType = V_FLOAT;
		}
		else if (data == "false" || data == "true") {
			//is bool
			this->variableType = V_BOOL;
		} else {
			//is numeric
			this->variableType = V_NUMBER;
		}

		this->ins = addr;
		this->data = data;
	}
};