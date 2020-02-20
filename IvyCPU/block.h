#pragma once
#include "main.h"
#include "task.h"

class Block {
private:
	int id;                                     // Id of the block
	int size;                                   // Measured in bytes
	Task* task;                                 // Task pointer
public:

	Block(int id, int size);

	int getId();
	int getSize();
	Task* getTask();
	void setTask(Task* task);
};