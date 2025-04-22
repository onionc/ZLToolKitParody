#include "utils.h"
#include "algorithm"

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

}