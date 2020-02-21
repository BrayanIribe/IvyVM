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

string Task::getStatusString() {
	string rtn;
	switch (status) {
	case STATUS_UNDIRTY: 
		rtn = "";
		break;
	case STATUS_WAITING:
		rtn = "Esperando";
		break;
	case STATUS_EXECUTING:
		rtn = "Ejecutando";
		break;
	case STATUS_FINISHED:
		rtn = "Finalizado";
	}
	return rtn;
}

void Task::setStatus(TASK_STATUS status) {
	Task::status = status;
}