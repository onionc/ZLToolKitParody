#pragma once

#include <map>
#include <functional>
#include <memory>
#include <sstream>
#include "mini.h"


namespace toolkit{
    
class Option{
public:
    using OptionHandler = std::function<bool(const std::shared_ptr<std::ostream> &stream, const std::string &arg)>;

    enum ArgType {
        ArgNone = 0,
        ArgRequired = 1,
        ArgOptional = 2,
    };

    Option() = default;
    Option( char short_opt,
            const char *long_opt,
            enum ArgType type,
            const char *default_value,
            bool must_exist,
            const char *des,
            const OptionHandler &cb
            ){
        _short_opt = short_opt;
        _long_opt = long_opt;
        _type = type;
        if(type!=ArgNone){
            if(default_value){
                _default_value = std::make_shared<std::string>(default_value);
            }
            
            if(!default_value && must_exist){
                _must_exist = true;
            }
        }

        _des = des;
        _cb = cb;

    }


    bool operator() (const std::shared_ptr<std::ostream> &stream, const std::string &arg) {
        return _cb ? _cb(stream, arg) : true;
    }

private:
    friend class OptionParser;
    bool _must_exist = false;
    char _short_opt;
    enum ArgType _type;
    std::string _des;
    std::string _long_opt;
    OptionHandler _cb;
    std::shared_ptr<std::string> _default_value;
};

class OptionParser{

public:
    using OptionCompleted = std::function<void(const std::shared_ptr<std::ostream> &, mINI &)>;

    OptionParser(const OptionCompleted &cb = nullptr, bool enable_empty_args = true) {
        _on_completed = cb;
        _enable_empty_args = enable_empty_args;
        _helper = Option('h', "help", Option::ArgNone, nullptr, false, "打印帮助信息", 
            [this](const std::shared_ptr<std::ostream> &stream, const std::string &arg)->bool{
                static const char *argsType[] = {"无参", "有参", "选参"};
                static const char *mustExist[] = {"选填", "必填"};
                static std::string defaultPrefix = "默认";
                static std::string defaultNull = "null";

                std::stringstream printer;
                size_t maxLenLongOpt = 0;
                auto maxLenDefault = defaultNull.size();

                // 更新长度：长参数的最大长度，默认值最大长度。为了后续输出整齐
                for (auto &pr : _map_options) {
                    auto &opt = pr.second;
                    if(opt._long_opt.size() > maxLenLongOpt) {
                        maxLenLongOpt = opt._long_opt.size();
                    }
                    if(opt._default_value && opt._default_value->size() > maxLenDefault){
                        maxLenDefault = opt._default_value->size();
                    }
                }

                for (auto &pr : _map_options){
                    auto &opt = pr.second;
                    // 打印 短参和长参名
                    if(opt._short_opt){
                        printer << "  -" << opt._short_opt << "  --" << opt._long_opt;
                    }else{
                        printer << "      --" << opt._long_opt;
                    }

                    for(size_t i=0; i<maxLenLongOpt - opt._long_opt.size(); ++i){
                        printer << " ";
                    }

                    // 打印 是否有参
                    printer << "  " << argsType[opt._type];

                    // 打印 默认参数
                    std::string defaultValue = defaultNull;
                    if(opt._default_value){
                        defaultValue = *opt._default_value;
                    }
                    printer << "  " << defaultPrefix << defaultValue;
                    for(size_t i=0; i<maxLenDefault - defaultValue.size(); ++i){
                        printer << " ";
                    }

                    // 打印 是否必填参数
                    printer << "  " << mustExist[opt._must_exist];
                    // 打印 描述
                    printer << "  " << opt._des << std::endl;
                }
                throw std::invalid_argument(printer.str());
            });
        (*this) << _helper;
    }

    OptionParser &operator<<(Option &&option) {
        int index = 0XFF + (int) _map_options.size();
        if (option._short_opt){
            _map_char_index.emplace(option._short_opt, index);
        }
        _map_options.emplace(index, std::forward<Option>(option));
        return *this;
    }

    OptionParser &operator<<(const Option &option){
        int index = 0xFF + (int) _map_options.size();
        if(option._short_opt){
            _map_char_index.emplace(option._short_opt, index);
        }
        _map_options.emplace(index, option);
        return *this;
    }

    void delOption(const char *key){
        for(auto &pr : _map_options){
            if(pr.second._long_opt == key){
                _map_char_index.erase(pr.second._short_opt);
            }
            _map_options.erase(pr.first);
            break;
        }
    }

    void operator () (mINI &all_args, int argc, char *argv[], const std::shared_ptr<std::ostream> &stream);

private:
    bool _enable_empty_args;
    Option _helper;
    std::map<char, int> _map_char_index;
    std::map<int, Option> _map_options;
    OptionCompleted _on_completed;
};

}


