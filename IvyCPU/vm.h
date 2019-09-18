#include "instruction.h"
#include "script.h"

class VM {
public:
	VM(std::wstring script_path, bool verbose) {

		Utils::Log(L"Loading script file...", info);

		this->script = new Script(script_path);
		this->verbose = verbose;

		if (!this->script->valid) {
			return;
		}
		Utils::Log(L"VM Instance created successfully.", success);
	}

	bool run() {
		if (!this->script->valid)
			return false;

		Utils::Log(L"Executing script...\n\n", bold);
		for (int i = 0; i < this->script->app.size(); i++) {
			// Instruction * ins = &this->script->app[i];
			if (!this->execute(this->script->app[i]))
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
	vector<string> stack;

	bool execute(Instruction ins) {
		if (ins.type == LOAD_AC) {
			string data = this->GetDataOfAddress(ins.data);
			if (!data.length()) { return false; }
			this->AC = data;
		}
		else if (ins.type == PUSH_AC) {
			stack.push_back(this->AC);
		}
		else if (ins.type == CALL_MESSAGEBOX) {
			if (stack.size() != 2)
			return false;
			MessageBoxA(NULL, stack[1].c_str(), stack[0].c_str(), MB_TOPMOST);
		}
		else if (ins.type == PRINT_AC) {
			printf("AC -> %s\n\n", this->AC.c_str());
		}
		return true;
	}

	string GetDataOfAddress(string addr) {

		for (int i = 0; i < this->script->data.size(); i++) {
			Instruction * ins = &this->script->data[i];
			if (ins->ins == addr) {
				//addr found, return data
				return ins->data;
			}
		}

		return "";

	}
};