#pragma once
#include <string>

namespace biliroku {
	using std::string;

#define SIZE_BUFFER_INIT_SIZE 1024 //1KB



	struct ByteBuffer {
	private:
		size_t len;
		size_t max_size;
		unsigned char *buffer;
	public:
		ByteBuffer();
		~ByteBuffer();
		string str();
		size_t size();
		size_t append(const unsigned char *buffer, size_t bytelength);
		unsigned char *getBuffer();
	};
}