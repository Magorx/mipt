#pragma once


#include <string>


class Logger;
class Tag;


class TagProxy {
    friend Tag;

    Tag &tag;

public:
    TagProxy(Tag &tag);
    ~TagProxy();

    TagProxy &operator()(const std::string &arg_name, const std::string &arg);
};


class Tag {
    friend TagProxy;

    std::string name;
    Logger &logger;

public:
    Tag(const std::string &name, Logger &logger);

    TagProxy open();
    void close();
};
