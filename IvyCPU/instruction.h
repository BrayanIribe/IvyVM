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
	bool comment = false;
	INSTRUCTION_TYPE type;
	VARIABLE_TYPE variableType;
	string raw, ins, data;
	bool isData = false;

	Instruction(string instruction, bool isData = false) {

		this->raw = instruction;

		string rawOpcode, rawArguments;

		if (!this->raw.length() || (this->raw.length() && this->raw.find("//") < 4)) {
			this->comment = true;
			return;
		}

		if (isData)
			rawOpcode = instruction.substr(0, 4);
		else
			rawOpcode = instruction.substr(0, 2);

		if (!rawOpcode.length()) {
			return;
		}

		if (isData)
			rawArguments = instruction.substr(5);
		else if (instruction.length() > 2)
			rawArguments = instruction.substr(3);

		size_t quoteInd = rawArguments.find("\"");
		if (quoteInd != -1) {
			rawArguments = rawArguments.substr(quoteInd + 1);
			size_t lastQuoteInd = rawArguments.find("\"");
			if (lastQuoteInd == -1)
				return;
			rawArguments = rawArguments.substr(0, rawArguments.length() - 1);
		}
		else {
			//is number or boolean
			size_t commentInd = rawArguments.find(" //");
			rawArguments = rawArguments.substr(0, commentInd);
			//now remove spaces
			size_t lastSpace = rawArguments.find_last_of(' ');
			rawArguments = rawArguments.substr(lastSpace + 1);
		}

		this->ins = rawOpcode;
		this->data = rawArguments;

		if (isData) {

			//opcode = address
			//arguments = data
			this->constructData();
			return;

		}

		byte opcode = atoi(rawOpcode.c_str());
		int data = atoi(rawArguments.c_str());

		if (opcode == 0) {
			Utils::Log("Invalid opcode provided.", error);
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

		this->ins = rawOpcode;
		this->data = rawArguments;
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
		case SUM_AC:  name = "SUM       AC, " + addr; break;
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

	void constructData() {
		this->isData = true;

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
		}
		else {
			//is numeric
			this->variableType = V_NUMBER;
		}
		this->valid = true;
	}
};