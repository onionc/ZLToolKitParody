#pragma once

#include <functional>

namespace toolkit{

class onceToken{

public:
    using task = std::function<void(void)>;

    template<typename FUNC>
    onceToken(const FUNC &onConstructed, task onDestructed = nullptr){
        onConstructed();
        _onDestructed = std::move(onDestructed);
    }

    onceToken(std::nullptr_t, task onDestructed = nullptr){
        _onDestructed = std::move(onDestructed);
    }

    ~onceToken(){
        if(_onDestructed){
            _onDestructed();
        }
    }

private:
    onceToken() = delete; // 删除默认构造函数
    onceToken(const onceToken &) = delete; // 删除拷贝构造函数
    onceToken(onceToken &&) = delete; // 删除移动构造函数
    onceToken &operator=(const onceToken &) = delete; // 删除拷贝赋值函数
    onceToken &operator=(onceToken &&) = delete; // 删除移动赋值运算符

private:
    task _onDestructed;
};
}
