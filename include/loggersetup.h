#pragma once

#include <string>

#include "spdlog/spdlog.h"


//forward declarations
namespace sol {
class state;
}

namespace vtk {

class LoggerSetup {
public:
	LoggerSetup();

	void setLogFolder(const std::string& path);
	void setup();

	static void registerScriptInterface(sol::state& lua);

protected:
	std::string mLogFolder;

};

}
