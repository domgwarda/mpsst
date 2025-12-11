#pragma once

#include <string>
#include "abstract_dir_scanner.h"

class PcreDirScanner : public AbstractDirScanner {
public:
    explicit PcreDirScanner(AbstractFileScanner &handler);
    void scan(const std::string &root) override;
};