#include "tools/function_traits.h"
#include <iostream>
#include <typeinfo>
#include <functional>

using namespace std;
using namespace toolkit;

// 工具函数，打印类型名字
template<typename T>
void print_type(){
    cout<<typeid(T).name() << endl;
}

// 普通函数
int normal_func(double, const string &){
    return 42;
}

// 函数指针
int (*func_ptr)(double, const string&) = normal_func;

// std::function
std::function<int(double, const string&)> std_function = normal_func;

// 成员函数
class MyClass {
public:
    float member_func(int, char){ return 0.5f; }
    double member_func_const(int, char) const { return 0.5f; }
};

// 函数对象（仿函数）
class Functor{
public:
    int operator()(float, bool){
        return 99;
    }
};

int main(){
    cout<<"普通函数：\n";
    cout<<"\t参数个数：" << function_traits<decltype(normal_func)>::arity << endl;
    cout<<"\t返回类型：";
    print_type<function_traits<decltype(normal_func)>::return_type>();
    cout<<"\t第一个参数类型：";
    print_type<function_traits<decltype(normal_func)>::args<0>::type>();
    cout<<"\t第二个参数类型：";
    print_type<function_traits<decltype(normal_func)>::args<1>::type>();
    //cout<<"\t第三个参数类型（不存在）：";
    //print_type<function_traits<decltype(normal_func)>::args<2>::type>();

    cout<<"\n函数指针：\n";
    cout<<"\t参数个数：" << function_traits<decltype(func_ptr)>::arity << endl;

    cout<<"\nstd::function：\n";
    cout<<"\t参数个数："<< function_traits<decltype(std_function)>::arity << endl;

    cout<<"\n成员函数  非const和const：\n";
    cout<<"\t参数个数："<<function_traits<decltype(&MyClass::member_func)>::arity << endl;
    cout<<"\t参数个数："<<function_traits<decltype(&MyClass::member_func_const)>::arity << endl;
    cout<<"\t返回类型：";
    print_type<function_traits<decltype(&MyClass::member_func_const)>::return_type>();

    cout<<"\n仿函数：\n";
    cout<<"\t参数个数："<<function_traits<Functor>::arity << endl;
    cout<<"\t返回类型：";
    print_type<function_traits<Functor>::return_type>();
    
    return 0;
}