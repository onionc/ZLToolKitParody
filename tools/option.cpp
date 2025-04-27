#include "option.h"
#include <vector>
#include <mutex>

#if defined(__WIN32)
#include "win32/getopt.h"
#else
#include <getopt.h>
#endif

namespace toolkit{
    void OptionParser::operator() (mINI &all_args, int argc, char *argv[], const std::shared_ptr<std::ostream> &stream) {
        std::vector<struct option> vec_long_opt;
        std::string str_short_opt;
        do {
            struct option tmp;
            for(auto &pr : _map_options){
                // *stream << pr.first << ":" << pr.second.dump() << std::endl;
                // std::cout << pr.first << ":" << pr.second.dump() << std::endl;
                // 获取 Option
                auto &opt = pr.second;
                
                // 长参数
                tmp.name = (char *) opt._long_opt.data();
                tmp.has_arg = opt._type;
                tmp.flag = nullptr;
                tmp.val = pr.first;
                vec_long_opt.emplace_back(tmp);
                // 短参数
                if(!opt._short_opt){
                    continue;
                }
                str_short_opt.push_back(opt._short_opt);
                switch(opt._type){
                    case Option::ArgRequired: str_short_opt.append(":"); break;
                    case Option::ArgOptional: str_short_opt.append("::"); break;
                    default: break;
                }
            }
            tmp.flag = 0;
            tmp.name = 0;
            tmp.has_arg = 0;
            tmp.val = 0;
            vec_long_opt.emplace_back(tmp);
        }while(0);

        static std::mutex s_mtx_opt;
        std::lock_guard<std::mutex> lck(s_mtx_opt);

        int index;
        optind = 0;
        opterr = 0;

        while((index=getopt_long(argc, argv, &str_short_opt[0], &vec_long_opt[0], nullptr)) != -1){
            std::stringstream ss;
            ss << "未识别的选项，输入\"-h\"获取帮助";
            if(index < 0xFF) {
                //std::cout << "index:" << index << "(char)->" << (char)index << std::endl;

                // 获取到短参数，找到索引
                auto it = _map_char_index.find(index);
                if(it == _map_char_index.end()) {
                    throw std::invalid_argument(ss.str());
                }
                index = it->second;
            }

            //std::cout << "index2:" << index << std::endl;
            //std::cout << "long opt" << std::endl;
            // 通过 index 获取 option
            auto it = _map_options.find(index);
            if(it==_map_options.end()){
                throw std::invalid_argument(ss.str());
            }
            auto &opt = it->second;
            // std::cout<< "找到命令 " << opt.dump() << std::endl;
            
            // optarg 是带有参数选项的参数
            // all_args 为设置的选项
            auto pr = all_args.emplace(opt._long_opt, optarg ? optarg : "");
            // 调用 Option handle 处理函数
            if(!opt(stream, pr.first->second)){
                return;
            }
            optarg = nullptr;
        }
        // TODO : 有两个一样的参数 ？？ 
        // std::cout << all_args.dump() << std::endl;

        // 处理带有默认值的参数
        for(auto &pr : _map_options){
            // 有默认值，并且没有提供默认值的参数，使用默认值
            if(pr.second._default_value && all_args.find(pr.second._long_opt) == all_args.end()){
                all_args.emplace(pr.second._long_opt, *pr.second._default_value);
            }
        }

        // 检查必填参数
        for(auto &pr : _map_options){
            if(pr.second._must_exist){
                if(all_args.find(pr.second._long_opt) == all_args.end()){
                    std::stringstream ss;
                    ss<<" 参数\"" << pr.second._long_opt << "\"为必填项，输入\"-h\"选项获取帮助";
                    throw std::invalid_argument(ss.str());
                }
            }
        }

        if(all_args.empty() && _map_options.size()>1 && !_enable_empty_args){
            _helper(stream, "");
            return;
        }

        if(_on_completed){
            _on_completed(stream, all_args);
        }
    }
}