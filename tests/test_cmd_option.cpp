#include <iostream>
#include "tools/option.h"
#include <memory>

using namespace std;
using namespace toolkit;


/* 测试
    ./test_cmd_option           , output: 参数"test"为必填项，输入"-h"选项获取帮助
    ./test_cmd_option -h        , output: xxxx
    ./test_cmd_option -T  1     , output: xxxx
*/
int main(int argc, char *argv[]){
   
    shared_ptr<OptionParser> _parser;
    mINI m;
    std::shared_ptr<std::ostream> coutPtr(&cout, [](std::ostream*) {});

    _parser.reset(new OptionParser([=](const shared_ptr<ostream> &stream, mINI &args){

        *stream << "optionparser cb lambda\n";
        *stream << args.dump() << std::endl;
    }));
    (*_parser) << Option('T', "test", Option::ArgRequired, nullptr, true, "TEST X", nullptr);

    try{
        (*_parser)(m, argc, argv, coutPtr);
    }catch (std::exception &ex) {
        cout << ex.what() << endl;
    }

    cout<<"end."<<endl;
    return 0;
}