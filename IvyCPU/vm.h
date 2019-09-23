#include "instruction.h"
#include "script.h"

class VM {
public:
	VM(std::wstring script_path, bool verbose) {
		Utils::Log("Loading script file...", info);

		this->script = new Script(script_path);
		this->verbose = verbose;

		if (!this->script->valid) {
			Utils::Log("VM failed to load the script.", error);
			return;
		}

		//now check every memory address

		vector<string> addresses;
		vector<string>::iterator it;

		for (size_t i = 0; i < this->script->memory.size(); i++) {
			Instruction * ins = &this->script->memory[i];
			it = std::find(addresses.begin(), addresses.end(), ins->address);
			if (it != addresses.end()) {
				char buffer[100];
				sprintf_s(buffer, 100, "Address %s provided in line %i is already in use.", ins->address.c_str(), ins->line);
				Utils::Log(buffer, error);
				return;
			}
			addresses.push_back(ins->address);
		}

		Utils::Log("VM Instance created successfully.", success);
		this->valid = true;
	}

	bool run() {
		if (!this->valid)
			return false;

		Utils::Log("Executing script...\n", bold);
		//do cpu cycles
		while (true) {
			//find instruction every cycle
			string addr = to_string(this->PC);
			Utils::padString(addr, 4, '0');
			Instruction * ins = this->GetInstruction(addr, false);

			if (!ins || ins == nullptr || ins->isData) {
				//program ended
				break;
			}
			if (!this->execute(ins)) {
				return false;
			}
			this->PC++;
			this->RP++;
		}
		printf("\n\n");
		char buffer[100];
		sprintf_s(buffer, 100, "Script execution ended. PC = %i", this->PC);
		Utils::Log(buffer, warn);
		return true;
	}
private:
	Script * script;
	bool verbose = false;
	bool valid = false;

	int32_t PC = 1; //Program Counter (address that is being executed at the moment)
	int32_t RP = 1; //Real Program counter (total instructions executed)
	string AC = ""; //Accumulator A, store temporal information
	string BC = ""; //Accumulator B, used for comparations, accesible only in internal.
	VARIABLE_TYPE AC_TYPE; //Accumulator A VARIABLE_TYPE -> STRING, FLOAT, BOOL, NUMBER
	vector<string> stack; //Stack, just like ASM, pop, and push.

	bool execute(Instruction * ins) {
		if (verbose && ins->type != CALL_GETCH) {
			logInstruction(ins->GetInstructionName());
		}

		Instruction * chunk;
		string input;

		if (ins->requireData && !ins->data.empty() && ins->data.length()) {
			chunk = this->GetInstruction(ins->data);
			if (chunk == nullptr || !chunk->isData) { 
				return false;
			}
		}

		switch (ins->type) {
		case LOAD_AC:
			this->AC = chunk->data;
			this->AC_TYPE = chunk->variableType;
			break;
		case SAVE_AC:
			chunk->data = this->AC;
			break;
		case PUSH_AC:
			stack.push_back(this->AC);
			break;
		case POP_AC:
			this->AC = this->pop();
			if (!this->AC.length())
				return false;
			break;
		case CLEAN_AC:
			this->AC = "";
			break;
		case CLEAN_STACK:
			stack.clear();
		case PRINT_STACK:
			return this->printStack();
			break;
		case ADD_AC:
		case SUB_AC:
		case MUL_AC:
		case DIV_AC:
			return this->doMath(chunk, ins->type);
			break;
		case CALL_PRINT:
			return this->WinApi_printf();
			break;
		case CALL_SPRINTF:
			return this->WinApi_sprintf();
			break;
		case CALL_GETCH:
			std::cin >> input;
			this->AC = input;
			break;
		case CALL_MESSAGEBOX:
			return this->WinApi_MessageBox();
		}
			
			
		if (verbose && ins->type == CALL_GETCH) {
			logInstruction(ins->GetInstructionName());
		}

		return true;
	}

	void logInstruction(string ins) {
		SetConsoleTextAttribute(Utils::hConsole, Utils::CONSOLE_PINK);
		printf("%-20s| AC=%-50s PC=%-5i\n", ins.c_str(), this->AC.c_str(), this->PC);
		SetConsoleTextAttribute(Utils::hConsole, Utils::CONSOLE_DEFAULT);
	}

	string pop(bool showError = true) {
		int size = stack.size();
		if (size == 0) {
			if (showError)
				Utils::Log("Tried to pop stack but stack is empty.", error);
			return "";
		}
		// this->AC = stack[size - 1];
		// now remove item from stack
		string data = stack[size - 1];
		stack.erase(stack.begin() + size - 1);
		return data;
	}

	Instruction * GetInstruction(string addr, bool warn = true) {

		for (size_t i = 0; i < this->script->memory.size(); i++) {
			Instruction * ins = &this->script->memory[i];
			if (ins->address == addr) {
				//addr found, return data
				return ins;
			}
		}

		if (!warn) {
			return 0;
		}

		char buf[100];
		sprintf_s(buf, 100, "Invalid address. The address %s doesnt exists in the app.", addr.c_str());
		Utils::Log(buf, error);

		return 0;

	}

