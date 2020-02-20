#include "block.h"

int Block::getId() {
	return id;
}

int Block::getSize() {
	return size;
}

Task* Block::getTask() {
	return task;
}