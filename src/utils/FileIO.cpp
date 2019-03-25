#include <utils/FileIO.h>
#include <dirent.h>
#include <set>

namespace mathernogl {

//will return the contents of a file as a string
std::string readTextFile(std::string filePath) {
	std::ifstream textFile(filePath.c_str());
	std::stringstream fileContentsStream;

	if (textFile.is_open()) {
		fileContentsStream << textFile.rdbuf();
		textFile.close();
	} else
		throw std::runtime_error("Couldn't open file: " + filePath + ". ");

	return fileContentsStream.str();
}

//will create and write to the file, overwriting if file exists
void writeTextFile(std::string filePath, std::string text) {
	std::ofstream textFile(filePath.c_str());
	textFile << text;
	textFile.close();
}

//will create and write to the file, appending if the file exists
void appendTextFile(std::string filePath, std::string text) {
	std::ofstream textFile(filePath.c_str(), std::ios_base::app);
	textFile << text;
	textFile.close();
}

void getFilesInDirectory(const std::string& directoryPath, std::list<std::string>* files, const std::string& extension)
  {
  std::set<std::string> filesSet;
  DIR* directory;
  struct dirent* ent;
  if ((directory = opendir(directoryPath.c_str())) != nullptr)
    {
    while ((ent = readdir(directory)) != nullptr)
      {
      std::string fileName = ent->d_name;
      if (fileName == "." || fileName == "..")
        continue;

      if (!extension.empty() && getFileExtension(fileName) != extension)
        continue;

      filesSet.insert(fileName);
      }
    closedir(directory);
    }

  for (const std::string& file : filesSet)
    files->push_back(file);
  }

std::string removeFileExtension(const std::string& filePath){
  size_t idx = filePath.find_last_of('.');
  if (idx != std::string::npos)
    return filePath.substr(0, idx);
  return filePath;
}

std::string getFileExtension(const std::string& filePath){
  size_t idx = filePath.find_last_of('.');
  if (idx != std::string::npos)
    return filePath.substr(idx + 1);
  return "";
}

std::string getDirectoryFromPath(const std::string& filePath) {
  size_t idx = filePath.find_last_of('/');
  if (idx == std::string::npos)
    idx = filePath.find_last_of('\\');

  if (idx != std::string::npos)
    return filePath.substr(0, idx + 1);   // include the '/' at the end
  return "";
}

}
