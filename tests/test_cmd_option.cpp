#include <iostream>
#include "tools/cmd.h"
#include <memory>

using namespace std;
using namespace toolkit;



int main(int argc, char *argv[]){
   
    shared_ptr<OptionParser> _parser;
    mINI m;
    std::shared_ptr<std::ostream> coutPtr(&cout, [](std::ostream*) {});

    _parser.reset(new OptionParser([=](const shared_ptr<ostream> &stream, mINI &args){
        /*if(hasKey("connect")){
            connect(stream);
            return;
        }*/
        *stream << "optionparser cb lambda";
    }));
    (*_parser) << Option('T', "test", Option::ArgRequired, nullptr, true, "TEST X", nullptr);

    try{
        (*_parser)(m, argc, argv, coutPtr);
        cout << "22";
    }catch (std::exception &ex) {
        cout << ex.what() << endl;
    }
    




    cout<<"."<<endl;

}