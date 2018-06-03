#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <list>

namespace mathernogl {

std::string readTextFile(std::string filePath);
void writeTextFile(std::string filePath, std::string text);
void appendTextFile(std::string filePath, std::string text);
void getFilesInDirectory(const std::string& directoryPath, std::list<std::string>* files, const std::string& extension = "");
std::string removeFileExtension(const std::string& filePath);
std::string getFileExtension(const std::string& filePath);
std::string getDirectoryFromPath(const std::string& filePath);    //  removes the file name component from the file path, obtaining the directory that file is in

}
