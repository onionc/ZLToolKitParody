#include "cmd.h"
#include "onceToken.h"

namespace toolkit{

// 默认注册指令
static onceToken s_token([](){
    REGIST_CMD(exit)
    REGIST_CMD(quit)
    REGIST_CMD(help)
    REGIST_CMD(clear)
});


CMDRegister &CMDRegister::Instance(){
    static CMDRegister instance;
    return instance;
}

}