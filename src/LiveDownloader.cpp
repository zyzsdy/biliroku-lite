#include "LiveDownloader.h"
#include "curlTools.h"
#include "ByteBuffer.h"

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <sstream>

namespace biliroku {
	LiveDownloader::LiveDownloader(string _roomid, string _outputPath, bool _autoRetry)
		: roomid(_roomid), outputPath(_outputPath), isUseProxy(false), isAutoRetry(_autoRetry), streamUrl("") {
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
	bool LiveDownloader::init()
	{
		log->addLog(BRL_LOG_INFO, "Resolve real room number...");
		//解析真实房间号
		string roomWebpageUrl = "https://api.live.bilibili.com/room/v1/Room/room_init?id=" + roomid;

		ByteBuffer realRoomidResult;

		try {
			CURL_simple_get(roomWebpageUrl, realRoomidResult, isUseProxy, proxyString);
		}
		catch (std::exception e) {
			log->addLog(BRL_LOG_ERROR, e.what());
			return false;
		}

		rapidjson::Document realRoomidJson;
		realRoomidJson.Parse(reinterpret_cast<char *>(realRoomidResult.getBuffer()), realRoomidResult.size());
		long long realRoomid = rapidjson::Pointer("/data/room_id").GetWithDefault(realRoomidJson, 0).GetInt64();

		std::stringstream infoTemp;
		infoTemp << "Real Roomid: " << realRoomid;

		log->addLog(BRL_LOG_INFO, infoTemp.str());

		if (realRoomid == 0) {
			log->addLog(BRL_LOG_ERROR, "Failed to get the real Roomid.");
			return false;
		}

		//解析真实下载地址
		std::stringstream apiUrl;
		apiUrl << "https://api.live.bilibili.com/api/playurl?cid=" << realRoomid << "&otype=json&quality=0&platform=web";

		ByteBuffer streamUrlResult;

		try {
			CURL_simple_get(apiUrl.str(), streamUrlResult, isUseProxy, proxyString);
		}
		catch (std::exception e) {
			log->addLog(BRL_LOG_ERROR, e.what());
			return false;
		}

		rapidjson::Document streamJson;
		streamJson.Parse(reinterpret_cast<char *>(streamUrlResult.getBuffer()), streamUrlResult.size());
		streamUrl = rapidjson::Pointer("/durl/0/url").GetWithDefault(streamJson, "").GetString();

		if (streamUrl.length() == 0) {
			log->addLog(BRL_LOG_ERROR, "Failed to resolve real stream url.");
			return false;
		}

		log->addLog(BRL_LOG_INFO, "Real stream url: " + streamUrl);
		return true;
	}
}