#pragma once

#include "engine.h"
#include "abstract_file_scanner.h"
#include "hs_file_scanner.h"

using namespace std;


class EngineFileScanner {
private:
    AbstractFileScanner* regex_file_scanner = nullptr;
public:
    EngineFileScanner(enum Engine eng_, RegexDatabase database_variant) { 
        switch (eng_) {
            case Hyperscan:
                regex_file_scanner = new HSFileScanner(database_variant);
                break;
            default:
                cerr << "Unknown engine type" << endl;
        }
    }

    ~EngineFileScanner() {
        delete regex_file_scanner;
    }

    AbstractFileScanner* get_engine() { 
        return regex_file_scanner;
    }
};
