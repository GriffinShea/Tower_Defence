#ifndef FILEUTILS_H
#define FILEUTILS_H

using namespace std;

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

class FileUtils {
public:
	static string LoadTextFile(const char *filename);
};
#endif // !FILEUTILS_H
