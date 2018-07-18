#pragma once

#include <string>
#include "Logger.h"
#include "DownloadMeter.h"

using std::string;

namespace biliroku {
	class LiveDownloader {
		string roomid;
		string outputPath;

		string proxyString;
		bool isUseProxy;
		bool isAutoRetry;

		Logger *log;
		long long realRoomid;
		string streamUrl;
		ctrlCEvent *ce;
	public:
		LiveDownloader(string _roomid, string _outputPath, bool _autoRetry, ctrlCEvent *_ce);
		~LiveDownloader();
		void setProxy(string _proxyString);
		void setLogFunc(LogCallback logfunc);
		bool init();
		bool getTrueStream();
		void download();
	};
}