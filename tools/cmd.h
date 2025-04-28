#pragma once

#include "tools/mini.h"
#include "tools/option.h"
#include <iostream>
#include <mutex>

namespace toolkit{

class CMD : public mINI {

public:
    virtual ~CMD() = default;
    virtual const char* description() const{
        return "default description";
    }

    void operator() (int argc, char *argv[], const std::shared_ptr<std::ostream> &stream = nullptr) {
        this->clear();
        std::shared_ptr<std::ostream> coutPtr(&std::cout, [](std::ostream *){});
        (*_parser)(*this, argc, argv, stream ? stream : coutPtr);
    }

    bool hasKey(const char *key){
        return this->find(key) != this->end();
    }

    // 使用 delim 拆分值
    std::vector<variant> splitedVal(const char *key, const char *delim = ":"){
        std::vector<variant> ret;
        auto &val = (*this)[key];
        split(val, delim, ret);
        return ret;
    }

    void delOption(const char *key){
        if(_parser){
            _parser->delOption(key);
        }
    }

protected:
    std::shared_ptr<OptionParser> _parser;

private:
    void split(const std::string &s, const char *delim, std::vector<variant> &ret) {
        size_t last = 0;
        auto index = s.find(delim, last);
        while (index != std::string::npos){
            if(index-last > 0){
                ret.push_back(s.substr(last, index - last));
            }
            last = index + strlen(delim);
            index = s.find(delim, last);
        }
        if(s.size() - last > 0){
            ret.push_back(s.substr(last));
        }
    }

};


class CMDRegister {

public:
    static CMDRegister &Instance();

    void clear() {
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        _cmd_map.clear();
    }

    void registerCMD(const char *name, const std::shared_ptr<CMD> &cmd){
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        _cmd_map.emplace(name, cmd);
    }

    void unregisterCMD(const char *name){
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        _cmd_map.erase(name);
    }

    std::shared_ptr<CMD> operator[] (const char *name){
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        auto it = _cmd_map.find(name);
        if(it == _cmd_map.end()){
            throw std::invalid_argument(std::string("CMD not existed: ") + name);
        }
        return it->second;
    }

    void operator() (const char *name, int argc, char *argv[], const std::shared_ptr<std::ostream> &stream = nullptr){
        auto cmd = (*this)[name];
        if(!cmd){
            throw std::invalid_argument(std::string("CMD not existed: ") + name);
        }
        (*cmd)(argc, argv, stream);
    }

    void printHelp(const std::shared_ptr<std::ostream> &streamTmp = nullptr) {
        auto stream = streamTmp;
        if(!stream){
            stream.reset(&std::cout, [](std::ostream *){});
        }
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        
        // 获取最大长度
        size_t maxLen = 0;
        for(auto &pr : _cmd_map){
            if(pr.first.size() > maxLen) {
                maxLen = pr.first.size();
            }
        }

        for(auto &pr : _cmd_map){
            (*stream) << "  " << pr.first;
            for(size_t i=0; i<maxLen-pr.first.size(); ++i){
                (*stream) << "  ";
            }
            (*stream) << "  " << pr.second->description() << std::endl;
        }
    }

    void operator() (const std::string &line, const std::shared_ptr<std::ostream> &stream = nullptr){
        if(line.empty()){
            return;
        }
        std::vector<char*> argv;
        size_t argc = getArgs((char *) line.data(), argv);
        if(argc == 0){
            return;
        }
        std::string cmd = argv[0];
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        auto it = _cmd_map.find(cmd);
        if(it == _cmd_map.end()){
            std::stringstream ss;
            ss << "未识别的命令\"" << cmd << "\"，输入 \"help\" 获取帮助。";
            throw std::invalid_argument(ss.str());
        }
      
        (*it->second)((int)argc, &argv[0], stream);
    }

    
private:
    size_t getArgs(char *buf, std::vector<char *> &argv){
        size_t argc = 0;
        bool start = false;
        auto len = strlen(buf);
        for(size_t i=0; i<len; i++){
            if(buf[i]!=' ' && buf[i]!='\t' && buf[i]!='\r' && buf[i]!='\n') {
                if(!start){
                    start = true;
                    if(argv.size() < argc+1){
                        argv.resize(argc+1);
                    }
                    argv[argc++] = buf+i;
                }
            }else{
                buf[i] = '\0';
                start = false;
            }
        }
        return argc;
    }

    std::recursive_mutex _mtx;
    std::map<std::string, std::shared_ptr<CMD>> _cmd_map;
};

// 帮助命令
class CMD_help : public CMD {
public:
    CMD_help() {
        _parser = std::make_shared<OptionParser>( [](const std::shared_ptr<std::ostream> &stream, mINI &) {
            CMDRegister::Instance().printHelp(stream);
        });
    }

    const char* description() const override {
        return "打印帮助信息";
    }
};

#define CMD_quit CMD_exit
class ExitException : public std::exception {};

// 退出程序命令
class CMD_exit : public CMD {
public:
    CMD_exit() {
        _parser = std::make_shared<OptionParser>( [](const std::shared_ptr<std::ostream>&, mINI &) {
            throw ExitException();
        });
    }

    const char* description() {
        return "退出 shell";
    }
};


// 清空屏幕
class CMD_clear : public CMD {
public:
    CMD_clear() {
        _parser = std::make_shared<OptionParser>( [this](const std::shared_ptr<std::ostream> &stream, mINI &) {
            clear(stream);
        });

    }

    const char* description() {
        return "清空屏幕输出";
    }
private:
    void clear(const std::shared_ptr<std::ostream> &stream) {
        // x1b[2J 清屏， \x1b[H 光标移动到左上角
        (*stream) << "\x1b[2J\x1b[H";
        stream->flush();
    }
};


// 几个宏
// 获取命令
#define GET_CMD(name) (*(CMDRegister::Instance()[name]))
// 执行命令
#define CMD_DO(name, ...) (*(CMDRegister::Instance()[name]))(__VA_ARGS__)
// 注册命令，实现类需要是CMD_XXXX 格式（XXXX为name）
#define REGIST_CMD(name) CMDRegister::Instance().registerCMD(#name, std::make_shared<CMD_##name>());




}