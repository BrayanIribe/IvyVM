#include "task.h"

Task::Task(int id, int time, int size) {
	Task::id = id;
	Task::time = time;
	Task::size = size;
	Task::status = TASK_STATUS::STATUS_UNDIRTY;
}

int Task::getId() {
	return id;
}

int Task::getTime() {
	return time;
}

int Task::getSize() {
	return size;
}

Script* Task::getScript() {
	return script;
}

TASK_STATUS Task::getStatus() {
	return status;
}

bool Task::setTask(int id, int time, int size, Script* script){
	return true;
}