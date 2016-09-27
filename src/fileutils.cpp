#include <fstream>
#include <string>

#include "fileutils.h"
#include "spdlog/spdlog.h"

namespace vtk::file {

std::string loadFileIntoString(const std::string& fName) {
	std::ifstream in(fName, std::ios::in | std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		spdlog::get("file")->info("Loaded file: {}", fName);
		return contents;
	}
	spdlog::get("file")->warn("Unable to load: {}, returning empty string", fName);
	return "";
}


}
