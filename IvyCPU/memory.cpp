#include "memory.h"

Block* Memory::getBlockById(int id) {
	for (size_t i = 0; i < blocks.size(); i++) {
		if (blocks[i].getId() == id)
			return (Block*)(&blocks[i]);
	}
	return nullptr;
}

int Memory::getTotalFragmentation() {
	int total = 0;
	for (size_t i = 0; i < blocks.size(); i++) {
		Task* task = blocks[i].getTask();
		if (task == nullptr)
			continue;
		total += blocks[i].getSize() - task->getSize();
	}
	return total;
}

int Memory::getTotalMemory() {
	int total = 0;
	for (size_t i = 0; i < blocks.size(); i++) {
		total += blocks[i].getSize();
	}
	return total;
}