#include "tag.h"

#include "utils/logger.h"



TagProxy::TagProxy(Tag &tag) :
tag(tag)
{}

TagProxy::~TagProxy() {
    tag.logger.print(">");
}

TagProxy &TagProxy::operator()(const std::string &arg_name, const std::string &arg) {
    tag.logger.print(" %s=%s", arg_name.c_str(), arg.c_str());
    return *this;
}

Tag::Tag(const std::string &name, Logger &logger) :
name(name),
logger(logger)
{}

TagProxy Tag::open() {
    logger.print("<%s", name.c_str());

    return TagProxy(*this);
}

void Tag::close() {
    logger.print("</%s>", name.c_str());
}
