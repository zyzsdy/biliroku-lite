#include "LiveDownloader.h"

namespace biliroku {
	LiveDownloader::LiveDownloader(string _roomid, string _outputPath, bool _autoRetry)
		: roomid(_roomid), outputPath(_outputPath), isUseProxy(false), isAutoRetry(_autoRetry) {
		log = Logger::getInstance();
	}
	LiveDownloader::~LiveDownloader() { }
	void LiveDownloader::setProxy(string _proxyString) {
		proxyString = _proxyString;
		isUseProxy = true;
	}
	void LiveDownloader::setLogFunc(LogCallback logfunc)
	{
		log->setLogCallback(logfunc);
	}
}