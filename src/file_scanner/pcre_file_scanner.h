#pragma once

#include "abstract_file_scanner.h"
#include <pcre2.h>
#include <vector>

class PCREFileScanner : public AbstractFileScanner{
private:
    std::string read_file_content(const std::string& path);
    std::vector<pcre2_code*>* compiled_regexs = nullptr;
public:
    explicit PCREFileScanner(RegexDatabase database_variant_);
    void scan_file(const std::string &path) override;
    // void debug_scan_literal(RegexDatabase *database) override {};

};