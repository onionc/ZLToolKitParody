#include "variant.h"
#include <iostream>

int main(){
    
    toolkit::variant a1("True");
    std::cout<<a1<<": " << a1.as<bool>() << " " << (bool)a1 << std::endl;

    toolkit::variant a2("false");
    std::cout<<a2<<": " << a2.as<bool>() << " " << (bool)a2 << std::endl;

    toolkit::variant a3("257");
    std::cout<<a3<<": "
        " (int)" << (int)a3 << 
        " (as<uint8_t>)" << (int) a3.as<uint8_t>() << 
        " (uint8_t)" << (int)(uint8_t) a3 << std::endl;
}