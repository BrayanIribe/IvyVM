#include "memory.h"

Block* Memory::getBlockById(int id) {
	for (size_t i = 0; i < blocks.size(); i++) {
		if (blocks[i].getId() == id)
			return (Block*)(&blocks[i]);
	}
	return nullptr;
}

int Memory::getTotalBlocks() {
	return blocks.size();
}

bool Memory::isProccessOnBlocks(int id) {
	for (size_t i = 0; i < blocks.size(); i++) {
		Block block = blocks[i];
		if (block.getTask() != nullptr && block.getTask()->getId() == id)
			return true;
	}
	return false;
}

vector<Task*> Memory::getWaitingProcesses() {
	vector<Task*> waiting;
	for (size_t i = 0; i < blocks.size(); i++) {
		Block block = blocks[i];
		Task* task = block.getTask();
		if (task == nullptr || task->getStatus() == TASK_STATUS::STATUS_EXECUTING)
			continue;
		if (task->getStatus() == TASK_STATUS::STATUS_WAITING)
			waiting.push_back(task);
	}
	return waiting;
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