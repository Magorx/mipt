#include "func_logger.h"

#include "utils/logger.h"


const int FUNC_CALL_OFFSET_SHIFT = 4;


FuncLogger::FuncLogger(const std::string &name) :
name(name)
{
    if (call_cnt.contains(name)) {
        ++call_cnt[name];
    } else {
        call_cnt[name] = 1;
    }

    logger.print_log_prefix("mclg", "  func  ");
    logger.print("[%d] %s() <{\n", call_cnt[name], name.c_str());
    logger.n();

    logger.shift_offset(+FUNC_CALL_OFFSET_SHIFT);
}

FuncLogger::~FuncLogger() {
    logger.shift_offset(-FUNC_CALL_OFFSET_SHIFT);

    logger.print_log_prefix("mclg", "  func  ");
    logger.print("}> [%d] %s()\n", call_cnt[name], name.c_str());

    --call_cnt[name];
}

std::map<std::string, int> FuncLogger::call_cnt;
