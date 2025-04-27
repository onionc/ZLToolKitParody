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

}