class Script {
public:

	bool valid = false;
	vector<Instruction> memory;

	Script(wstring script_path) {
		//process script file provided

		ifstream ifs(script_path);
		string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

		if (content.length() == 0) {
			Utils::Log("File provided is not valid.", error);
			return;
		}

		//process as raw

		vector<string> instruction;
		vector<string> raw = Utils::explode(content, '\n');

		//parse script

		for (size_t i = 0; i < raw.size(); i++) {
			
			if (!raw[i].length()) {
				continue;
			}

			Instruction ins(raw[i]);

			if (!ins.valid && !ins.comment) {
				char buf[200];
				sprintf_s(buf, 200, "Invalid instruction provided in line %i.", i + 1);
				Utils::Log(buf, error);
				return;
			}

			if (ins.comment)
				continue;

			ins.line = i + 1;

			this->memory.push_back(ins);
		}

		if (!memory.size()) {
			Utils::Log("The script doesnt have any instructions to execute.", error);
			return;
		}

		this->valid = true;
	}
};