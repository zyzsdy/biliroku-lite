#include "curlTools.h"
#include "Logger.h"
#include <sstream>


namespace biliroku {

	//CURL_EASY_HANDLE的内部回调，用来将响应数据写入内存。
	size_t readCurlUChar(void * buffer, size_t size, size_t nmemb, void * userp) {
		ByteBuffer *outBuffer = static_cast<ByteBuffer *>(userp);

		outBuffer->append((unsigned char *)buffer, size * nmemb);
		return size * nmemb;
	}

	//设置CURL使用的Header
	curl_slist *getCurlHeader() {
		curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
		headers = curl_slist_append(headers, "Accept-Language: ja;q=0.9,zh-CN,zh;q=0.8,en;q=0.7");
		headers = curl_slist_append(headers, "Cache-Control: max-age=0");
		return headers;
	}

	//使用GET读取，这个函数会同步阻塞，当其返回时，响应内容已经全部装入out中（不含Header）
	void CURL_simple_get(string url, ByteBuffer &out, bool isUseProxy = false, string proxyStr = "") {
		curl_global_init(CURL_GLOBAL_ALL);

		

		CURL *curl = curl_easy_handler_builder(url, out, getCurlHeader(), isUseProxy, proxyStr);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			string k = "Error occured while downloading " + url + ": " + string(curl_easy_strerror(res));
			throw std::exception(k.c_str());
		}
		curl_easy_cleanup(curl);

		curl_global_cleanup();
	}

	CURL *curl_easy_handler_builder(string url, ByteBuffer &out, curl_slist *headers, bool isUseProxy, string proxyStr) {
		CURL *curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, CURL_TIMEOUT_SECOND);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CURL_CONNTIMEOUT_SEC);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readCurlUChar);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
			if (isUseProxy) {
				curl_easy_setopt(curl, CURLOPT_PROXY, proxyStr.c_str());
			}
		}
		else {
			throw std::exception("Error occured while curl handler init.");
		}

		return curl;
	}
}