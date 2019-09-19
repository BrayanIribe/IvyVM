#include "instruction.h"
#include "script.h"

class VM {
public:
	VM(std::wstring script_path, bool verbose) {

		if (verbose)
			Utils::Log("Loading script file...", info);

		this->script = new Script(script_path);
		this->verbose = verbose;

		if (!this->script->valid) {
			Utils::Log("VM failed to load the script.", error);
			return;
		}

		if (verbose)
			Utils::Log("VM Instance created successfully.", success);
	}

	bool run() {
		if (!this->script->valid)
			return false;

		if (verbose)
			Utils::Log("Executing script...\n", bold);
		for (this->PC = 0; this->PC < (int32_t)this->script->app.size(); this->PC++) {
			//CPU cycle
			if (!this->execute(this->script->app[this->PC]))
				return false;
		}

		return true;
	}
private:
	Script * script;
	bool verbose = false;
	bool valid;

	//ACCUMULATORS

	int32_t PC = 0;
	string AC = "";
	VARIABLE_TYPE AC_TYPE;
	vector<string> stack;

	bool execute(Instruction ins) {
		SetConsoleTextAttribute(Utils::hConsole, Utils::CONSOLE_WHITE);
		if (ins.type == LOAD_AC) {
			Instruction * chunk = this->GetData(ins.data);
			if (chunk == nullptr) { return false; }
			this->AC = chunk->data;
			this->AC_TYPE = chunk->variableType;
		}
		else if (ins.type == SAVE_AC) {
			Instruction * chunk = this->GetData(ins.data);
			if (chunk == nullptr) { return false; }
			chunk->data = this->AC;
		}
		else if (ins.type == PUSH_AC) {
			stack.push_back(this->AC);
		}
		else if (ins.type == POP_AC) {
			this->AC = this->pop();
			if (!this->AC.length())
				return false;
		}
		else if (ins.type == PRINT_AC) {
			printf("%s\n", this->AC.c_str());
		}
		else if (ins.type == SUM_AC || ins.type == SUB_AC || ins.type == MUL_AC || ins.type == DIV_AC) {
			Instruction * chunk = this->GetData(ins.data);
			if (chunk == nullptr) { return false; }
			if (!AC.length()) { Utils::Log("AC doesnt have a value.", error); return false; }
			double ac = atof(this->AC.c_str());
			double data = atof(chunk->data.c_str());
			switch (ins.type) {
			case SUM_AC:
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
		}
		else if (ins.type == CALL_PRINT) {

		}
		else if (ins.type == CALL_SPRINTF) {
			int count = Utils::getOccurrences(this->AC, "%pop");

			if (count != stack.size()) {
				Utils::Log("Tried to do sprintf, but stack size is not the same as format.", error);
				return false;
			}

			for (size_t i = 0; i < 1; i++) {
				string pop = this->pop();
				size_t occ = this->AC.find("%pop");
				this->AC.replace(occ, occ + 4, pop);
			}
 
		}
		else if (ins.type == CALL_MESSAGEBOX) {
			if (stack.size() != 2)
				return false;
			MessageBoxA(NULL, this->pop().c_str(), this->pop().c_str(), MB_TOPMOST);
			stack.clear();
		}

		if (verbose) {
			SetConsoleTextAttribute(Utils::hConsole, Utils::CONSOLE_PINK);
			printf("%s -> %-10s| AC=%-50s PC=%-5i\n", ins.ins.c_str(), ins.data.c_str(), this->AC.c_str(), this->PC);
		}
		return true;
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

	Instruction * GetData(string addr) {

		for (int i = 0; i < (int)this->script->data.size(); i++) {
			Instruction * ins = &this->script->data[i];
			if (this->script->data[i].ins == addr) {
				//addr found, return data
				return ins;
			}
		}

		char buf[100];
		sprintf_s(buf, 100, "Invalid address. The address %s doesnt exists in the app.", addr.c_str());
		Utils::Log(buf, error);

		return 0;

	}
};