#include <iostream>
#include "tools/mini.h"

using namespace std;
using namespace toolkit;

int main(){

    mINI ini;


    // 测试解析
    cout<<"测试解析函数 parse()  ---------------"<<endl;
    ini.parse(R"(
        [socket ]
        null=
        ip=1
        addr = 3
        =4
        =5
        b=6

    )");
    for(auto &pr : ini){
        cout<<pr.first << "=" << pr.second << endl;
    }

    // 测试 dump
    cout<<"测试dump()  --------------------"<<endl;
    cout<<ini.dump()<<endl;

    // 保存文件
     cout<<"测试文件保存  --------------------"<<endl;
    ini.dumpFile();

    // 从文件读取
    cout<<"测试文件读取  --------------------"<<endl;
    ini.parseFile();
    cout<<ini.dump()<<endl;

    // 清除
    ini.clear();

    // map
    cout<<"map赋值  --------------------"<<endl;
    ini["mqtt.ip"] = "192.168.1.12";
    ini["mqtt.port"] = 80;
    ini["ws.port"] = 8081;
    ini["ws.bool"] = false;
    cout<<ini.dump()<<endl;
    

     // 测试一个bug
     mINI a;
     a["a"] = 1;
     a["b.x"] = 2;
     a["c"] = 3;
     std::cout<< a.dump() << std::endl;
     // 保存文件
     cout<<"测试文件保存  --------------------"<<endl;
     a.dumpFile();
     // 从文件读取
     cout<<"测试文件读取  --------------------"<<endl;
     a.parseFile();
     cout<<a.dump()<<endl;
}