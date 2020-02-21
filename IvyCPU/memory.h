#pragma once
#include "main.h"
#include "block.h"

using namespace std;

class Memory {
public:
	vector<Block> blocks;

	Block* getBlockById(int id);
	int getTotalFragmentation();
	int getTotalMemory();
};