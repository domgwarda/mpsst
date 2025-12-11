#pragma once

#include <string>
#include "abstract_file_scanner.h"

class AbstractDirScanner {
public:
    explicit AbstractDirScanner(AbstractFileScanner &handler): handler_(handler) {};
    virtual void scan(const std::string &root) = 0;
protected:
    AbstractFileScanner &handler_;
};
