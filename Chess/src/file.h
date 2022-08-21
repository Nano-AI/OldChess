#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <loguru.hpp>

class File {
public:
	static std::string ReadFile(std::string path) {
		LOG_F(INFO, "Reading from file %s", path.c_str());
		std::ifstream file(path);
		std::string output = "";
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				output += line;
			}
			file.close();
		}
		return output;
	}
};
