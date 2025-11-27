#pragma once

#include <vector>
#include <string>
#include <hs/hs.h>

using namespace std;

class AbstractFileScanner {
    protected:
        hs_database_t *database;
    public:
        explicit AbstractFileScanner(hs_database_t *database_): database(database_) {};
        virtual ~AbstractFileScanner() = default;
        virtual void scan_file(const std::string &path) = 0;
        virtual void debug_scan_literal(hs_database_t *database) = 0;
};      