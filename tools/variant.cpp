#include "variant.h"


namespace toolkit{

template<>
bool variant::as<bool>() const {
    // 为空或者数字开头
    if(empty() || isdigit(front())) {
        return as_default<bool>();
    }

    if(strToLower(std::string(*this)) == "true"){
        return true;
    }

    if(strToLower(std::string(*this)) == "false"){
        return false;
    }

    // 未识别字符
    return as_default<bool>();

}

template<>
uint8_t variant::as<uint8_t> () const{
    return 0xFF & as_default<int>();
}

}
