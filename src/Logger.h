#pragma once
#include <functional>
#include <string>

using std::string;

namespace biliroku {
#ifndef BRL_LOG
#define BRL_LOG
#define BRL_LOG_DEBUG 0
#define BRL_LOG_INFO 1
#define BRL_LOG_NOTICE 2
#define BRL_LOG_WARNING 3
#define BRL_LOG_ERROR 4
#define BRL_LOG_FETAL 5
#endif
	typedef std::function<void(int log_level, const string &message)> LogCallback;

	class Logger
	{
	public:
		static Logger *getInstance();
		void setLogCallback(LogCallback &cb);
		LogCallback addLog;
	private:
		Logger();
		Logger(const Logger&);
		~Logger();

		static Logger *instance;
	};
}

