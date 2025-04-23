#include "utils.h"
#include "algorithm"
#include "File.h"

namespace toolkit{

// string 转小写
std::string &strToLower(std::string &str){
    transform(str.begin(), str.end(), str.begin(), 
    [](unsigned char c){return std::tolower(c); }); // uchar 确保 tolower 不会UB
    return str;
}
std::string strToLower(std::string &&str){
    transform(str.begin(), str.end(), str.begin(), 
    [](unsigned char c){return std::tolower(c); }); 
    return std::move(str);
}

// string  转大写
std::string &strToUpper(std::string &str){
    transform(str.begin(), str.end(), str.begin(), 
    [](unsigned char c){return std::toupper(c); }); 
    return str;
}
std::string strToUpper(std::string &&str){
    transform(str.begin(), str.end(), str.begin(), 
    [](unsigned char c){return std::toupper(c); }); 
    return std::move(str);
}

// trim
#define TRIM(s, chars) \
do{ \
    std::string map(0xFF, '\0'); \
    for(auto &ch : chars) { \
        map[0xFF & ch] = '\1'; \
    } \
    while(s.size() && map.at(0xFF & s.back())) s.pop_back(); \
    while(s.size() && map.at(0xFF & s.front())) s.erase(0,1); \
}while(0);

std::string& trim(std::string &s, const std::string &chars){
    TRIM(s, chars);
    return s;
}
std::string trim(std::string &&s, const std::string &chars){
    TRIM(s, chars);
    return std::move(s);
}

std::string exePath(bool isExe){
    char buffer[PATH_MAX*2 + 1] = {0};
    int n=-1;
    
    #if defined(__linux__)
    n=readlink("/proc/self/exe", buffer, sizeof(buffer));
    #else
    // 其他平台下获取文件名
    TODO
    #endif

    std::string filePath;
    if(n<=0){
        filePath = "./";
    }else{
        filePath = buffer;
    }

    #if defined(__WIN32)
    TODO: 路径转换 \ 转 / 
    #endif

    return filePath;
}

}