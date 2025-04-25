#include "cmd.h"
#include <vector>
#include <mutex>
#include <iostream>

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
                 //opt(stream, "1");
                 *stream << pr.first << ":" << opt.dump() << std::endl;
                
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
            std::cout << "t1" << std::endl;
        }while(0);

        static std::mutex s_mtx_opt;
        std::lock_guard<std::mutex> lck(s_mtx_opt);

        int index;
        optind = 0;
        opterr = 0;
        std::cout << "argc" <<argc  << std::endl;
        for(int i=0; i<argc; i++){
            std::cout << "\t" << argv[i] << std::endl;
        }

        
        while((index=getopt_long(argc, argv, &str_short_opt[0], &vec_long_opt[0], nullptr)) != -1){
            std::stringstream ss;
            ss << "未识别的选项，输入\"-h\"获取帮助";
            std::cout << "index:" << index << "(char)->" << (char)index << std::endl;
            std::cout << optind << " " << opterr << std::endl;

            
            if(index < 0xFF) {
                // 获取到短参数，找到索引
                auto it = _map_char_index.find(index);
                if(it == _map_char_index.end()) {
                    throw std::invalid_argument(ss.str());
                }
                index = it->second;
                std::cout << "index2:" << index << "(char)->" << (char)index << std::endl;

            }
            std::cout << "long opt" << std::endl;

        }
    }
}