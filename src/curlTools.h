#pragma once
#include <string>
#include <curl/curl.h>
#include "ByteBuffer.h"

namespace biliroku {
	#define CURL_TIMEOUT_SECOND 180
	#define CURL_CONNTIMEOUT_SEC 30

	using std::string;

	curl_slist *getCurlHeader();
	size_t readCurlUChar(void * buffer, size_t size, size_t nmemb, void * userp);

	bool CURL_simple_get(string url, ByteBuffer &out, bool isUseProxy, string proxyStr);
	CURL *curl_easy_handler_builder(string url, ByteBuffer &out, curl_slist *headers, bool isUseProxy, string proxyStr);
}