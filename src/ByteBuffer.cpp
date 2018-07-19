#include "ByteBuffer.h"
#include <algorithm>
#ifndef _MSC_VER
#include <string.h>
#endif


namespace biliroku {
	//ByteBuffer
	ByteBuffer::ByteBuffer()
	{
		len = 0;
		max_size = SIZE_BUFFER_INIT_SIZE;
		buffer = new unsigned char[SIZE_BUFFER_INIT_SIZE];
	}

	ByteBuffer::~ByteBuffer()
	{
		if (buffer != NULL) delete[] buffer;
	}

	string ByteBuffer::str()
	{
		unsigned char *strend = new unsigned char[1];
		strend[0] = '\0';
		append(strend, 1);
		delete[] strend;
		return string((char *)buffer);
	}

	size_t ByteBuffer::size()
	{
		return len;
	}

	size_t ByteBuffer::append(const unsigned char * buffer, size_t bytelength)
	{
		if (max_size - len < bytelength) {
			//扩容
			size_t newsize = std::max((max_size * 2), (max_size + bytelength));
			auto newbuffer = new unsigned char[newsize];
			//复制旧数据

			#ifdef _MSC_VER
			memcpy_s(newbuffer, newsize, this->buffer, len);
			#else
			memcpy(newbuffer, this->buffer, len);
			#endif
			//删除旧数据
			delete[] this->buffer;
			//重置指针
			this->buffer = newbuffer;
			max_size = newsize;
		}

		#ifdef _MSC_VER
		memcpy_s(this->buffer + len, max_size - len, buffer, bytelength);
		#else
		memcpy(this->buffer, buffer, bytelength);
		#endif
		len += bytelength;

		return bytelength;
	}

	unsigned char * ByteBuffer::getBuffer()
	{
		return buffer;
	}

}