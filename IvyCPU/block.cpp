#include "block.h"

Block::Block(int id, int size) {
	Block::id = id;
	Block::size = size;
	Block::task = nullptr;
}

int Block::getId() {
	return id;
}

int Block::getSize() {
	return size;
}

Task* Block::getTask() {
	return task;
}

void Block::setTask(Task* task) {
	Block::task = task;
}