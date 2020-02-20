#pragma once
#include "main.h"
#include "block.h"

using namespace std;

class Memory {
public:
	vector<Block> block;

	Block getBlockById(int id);
	int getTotalFragmentation();
};