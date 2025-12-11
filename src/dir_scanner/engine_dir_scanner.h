#pragma once

#include "engine.h"
#include "abstract_dir_scanner.h"
#include "hs_dir_scanner.h"


using namespace std;


class EngineDirScanner {
private:
    AbstractDirScanner* regex_dir_scanner = nullptr;
public:
    EngineDirScanner(enum Engine eng_, AbstractFileScanner* file_scanner) { 
        switch (eng_) {
            case Hyperscan:
                regex_dir_scanner = new HSDirScanner(*file_scanner);
                break;
            default:
                cerr << "Unknown engine type" << endl;
        }
    }

    ~EngineDirScanner() {
        delete regex_dir_scanner;
    }

    AbstractDirScanner* get_engine() { 
        return regex_dir_scanner;
    }
};
