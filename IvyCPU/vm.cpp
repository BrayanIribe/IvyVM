#include "vm.h"
#include "utils.h"

VM::VM(wstring script_path, bool verbose, bool memoryMode) {
	VM::verbose = verbose;
	VM::memoryMode = memoryMode;
	if (memoryMode) {

		// create tasks

		int taskTime[] = { 5,4,8,2,2,6,8,10,7,6,5,8,9,10,10,7,3,1,9,3,7,2,8,5,10 };
		int taskSize[] = { 5760,4190,3290,2030,2550,6990,8940,740,3930,6890,6580,3820,9140,420,220,7540,3210,1380,9350,3610,7540,2710,8390,5950,60 };

		if (size(taskTime) != size(taskSize)) {
			Utils::Log("Task size and time doesnt contain the same number of elements.", error);
			return;
		}

		for (int i = 0; i < size(taskTime); i++) {
			//int id, int time, int size
			Task task(tasks.size() + 1, taskTime[i], taskSize[i]);
			tasks.push_back(task);
		}

		// create memory blocks

		int blockSize[] = { 9500, 7000, 4500, 8500, 3000, 9000, 1000, 5500, 1500, 500 };
		for (int i = 0; i < size(blockSize); i++) {
			Block block(memory.blocks.size() + 1, blockSize[i]);
			memory.blocks.push_back(block);
			char buf[100];
			sprintf_s(buf, "Memory block of %d bytes created at %d", blockSize[i], block.getId());
			Utils::Log(buf, warn);
		}

		Utils::Log("Cabada mode...", warn);
		valid = true;
		return;
	}
	Utils::Log("Loading script file...", info);

	script = new Script(script_path);

	if (!script->valid) {
		Utils::Log("VM failed to load the script.", error);
		return;
	}

	//now check every memory address

	vector<string> addresses;
	vector<string>::iterator it;

	for (size_t i = 0; i < script->memory.size(); i++) {
		Instruction * ins = &script->memory[i];
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
	valid = true;
}

bool VM::run() {
	if (!valid)
		return false;

	if (memoryMode) {
		while (true) {
			char buf[4];
			cin >> buf;
			if (strcmp(buf, "exit") == 0) {
				break;
			}
		}
		return true;
	}


	Utils::Log("Executing script...\n", bold);
	//do cpu cycles
	while (true) {
		//find instruction every cycle
		string addr = to_string(PC);
		Utils::padString(addr, 4, '0');
		Instruction * ins = GetInstruction(addr, false);

		if (!ins || ins == nullptr || ins->isData) {
			//program ended
			break;
		}
		if (!execute(ins)) {
			return false;
		}
		PC++;
		RP++;
	}
	printf("\n\n");
	char buffer[100];
	sprintf_s(buffer, 100, "Script execution ended. PC = %i", PC);
	Utils::Log(buffer, warn);
	return true;
}

bool VM::execute(Instruction * ins) {
	if (verbose && ins->type != CALL_GETCH) {
		logInstruction(ins->GetInstructionName());
	}

	Instruction * chunk;
	string input;

	if (ins->requireData && !ins->data.empty() && ins->data.length()) {
		chunk = GetInstruction(ins->data);
		if (chunk == nullptr || !chunk->isData) {
			return false;
		}
	}

	switch (ins->type) {
	case LOAD_AC:
		AC = chunk->data;
		AC_TYPE = chunk->variableType;
		break;
	case SAVE_AC:
		chunk->data = AC;
		break;
	case PUSH_AC:
		stack.push_back(AC);
		break;
	case POP_AC:
		AC = pop();
		if (!AC.length())
			return false;
		break;
	case CLEAN_AC:
		AC = "";
		break;
	case CLEAN_STACK:
		stack.clear();
	case PRINT_STACK:
		return printStack();
		break;
	case ADD_AC:
	case SUB_AC:
	case MUL_AC:
	case DIV_AC:
		return doMath(chunk, ins->type);
		break;
	case CALL_PRINT:
		return WinApi_printf();
		break;
	case CALL_SPRINTF:
		return WinApi_sprintf();
		break;
	case CALL_GETCH:
		std::cin >> input;
		AC = input;
		break;
	case CALL_MESSAGEBOX:
		return WinApi_MessageBox();
	}


	if (verbose && ins->type == CALL_GETCH) {
		logInstruction(ins->GetInstructionName());
	}

	return true;
}

void VM::logInstruction(string ins) {
	SetConsoleTextAttribute(Utils::hConsole, Utils::CONSOLE_PINK);
	printf("%-20s| AC=%-50s PC=%-5i\n", ins.c_str(), AC.c_str(), PC);
	SetConsoleTextAttribute(Utils::hConsole, Utils::CONSOLE_DEFAULT);
}

string VM::pop(bool showError) {
	size_t size = stack.size();
	if (size == 0) {
		if (showError)
			Utils::Log("Tried to pop stack but stack is empty.", error);
		return "";
	}
	// AC = stack[size - 1];
	// now remove item from stack
	string data = stack[size - 1];
	stack.erase(stack.begin() + size - 1);
	return data;
}

Instruction * VM::GetInstruction(string addr, bool warn) {

	for (size_t i = 0; i < script->memory.size(); i++) {
		Instruction * ins = &script->memory[i];
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

bool VM::printStack() {
	Utils::Log("Print stack called.\n", info);
	if (stack.size()) {
		for (size_t i = stack.size() - 1; i >= 0; i--) {
			printf("%i -> %s\n", i, stack[i].c_str());
		}
	}
	printf("\nTotal stack length: %d\n\n", stack.size());
	return true;
}

bool VM::doMath(Instruction * chunk, INSTRUCTION_TYPE instruction_type) {
	if (!AC.length()) { Utils::Log("AC doesnt have a value.", error); return false; }
	double ac = atof(AC.c_str());
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
	AC = newAC.str();
	return true;
}

bool VM::WinApi_printf() {
	if (AC.empty() || !AC.length()) {
		printf("\n");
		return true;
	}
	printf("%s", AC.c_str());
	return true;
}

bool VM::WinApi_sprintf() {
	string format = pop();
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
		string pop = VM::pop();
		format = format.substr(0, occ) + pop + format.substr(occ + 5);
	}

	format = Utils::replace(format, "%ac%", AC);
	format = Utils::replace(format, "\\n", "\n\r");
	AC = format;
	stack.clear();
	return true;
}

bool VM::WinApi_MessageBox() {
	if (stack.size() < 2)
		return false;
	ostringstream newAC;
	UINT uType = MB_SYSTEMMODAL;
	string lpCaption = pop();
	string lpText = pop();
	if (stack.size() > 0) {
		string format = pop();
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
	AC = newAC.str();
	stack.clear();
	return true;
}