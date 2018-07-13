#include "cmdline.h"
#include "version.h"
#include "Logger.h"
#include <string>
#include <sstream>

#include "LiveDownloader.h"

using std::string;

string getVersion() {
	std::stringstream versionStream;
	versionStream << "Biliroku-lite 1.0.0 (" << BRL_STR(BRL_COMP_NAME) << " " << BRL_COMP_VERSION;
	versionStream << ";" << BRL_STR(BRL_OS) << ") on " << BRL_STR(BRL_ARCH);

	return versionStream.str();
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
        std::cerr << "  ------\nWildcard description:\nYou can use wildcards in the output argument.\n";
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
	
	biliroku::LiveDownloader downloader(cp.get<string>("roomid"), cp.get<string>("output"), isAutoRetry);

	downloader.setLogFunc([](int level, const string &message) {
		static const string logHeaders[] = {
			"[DEBUG]", "[INFO]", "[NOTICE]", "[WARNING]", "[ERROR]", "[FETAL]"
		};

		std::cerr << logHeaders[level] << ' ' << message << std::endl;
	});

	if (cp.exist("proxy")) {
		downloader.setProxy(cp.get<string>("proxy"));
	}

    std::cout << "OK\nROOMID: " << cp.get<string>("roomid") << "\n";
    std::cout << "OUTPUT: " << cp.get<string>("output") << "\n";
    std::cout << "PROXY: " << cp.get<string>("proxy") << "\n";
    std::cout << (cp.exist("autoretry") ? "USE" : "NOT USE") << " Auto Retry" << std::endl;

    return 0;


}