#include "Logger.h"


namespace biliroku {
	Logger *Logger::instance = nullptr;

	Logger * Logger::getInstance()
	{
		if (instance == nullptr) {
			instance = new Logger();
		}

		return instance;
	}
	void Logger::setLogCallback(LogCallback &cb)
	{
		addLog = cb;
	}
	Logger::Logger(){
		addLog = [](int log_level, const string &message) {};
	}
	Logger::Logger(const Logger&) {
	}
	Logger::~Logger(){
	}
}
