#pragma once

#include <cstring>
#include <string>

namespace toolkit{


// string 转小写
std::string &strToLower(std::string &str);
std::string strToLower(std::string &&str);

// string  转大写
std::string &strToUpper(std::string &str);
std::string strToUpper(std::string &&str);


}