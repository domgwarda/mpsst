#pragma once

#include <vector>
#include <string>
#include <hs/hs.h>
#include "abstract_file_scanner.h"

using namespace std;

class HSFileScanner : public AbstractFileScanner {
    private:
        hs_database_t *database = nullptr;
    public:
        explicit HSFileScanner(RegexDatabase database_variant);
        ~HSFileScanner() override;
        void scan_file(const std::string &path) override;
        void debug_scan_literal(hs_database_t *database) override;
};      
