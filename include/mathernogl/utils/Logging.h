#pragma once

#include <iostream>
#include <ctime>

#include <mathernogl/utils/FileIO.h>

namespace mathernogl {

void resetLog();
void logError(std::string message);
void logWarning(std::string message);
void logInfo(std::string message);
void logProgramState(std::string message);

}
