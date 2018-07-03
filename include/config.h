/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  Processes command line arguments and config files in header only library
 *
 *        Version:  1.0
 *        Created:  03/26/2014 09:30:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Michael Porter
 *
 * =====================================================================================
 */

/*

EXAMPLE CONFIG:

;this is a comment

key = value
integer = 69
float = 3.1415926
bool = 1
anotherbool = false

hierarchything {

    subkey = "value as a string"

}

*/

#pragma once

#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

//Config stores values
namespace sol {
class state;
}

class Config {
public:
    bool loadConfigFromFile(const std::string& fileName) { 
        std::ifstream file(fileName);
        if (!file) return false;
        std::string line;
        std::vector<std::string> hierarchyPosition;

        while (std::getline(file, line)) {
            auto temp = line; //copy line for safety
            trimString(temp);
            if (temp.find_first_of(";") != std::string::npos) { //contains comment
                temp.erase(temp.find_first_of(";"), temp.size() - 1);
            }
            if (!temp.empty()) { //it's not empty
                if (temp[0] != '{' && temp.find_first_of("{") != std::string::npos) {
                    std::string key = temp.substr(0, temp.find_first_of("{"));
                    trimString(key);
                    hierarchyPosition.push_back(key);
                } else if (temp[0] == '}') {
                    hierarchyPosition.pop_back();
                } else if (temp[0] != '=' && temp.find_first_of("=") != std::string::npos) { //it's assigning something!

                    std::string key = temp.substr(0, temp.find_first_of("="));
                    trimString(key);

                    std::string value = temp.substr(temp.find_first_of("=") + 1, temp.size() - 1);
                    trimString(value);

                    if (!hierarchyPosition.empty()) {
                        std::string tempKey = key;
                        key.clear();
                        for (auto& i : hierarchyPosition) {
                            key.append(i);
                            key.append(".");
                        }
                        key.append(tempKey);
                    }
                    values[key] = value;
                }
            }
        }
        return true;
    }

    template <typename T>
    T getValue(const std::string& key, const T& fallback) {
        if (values.find(key) != values.end()) {
            stream.clear();
            stream << values[key];
            T value;
            stream >> value;
            return value;
        }
        return fallback;
    }
    
    
    template <typename T>
    void setValue(const std::string& key, const T& value) {
        stream.clear();
        stream << value;
        stream >> values[key];
    }

    void addArgumentRule(const std::string& flag, const std::string& key) { //sets a value to be overriden by command line flag
        commandRules[flag] = key;
    }

    bool loadConfigFromArguments(const int& argc, char* argv[]) { 
        for (int i = 1; i < argc; i += 2) {
            auto flag = std::string(argv[i]);
            auto value = std::string(argv[i+1]);
            if (commandRules.find(flag) != commandRules.end()) {
                values[commandRules[flag]] = value;
            } else return false;
        }
        return true;
    }

    void printConfig() {
        for (auto& i : values) {
            std::cout << i.first << " = " << i.second << std::endl;
        }
    }

	static void registerScriptInterface(sol::state& lua);

protected:
    std::map<std::string, std::string> values;
    std::map<std::string, std::string> commandRules;
    std::stringstream stream;

    void trimString(std::string& string) {
        size_t end = string.find_last_not_of(" \t\n\v\f\r");
        if ( end != std::string::npos )
            string.resize( end + 1 );

        size_t start = string.find_first_not_of(" \t\n\v\f\r");
        if ( start != std::string::npos )
            string = string.substr( start );
    }
};


