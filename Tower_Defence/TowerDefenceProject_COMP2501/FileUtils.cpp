#include "FileUtils.h"

string FileUtils::LoadTextFile(const char *filename) {
	// Open file
	std::ifstream f;
	f.open(filename);
	if (f.fail()) {
		throw(std::ios_base::failure(string("Error opening file ") + string(filename)));
	}

	// Read file
	string content;
	string line;
	while (std::getline(f, line)) {
		content += line + "\n";
	}

	// Close file
	f.close();

	return content;
}
