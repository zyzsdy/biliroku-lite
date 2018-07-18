#include "DownloadMeter.h"
#include <ctime>


namespace biliroku {
	DownloadMeter::DownloadMeter(ctrlCEvent *_ce) :
		totalByte(0), failedCount(0), speed(0), ce(_ce)
	{
		lastTime = time(NULL);
		log = Logger::getInstance();
	}


	DownloadMeter::~DownloadMeter()
	{
	}
	ctrlCEvent::ctrlCEvent():
		isStop(false)
	{
	}
}