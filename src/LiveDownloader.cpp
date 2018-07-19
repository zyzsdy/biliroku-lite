#include "LiveDownloader.h"
#include "curlTools.h"
#include "ByteBuffer.h"
#include "utils.h"
#include "DownloadMeter.h"

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <sstream>
#include <fstream>
#include <algorithm>

#define CHECK_TIMEOUT_MS 1000
#define CHECK_FAILED_COUNT 3
#define RETRY_TIMEOUT_MS 30000

#ifdef _WIN32
#include <Windows.h>
#define BRL_SLEEP(s) Sleep((s))
#else
#include <unistd.h>
#define BRL_SLEEP(s) usleep(((s) * 1000))
#endif

namespace biliroku {
	size_t curlSaveToFile(void *buffer, size_t size, size_t nmemb, void *userp) {
		std::fstream *savefile = static_cast<std::fstream *>(userp);
		savefile->write(static_cast<char *>(buffer), size*nmemb);
		savefile->flush();
		return size*nmemb;
	}

	int curlProgressCallback(void *userp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
		DownloadMeter *dm = static_cast<DownloadMeter *>(userp);

		if (dm->ce->isStop) {
			dm->log->addLog(BRL_LOG_INFO, "User interrupt.");
			return 2;
		}

		time_t nowTime = time(NULL);
		if (nowTime - dm->lastTime > (CHECK_TIMEOUT_MS / 1000)) { //超过了检查一次的时间
			size_t dataSpan = dlnow - dm->totalByte;
			if (dataSpan <= 0) { //如果在这段时间内没接收到数据
				dm->failedCount += 1;
				std::stringstream logss;
				logss << "No data received during the check: count " << dm->failedCount;
				dm->log->addLog(BRL_LOG_INFO, logss.str());

				if (dm->failedCount >= CHECK_FAILED_COUNT) {
					dm->log->addLog(BRL_LOG_ERROR, "No data was received. Stop.");
					return 1;
				}
			}
			else {
				dm->failedCount = 0;
			}

			dm->speed = dataSpan / (double)(nowTime - dm->lastTime);
			
			dm->lastTime = nowTime;
			dm->totalByte = dlnow;
		}

		std::stringstream statusLog;
		statusLog << "Download: " << formatSize((double)dlnow) << " Speed: " << formatSize(dm->speed) << "/s";
		dm->log->addLog(BRL_LOG_NOTICE, statusLog.str());
		return 0;
	}


	LiveDownloader::LiveDownloader(string _roomid, string _outputPath, bool _autoRetry, ctrlCEvent *_ce)
		: roomid(_roomid), outputPath(_outputPath), isUseProxy(false), isAutoRetry(_autoRetry), streamUrl(""), realRoomid(0), ce(_ce)
	{
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
		realRoomid = rapidjson::Pointer("/data/room_id").GetWithDefault(realRoomidJson, 0).GetInt64();

		std::stringstream infoTemp;
		infoTemp << "Real Roomid: " << realRoomid;

		log->addLog(BRL_LOG_INFO, infoTemp.str());

		if (realRoomid == 0) {
			log->addLog(BRL_LOG_ERROR, "Failed to get the real Roomid.");
			return false;
		}

		return true;
	}

	bool LiveDownloader::getTrueStream()
	{
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
	void LiveDownloader::download()
	{
		do {
			if (getTrueStream()) {


				curl_global_init(CURL_GLOBAL_ALL);
				CURL *curl = curl_easy_init();

				string onceSavepath = ReplaceAll(outputPath, "{roomid}", roomid);
				onceSavepath = ReplaceAll(onceSavepath, "{time}", getCurrentTimeString());

				std::fstream *outputStream = new std::fstream(onceSavepath, std::ios::out | std::ios::binary);
				DownloadMeter *downloadMeter = new DownloadMeter(ce);


				if (curl) {
					curl_easy_setopt(curl, CURLOPT_URL, streamUrl.c_str());
					curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
					curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
					curl_easy_setopt(curl, CURLOPT_TIMEOUT, CURL_TIMEOUT_SECOND);
					curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CURL_CONNTIMEOUT_SEC);
					curl_easy_setopt(curl, CURLOPT_HTTPHEADER, getCurlHeader());
					curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);
					curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlSaveToFile);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputStream);
					curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
					curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, curlProgressCallback);
					curl_easy_setopt(curl, CURLOPT_XFERINFODATA, downloadMeter);
					if (isUseProxy) {
						curl_easy_setopt(curl, CURLOPT_PROXY, proxyString.c_str());
					}
				}
				else {
					log->addLog(BRL_LOG_ERROR, "Error occured while curl handler init.");
				}

				CURLcode res = curl_easy_perform(curl);
				if (res != CURLE_OK) {
					string k = "Error occured while downloading: " + string(curl_easy_strerror(res));
					log->addLog(BRL_LOG_ERROR, k);
				}
				curl_easy_cleanup(curl);
				curl_global_cleanup();
				outputStream->close();
				delete outputStream;
				delete downloadMeter;

			}

			if (ce->isStop) {
				break;
			}

			if (isAutoRetry) {
				std::stringstream tss;
				tss << "Sleep " << RETRY_TIMEOUT_MS << " milliseconds and try again.";
				log->addLog(BRL_LOG_INFO, tss.str());
				BRL_SLEEP(RETRY_TIMEOUT_MS);
			}

			if (ce->isStop) {
				break;
			}

		} while (isAutoRetry);
	}
}