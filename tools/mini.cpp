#include "mini.h"

namespace toolkit{
template<>
mIniBasic<std::string, variant> &mIniBasic<std::string, variant>::Instance(){
    static mINI_basic<string, variant> instance;
    return instance;
}

}