#pragma once 
#include <string>

namespace biliroku {
	using std::string;
	string getCurrentTimeString();
	string ReplaceAll(string str, const string& from, const string& to);
	string formatSize(double size);
}