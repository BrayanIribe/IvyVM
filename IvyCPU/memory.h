#pragma once
#include "main.h"
#include "block.h"

using namespace std;

class Memory {
public:
	vector<Block> blocks;

	Block* getBlockById(int id);
	vector<Task*> getWaitingProcesses();
	bool isProcessOnBlocks(int id);
	int  getTotalBlocks();
	int  getTotalFragmentation();
	int  getTotalMemory();
	int  getIdFirstBlockWithTask();
	int  getIdNextBlock(int id);
};