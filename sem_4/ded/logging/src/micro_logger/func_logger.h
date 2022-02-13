#pragma once


#include <string>
#include <map>


extern const int FUNC_CALL_OFFSET;


class FuncLogger {
    std::string name;

    static std::map<std::string, int> call_cnt;

public:
    FuncLogger(const std::string &name);
    ~FuncLogger();
};
