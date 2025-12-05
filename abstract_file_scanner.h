#pragma once

#include <vector>
#include <string>
#include "regex_database.h"

using namespace std;

class AbstractFileScanner {
    protected:
        RegexDatabase database_variant;
    public:
        explicit AbstractFileScanner(RegexDatabase database_variant_): database_variant(database_variant_) {};
        virtual ~AbstractFileScanner() = default;
        virtual void scan_file(const std::string &path) = 0;
        virtual void debug_scan_literal(hs_database_t *database) = 0;
};      