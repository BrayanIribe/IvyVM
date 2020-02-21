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

int Memory::getIdFirstBlockWithTask() {
	for (size_t i = 0; i < blocks.size(); i++) {
		if (blocks[i].getTask() != nullptr)
			return i + 1;
	}
	return 1;
}

int Memory::getIdNextBlock(int pos) {
	int id = pos - 1;
	for (size_t i = pos - 1; i < blocks.size(); i++) {
		if (blocks[i].getTask() != nullptr) {
			if (i > id)
			id = i + 1;
		}
	}

	if (id == pos - 1)
		id = Memory::getIdFirstBlockWithTask();

	return id;
}

bool Memory::isProcessOnBlocks(int id) {
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