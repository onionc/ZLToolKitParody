#pragma once

#include "cmd.h"
#include <vector>

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
            
        }
    }
}