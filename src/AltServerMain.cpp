// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <fstream>
#include <iterator>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <random>

#define _T(x) x

// AltSign
#include "DeviceManager.hpp"
#include "Error.hpp"

#include "AltServerApp.h"

#define odslog(msg) { std::stringstream ss; ss << msg << std::endl; OutputDebugStringA(ss.str().c_str()); }

#include <pplx/pplxtasks.h>
#include <pplx/threadpool.h>

#include <uuid/uuid.h>
#ifndef UUID_STR_LEN
#define UUID_STR_LEN 37
#endif

std::string make_uuid() {
    uuid_t b;
	char out[UUID_STR_LEN] = {0};
	uuid_generate(b);
  	uuid_unparse_lower(b, out);
	return out;
}

std::string temporary_directory()
{
	return fs::temp_directory_path().string();
}

std::vector<unsigned char> readFile(const char* filename)
{
	// open the file:
	std::ifstream file(filename, std::ios::binary);

	// Stop eating new lines in binary mode!!!
	file.unsetf(std::ios::skipws);

	// get its size:
	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// reserve capacity
	std::vector<unsigned char> vec;
	vec.reserve(fileSize);

	// read the data:
	vec.insert(vec.begin(),
		std::istream_iterator<unsigned char>(file),
		std::istream_iterator<unsigned char>());

	return vec;
}

#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#include <boost/stacktrace.hpp>

#include <usbmuxd.h>

void print_help() {
	printf("Usage:  AltServer-Linux options [ ipa-file ]\n");
	printf(
			"  -h  --help             Display this usage information.\n"
			"  -u  --udid UDID        Device's UDID, only needed when installing IPA.\n"
			"  -a  --appleID AppleID  Apple ID to sign the ipa, only needed when installing IPA.\n"
			"  -p  --password passwd  Password of Apple ID, only needed when installing IPA.\n"
			"  -d  --debug            Print debug output, can be used several times to increase debug level.\n"
			"\n"
			"The following environment var can be set for some special situation:\n"
			"  - ALTSERVER_ANISETTE_SERVER: Set to custom anisette server URL\n"
			"          if not set, the default one: https://armconverter.com/anisette/irGb3Quww8zrhgqnzmrx, is used\n"
			"  - ALTSERVER_NO_SUBSCRIBE: (*unused*) Please enable this for usbmuxd server that do not correctly usbmuxd_listen interfaces\n"
			);
}

int main(int argc, char *argv[]) {
	static struct option long_options[] =
        {
          {"udid",		required_argument,   	0, 'u'},
          {"appleID",	required_argument,      0, 'a'},
          {"password",	required_argument,      0, 'p'},
          //{"ipaddr",	required_argument,		0, 'i'},
		  //{"pairData",	required_argument,      0, 'P'},
		  {"debug",		no_argument,      		0, 'd'},
          {0, 0, 0, 0}
        };
	
	char *udid;
	char *ipaddr;
	char *appleID;
	char *password;
	char *pairDataFile;
	
	char *ipaPath = NULL;
	int debugLogLevel = 0;

	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;

		int c = getopt_long (argc, argv, "u:i:a:p:P:d",
						long_options, &option_index);
		if (c == -1) break;

		switch (c) {
        case 'u':
			udid = optarg;
            break;
       	case 'i':
			ipaddr = optarg;
			break;
        case 'a':
			appleID = optarg;
        case 'p':
            password = optarg;
			break;
		case 'P':
            pairDataFile = optarg;
			break;
		case 'd':
			//debugLog = true;
			debugLogLevel++;
			break;
		case 'h':
			print_help();
			exit(0);
		default:
            printf("?? getopt returned character code 0%o ??\n", c);
			print_help();
			exit(1);
    	}
	}

	if (argc == 1) {
		printf("No argument supplied, if you want for help, please use -h or --help\n");
	}

	bool installApp = true;
	if (optind == argc) {
		printf("Not supplying ipa, running in server mode!\n");
        installApp = false;
    } else if (optind + 1 == argc) {
		ipaPath = argv[optind];
	} else {
		printf("Unknown options: ");
 		while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
		return 1;
	}

	setvbuf(stdin, NULL, _IONBF, 0); 
    setvbuf(stdout, NULL, _IONBF, 0); 
    setvbuf(stderr, NULL, _IONBF, 0); 
	
	srand(time(NULL));

	if (debugLogLevel) {
		idevice_set_debug_level(debugLogLevel);
		libusbmuxd_set_debug_level(debugLogLevel - 2);
	}
    
	signal(SIGPIPE, SIG_IGN);

	if (installApp) {
		odslog("Installing app...");
		std::shared_ptr<Device> _selectedDevice = std::make_shared<Device>("unknown", udid, Device::Type::All);;
		std::optional<std::string> _ipaFilepath = std::make_optional<std::string>(ipaPath);
		auto task = AltServerApp::instance()->InstallApplication(_ipaFilepath, _selectedDevice, (appleID), (password));
		try
		{
			task.get();
		}
		catch (Error& error)
		{
			odslog("Error: " << error.domain() << " (" << error.code() << ").")
		}
		catch (std::exception& exception)
		{
			odslog("Exception: " << exception.what());
			odslog(boost::stacktrace::stacktrace());
		}

		odslog("Finished!");
	} else {
		AltServerApp::instance()->Start(0, 0);
		while (1) {
			sleep(100);
		}
	}
}