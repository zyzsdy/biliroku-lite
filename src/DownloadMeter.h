#pragma once
#include <cstdlib>
#include "Logger.h"

namespace biliroku {
	struct ctrlCEvent {
		bool isStop;
		ctrlCEvent();
	};

	struct DownloadMeter
	{
		size_t totalByte;
		time_t lastTime;
		int failedCount;
		double speed;
		Logger *log;
		ctrlCEvent *ce;
		DownloadMeter(ctrlCEvent *_ce);
		~DownloadMeter();
	};
}

