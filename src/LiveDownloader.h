#pragma once

#include <string>
#include "Logger.h"

using std::string;

namespace biliroku {
	class LiveDownloader {
		string roomid;
		string outputPath;

		string proxyString;
		bool isUseProxy;
		bool isAutoRetry;

		Logger *log;
		string streamUrl;
	public:
		LiveDownloader(string _roomid, string _outputPath, bool _autoRetry);
		~LiveDownloader();
		void setProxy(string _proxyString);
		void setLogFunc(LogCallback logfunc);
		bool init();
	};
}