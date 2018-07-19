#include "utils.h"
#include <ctime>
#include <sstream>
#include <iomanip>

namespace biliroku {
	string getCurrentTimeString()
	{
		time_t t = time(NULL);
		char ch[64] = { 0 };
		
		#ifdef _MSC_VER
		tm timeResult;
		localtime_s(&timeResult, &t);
		strftime(ch, sizeof(ch) - 1, "%Y%m%d%H%M%S", &timeResult);
		#else
		tm *timeResult = localtime(&t);
		strftime(ch, sizeof(ch) - 1, "%Y%m%d%H%M%S", timeResult);
		#endif
		
		return string(ch);
	}

	string ReplaceAll(string str, const string& from, const string& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}
	string formatSize(double size)
	{
		std::stringstream ss;
		ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);
		if (size <= 1024) {
			ss << size / 1.0 << "B";
		}
		else if (size <= 1048576) {
			ss << size / 1024.0 << "KB";
		}
		else if (size <= 1073741824) {
			ss << size / 1048576.0 << "MB";
		}
		else if (size <= 1099511627776) {
			ss << size / 1073741824.0 << "GB";
		}
		else {
			ss << size / 1099511627776.0 << "TB";
		}

		return ss.str();
	}
}
