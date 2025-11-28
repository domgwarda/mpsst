#pragma once

#include <vector>
#include <string>
#include <hs/hs.h>
#include "abstract_file_scanner.h"

using namespace std;

class HSFileScanner : public AbstractFileScanner {
    public:
        explicit HSFileScanner(hs_database_t *database);
        ~HSFileScanner() override;
        void scan_file(const std::string &path) override;
        void debug_scan_literal(hs_database_t *database) override;
};      
