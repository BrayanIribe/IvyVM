#pragma once
#include "spanish.h"
#include "english.h"

enum Language {
	Spanish,
	English
};

enum MessageType {
	// define TASK_STATUS strings
	TASK_STATUS_UNDIRTY,
	TASK_STATUS_WAITING,
	TASK_STATUS_EXECUTING,
	TASK_STATUS_FINISHED,
	MEMORY_BLOCK_SIZE
};

struct Message {
	MessageType type;
	const char * data;
};

Message GetMessage(MessageType type, Language lang) {
	Message message;
	message.type = type;
	switch (lang) {
	case Spanish:
		message.data = languageSpanish[type].c_str();
		break;
	case English:
		message.data = languageEnglish[type].c_str();
		break;
	}
	return message;
}