#pragma once

#include <cstring>
#include <string>


// 系统相关头文件
#if defined(__WIN32)

#else
#include <unistd.h>
#endif

namespace toolkit{


// string 转小写
std::string &strToLower(std::string &str);
std::string strToLower(std::string &&str);

// string  转大写
std::string &strToUpper(std::string &str);
std::string strToUpper(std::string &&str);

// trim, 去除前后空格、回车，制表符
std::string& trim(std::string &s, const std::string &chars=" \r\n\t");
std::string trim(std::string &&s, const std::string &chars=" \r\n\t");


// 文件相关
std::string exePath(bool isExe = true);

}