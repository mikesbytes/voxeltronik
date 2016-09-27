#include "loggersetup.h"

namespace vtk {

	LoggerSetup::LoggerSetup() {
		mLogFolder = "logs";
	}

	void LoggerSetup::setLogFolder(const std::string& path) {
		mLogFolder = path;
	}

	void LoggerSetup::setup() {
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/log", "log", 1048576 * 5, 3));
		logSinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
		auto generalLogger = std::make_shared<spdlog::logger>("general", begin(logSinks), end(logSinks));
		auto fileLogger = std::make_shared<spdlog::logger>("file", begin(logSinks), end(logSinks));
		spdlog::register_logger(generalLogger);
		spdlog::register_logger(fileLogger);
		spdlog::set_level(spdlog::level::debug);
	}
}
