#include "config.h"
#include "sol.hpp"

template <>
std::string Config::getValue<std::string>(const std::string& key, const std::string& fallback) {
    if (values.find(key) != values.end()) {
        return values[key];
    }
    return fallback;
}

void Config::registerScriptInterface(sol::state &lua) {
	lua.new_usertype<Config>("Config",
	                         "load_from_file", &Config::loadConfigFromFile,
	                         "get_value", &Config::getValue<std::string>,
	                         "set_value", &Config::setValue<std::string>);
	                         
}
