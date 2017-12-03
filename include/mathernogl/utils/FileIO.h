#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace mathernogl {

std::string readTextFile(std::string filePath);
void writeTextFile(std::string filePath, std::string text);
void appendTextFile(std::string filePath, std::string text);

}
