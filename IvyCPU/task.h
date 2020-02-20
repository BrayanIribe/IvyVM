#pragma once
#include "main.h"
#include "script.h"

enum TASK_STATUS {
	STATUS_UNDIRTY,
	STATUS_WAITING,
	STATUS_EXECUTED,
	STATUS_FINISHED
};

class Task {
private:
	int id;                                     //Identifier of the task
	int time;                                   //Time of task
	int size;                                   //Size of task
	Script* script;                             //Script pointer for the task
	TASK_STATUS status;                         //Actual status of the task
public:
	int getId();
	int getTime();
	int getSize();
	Script* getScript();
	TASK_STATUS getStatus();
	bool setTask(int id, int time, int size, Script* script = nullptr);
};