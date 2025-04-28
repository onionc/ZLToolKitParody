#include <iostream>
#include "tools/option.h"
#include "tools/cmd.h"
#include <memory>

using namespace std;
using namespace toolkit;

class CMD_http : public CMD {
public:
    CMD_http() {
        _parser = std::make_shared<OptionParser>( [this](const std::shared_ptr<std::ostream> &stream, mINI &) {
            if(hasKey("connect")){
                (*stream) << "包含指令connect, 准备连接 " << endl;
                connect(stream);
                return;
            }

            if(hasKey("commit")) {
                commit(stream);
                return;
            }
        });

        (*_parser) << Option('T', "type", Option::ArgRequired, nullptr, true, "模式选择 0：传统模式，1：shell模式", nullptr);
        (*_parser) << Option('s', "server", Option::ArgRequired, "www.acfun.com:80", false, "tcp服务器地址", 
                            [this](const std::shared_ptr<ostream> &stream, const string &arg){ // 解析到该选项的回调
                                if(arg.find(":") == string::npos){
                                    throw std::runtime_error("\t地址必须有端口号");
                                }
                                (*stream) << "server 设置：" << arg << endl;
                                return true;
                            });
        (*_parser) << Option('d', "disconnect", Option::ArgNone, nullptr, false, "是否断开连接",
                            [this](const std::shared_ptr<ostream> &stream, const string &arg){
                                disconnect(stream);
                                return false;
                            });

        (*_parser) << Option('c', "connect", Option::ArgNone, nullptr, false, "发起tcp connect操作", nullptr);
        (*_parser) << Option('t', "time_out", Option::ArgRequired, "3" ,true, "连接超时间", nullptr);

        
    }
    ~CMD_http() {}

    const char* description() const override{
        return "http 测试客户端";
    }

private:
    void connect(const std::shared_ptr<ostream> &stream){
        (*stream) << "connect 操作" << endl;
    }
    void disconnect(const std::shared_ptr<ostream> &stream){
        (*stream) << "disconnect 操作" << endl;
    }
    void commit(const std::shared_ptr<ostream> &stream){
        (*stream) << "commit 操作" << endl;
    }
};

int main(int argc, char *argv[]){
    REGIST_CMD(http);

    try{
        // 默认解析 http
        CMD_DO("http", argc, argv);
    }catch (std::exception &ex){
        cout << "http: \n" << ex.what() << endl;
        return 0 ;
    }

    if(GET_CMD("http")["type"] == 0) {
        cout << "传统模式，已退出，请尝试shell模式" << endl;
        return 0;
    }

    // 删除模式选择
    GET_CMD("http").delOption("type");

    cout << "> 欢迎进入命令模式" << endl;
    string cmd_line;
    while(cin.good()){
        try{
            cout << "> ";
            getline(cin, cmd_line);
            // 解析所有命令
            CMDRegister::Instance()(cmd_line);
        }catch (ExitException &){
            break;
        }catch (std::exception &ex){
            cout << ex.what() << endl;
        }
    }

    return 0;
}