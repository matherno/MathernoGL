#include <utils/Logging.h>

#define LOGNAME "Log.txt"

namespace mathernogl {

void resetLog() {
	std::time_t timeNow = std::time(0);
	std::string timeStamp = std::ctime(&timeNow);

	writeTextFile(LOGNAME, "MathernoGL Log: " + timeStamp);
}

void logError(std::string message) {
	appendTextFile(LOGNAME, "ERROR: " + message + "\n");
	std::cout << "ERROR: " + message << std::endl;
}

void logWarning(std::string message) {
	appendTextFile(LOGNAME, "WARNING: " + message + "\n");
	std::cout << "WARNING: " + message << std::endl;
}

void logInfo(std::string message) {
	appendTextFile(LOGNAME, message + "\n");
	std::cout << message << std::endl;
}

void logProgramState(std::string message) {
	appendTextFile(LOGNAME, "-- " + message + " --\n");
	std::cout << "-- " << message << " --" << std::endl;
}

}
