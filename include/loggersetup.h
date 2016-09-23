#pragma once

#include <string>

#include "spdlog/spdlog.h"

namespace vtk {

class LoggerSetup {
public:
	LoggerSetup();

	void setLogFolder(const std::string& path);
	void setup();

protected:
	std::string mLogFolder;

};

}
