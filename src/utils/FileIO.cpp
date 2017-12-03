#include <mathernogl/utils/FileIO.h>

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

}
