#include "cmdline.h"
#include "version.h"
#include "Logger.h"
#include <string>
#include <sstream>

#include "LiveDownloader.h"
#include "DownloadMeter.h"

#ifdef BRL_WIN32
#include <Windows.h>
#endif // BRL_WIN32

using std::string;
biliroku::ctrlCEvent *ce;

string getVersion() {
	std::stringstream versionStream;
	versionStream << "Biliroku-lite 1.0.0 (" << BRL_STR(BRL_COMP_NAME) << " " << BRL_COMP_VERSION;
	versionStream << ";" << BRL_STR(BRL_OS) << ") on " << BRL_STR(BRL_ARCH);

	return versionStream.str();
}

bool ctrlhandler(DWORD fdwctrltype) {
	switch (fdwctrltype)
	{
	case CTRL_C_EVENT:
		ce->isStop = true;
		std::cerr << "Ctrl-C pressed, waiting for exit." << std::endl;
		return false;
	default:
		return false;
	}
}

int main(int argc, char *argv[]) {
    cmdline::parser cp;

    cp.footer("\nBILIROKU-LITE -- A simple cross platform bilibili live downloader.\n" + getVersion() + "\n");
    cp.add<string>("roomid", 'n', "roomid which download from bilibili live.");
    cp.add<string>("output", 'o', "Set output file path, default to output.flv", false, "biliroku_{roomid}_{time}.flv");
    cp.add<string>("proxy", '\0', "Set proxy server to use. eg. socks5://127.0.0.1:1080", false);
    cp.add("autoretry", '\0', "Auto stop record and restart when live stream down.");
    cp.add("version", 'v', "Show version.");
    cp.add("help", 'h', "Print this message.");

    bool parseCheck = cp.parse(argc, argv);

    if (cp.exist("help")) {
        std::cerr << cp.usage();
        std::cerr << "\nWildcard description:\nYou can use wildcards in the output argument.\n";
        std::cerr << "{roomid} - replaced by room id\n{time} - replaced by a string of the form YYYYMMDDHHMMSS.";
        std::cerr << std::endl;
        return 0;
    }

    if (cp.exist("version")) {
        std::cerr << getVersion() << std::endl;
        return 0;
    }

    if (!parseCheck) {
        std::cerr << cp.error() << "\nExample: biliroku -n 1 -o yihaozhibojian_{time}.flv --proxy socks5://127.0.0.1:1080 --autoretry\nRun \"biliroku -h\" to show full help." << std::endl;
        return 1;
    }

	bool isAutoRetry = false;
	if (cp.exist("autoretry")) isAutoRetry = true;

	ce = new biliroku::ctrlCEvent();
#ifdef BRL_WIN32
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, true)) {
		std::cerr << "Press Ctrl-C to interrupt the downloading." << std::endl;
	}
	else {
		std::cerr << "[WARNING] Could not set control handler." << std::endl;
	}
#else
#error "You should implement a method to catch Ctrl-C event"
#endif // BRL_WIN32
	
	biliroku::LiveDownloader downloader(cp.get<string>("roomid"), cp.get<string>("output"), isAutoRetry, ce);

	downloader.setLogFunc([](int level, const string &message) {
		static string lastNotice = "";

		static const string logHeaders[] = {
			"[DEBUG]", "[INFO]", "[NOTICE]", "[WARNING]", "[ERROR]", "[FETAL]"
		};

		if (level != BRL_LOG_NOTICE) {
			std::cerr << logHeaders[level] << ' ' << message << std::endl;
			if (lastNotice.length() != 0) {
				std::cerr << lastNotice << '\r' << std::flush;
			}
		}
		else {
			std::cerr << message << '\r' << std::flush;
			lastNotice = message;
		}
		
	});

	if (cp.exist("proxy")) {
		downloader.setProxy(cp.get<string>("proxy"));
	}

	if (!downloader.init()) {
		return 2;
	}

	downloader.download();

	std::cerr << "Finished! Exit." << std::endl;

    return 0;
}