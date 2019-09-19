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
	PUSH_AC,
	POP_AC,
	PRINT_AC,

	SUM_AC,
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
	INSTRUCTION_TYPE type;
	VARIABLE_TYPE variableType;
	string raw, ins, data;
	bool isData = false;

	Instruction(string instruction, bool isData = false) {

		//remove comments
		size_t ind = instruction.find_first_of("//");
		if (ind != -1) {
			instruction = instruction.substr(0, ind - 1);
		}

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
			Utils::Log("Invalid opcode provided.", error);
			return;
		}

		if (data == 0) {
			Utils::Log("Invalid arguments provided.", error);
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
			this->type = PUSH_AC;
			break;
		case 4:
			this->type = POP_AC;
			break;
		case 5:
			this->type = PRINT_AC;
			break;
		case 10:
			this->type = SUM_AC;
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
			break;
		case 51:
			this->type = CALL_SPRINTF;
			break;
		case 52:
			this->type = CALL_GETCH;
			break;
		case 53:
			this->type = CALL_MESSAGEBOX;
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

		size_t ind = this->raw.find_first_of(" ");
		string addr = this->raw.substr(0, ind);
		string data = this->raw.substr(ind + 1);

		if (ind == -1 || !addr.length() || !data.length()) {
			return;
		}

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
		this->valid = true;
	}
};