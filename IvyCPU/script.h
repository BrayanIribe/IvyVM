class Script {
public:

	bool valid = false;
	string contents;
	vector<Instruction> app;
	vector<Instruction> data;

	Script(wstring script_path) {
		//process script file provided

		ifstream ifs(script_path);
		string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

		if (content.length() == 0) {
			Utils::Log("File provided is not valid.", error);
			return;
		}

		string app_start_find = ".app\n\n";
		string app_end_find = ".data\n\n";

		size_t app_start_ind = content.find(app_start_find);
		size_t data_start_ind = content.find(app_end_find);

		if (app_start_ind == -1 || data_start_ind == -1) {
			if (app_start_ind == -1)
				Utils::Log("Syntax Error: .app not found.", error);
			if (data_start_ind == -1)
				Utils::Log("Syntax Error: .data not found.", error);
			return;
		}

		string app_raw = content.substr(app_start_find.length(), data_start_ind - app_end_find.length() - 1);
		string data_raw = content.substr(data_start_ind + app_end_find.length());

		//add data and instructions to vector

		vector<string> app, data;

		app = Utils::explode(app_raw, '\n');
		data = Utils::explode(data_raw, '\n');

		//parse app

		for (size_t i = 0; i < app.size(); i++) {
			Instruction ins(app[i]);
			if (!ins.valid && !ins.comment) {
				Utils::Log("Invalid instruction provided in .app.", error);
				return;
			}
			if (ins.comment)
				continue;
			this->app.push_back(ins);
		}

		//parse data

		vector<string> address;

		for (size_t i = 0; i < data.size(); i++) {
			Instruction ins(data[i], true);
			if (!ins.valid && !ins.comment) {
				Utils::Log("Invalid instruction provided in .data.", error);
				return;
			}
			vector<string>::iterator it = find(address.begin(), address.end(), ins.ins);
			if (it != address.end()) {
				char buf[100];
				sprintf_s(buf, 100, "The address %s is already in use.", ins.ins.c_str());
				Utils::Log(buf, error);
				return;
			}
			if (ins.comment)
				continue;
			this->data.push_back(ins);
			address.push_back(ins.ins);
		}

		if (!app.size() || !data.size()){
			Utils::Log("Syntax Error: .app or .data is empty.", error);
			return;
		}

		this->valid = true;
		this->contents = content;
	}
};