	bool printStack() {
		Utils::Log("Print stack called.\n", info);
		if (this->stack.size()) {
			for (int i = this->stack.size() - 1; i >= 0; i--) {
				printf("%i -> %s\n", i, this->stack[i].c_str());
			}
		}
		printf("\nTotal stack length: %i\n\n", this->stack.size());
		return true;
	}

	bool doMath(Instruction * chunk, INSTRUCTION_TYPE instruction_type) {
		if (!AC.length()) { Utils::Log("AC doesnt have a value.", error); return false; }
		double ac = atof(this->AC.c_str());
		double data = atof(chunk->data.c_str());
		switch (instruction_type) {
		case ADD_AC:
			ac += data;
			break;
		case SUB_AC:
			ac -= data;
			break;
		case MUL_AC:
			ac *= data;
			break;
		case DIV_AC:
			ac /= data;
		}
		ostringstream newAC;
		newAC << ac;
		this->AC = newAC.str();
		return true;
	}

	bool WinApi_printf() {
		if (this->AC.empty() || !this->AC.length()) {
			printf("\n");
			return true;
		}
		printf("%s", this->AC.c_str());
		return true;
	}

	bool WinApi_sprintf() {
		string format = this->pop();
		int count = Utils::getOccurrences(format, "%pop%");
		if (count != stack.size()) {
			printf("Length: %d\n\n%s\n\n", stack.size(), format.c_str());
			Utils::Log("Tried to do sprintf, but stack size is not the same as format.", error);
			return false;
		}

		for (size_t i = 0; i < (size_t)count; i++) {
			size_t occ = format.find("%pop%");
			if (occ == -1) {
				break;
			}
			string pop = this->pop();
			format = format.substr(0, occ) + pop + format.substr(occ + 5);
		}

		format = Utils::replace(format, "%ac%", this->AC);
		format = Utils::replace(format, "\\n", "\n\r");
		this->AC = format;
		stack.clear();
		return true;
	}

	bool WinApi_MessageBox() {
		if (stack.size() < 2)
			return false;
		ostringstream newAC;
		UINT uType = MB_SYSTEMMODAL;
		string lpCaption = this->pop();
		string lpText = this->pop();
		if (stack.size() > 0) {
			string format = this->pop();
			vector<string> type = Utils::explode(format, '|');
			if (type.size())
				for (size_t i = 0; i < type.size(); i++) {
					string arg = type[i];
					if (arg == "MB_ABORTRETRYIGNORE")
						uType = uType | MB_ABORTRETRYIGNORE;
					else if (arg == "MB_CANCELTRYCONTINUE")
						uType = uType | MB_CANCELTRYCONTINUE;
					else if (arg == "MB_HELP")
						uType = uType | MB_HELP;
					else if (arg == "MB_OK")
						uType = uType | MB_OK;
					else if (arg == "MB_OKCANCEL")
						uType = uType | MB_OKCANCEL;
					else if (arg == "MB_RETRYCANCEL")
						uType = uType | MB_RETRYCANCEL;
					else if (arg == "MB_YESNO")
						uType = uType | MB_YESNO;
					else if (arg == "MB_YESNOCANCEL")
						uType = uType | MB_YESNOCANCEL;
					else if (arg == "MB_ICONEXCLAMATION")
						uType = uType | MB_ICONEXCLAMATION;
					else if (arg == "MB_ICONWARNING")
						uType = uType | MB_ICONWARNING;
					else if (arg == "MB_ICONINFORMATION")
						uType = uType | MB_ICONINFORMATION;
					else if (arg == "MB_ICONASTERISK")
						uType = uType | MB_ICONASTERISK;
					else if (arg == "MB_ICONQUESTION")
						uType = uType | MB_ICONQUESTION;
					else if (arg == "MB_ICONSTOP")
						uType = uType | MB_ICONSTOP;
					else if (arg == "MB_ICONERROR")
						uType = uType | MB_ICONERROR;
					else if (arg == "MB_ICONHAND")
						uType = uType | MB_ICONHAND;
					else if (arg == "MB_DEFBUTTON1")
						uType = uType | MB_DEFBUTTON1;
					else if (arg == "MB_DEFBUTTON2")
						uType = uType | MB_DEFBUTTON2;
					else if (arg == "MB_DEFBUTTON3")
						uType = uType | MB_DEFBUTTON3;
					else if (arg == "MB_DEFBUTTON4")
						uType = uType | MB_DEFBUTTON4;
				}
		}
		int rtn = MessageBoxA(NULL, lpText.c_str(), lpCaption.c_str(), uType);
		newAC << rtn;
		this->AC = newAC.str();
		stack.clear();
		return true;
	}
};