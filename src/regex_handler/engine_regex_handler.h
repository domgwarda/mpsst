#pragma once

#include "engine.h"
#include "abstract_regex_handler.h"
#include "hs_regex_handler.h"
#include "pcre_regex_handler.h"


using namespace std;

class EngineRegex {
private:
    AbstractRegexHandler *regex_handler = nullptr;
public:
    EngineRegex(enum Engine eng_) { 
        switch (eng_) {
            case Hyperscan:
                regex_handler = new HSRegexHandler();
                break;
            case PCRE2:
                regex_handler = new PCRERegexHandler();
                break;
            default:
                cerr << "Unknown engine type" << endl;
        }
    }

    ~EngineRegex() {
        delete regex_handler;
    }

    AbstractRegexHandler* get_engine() { 
        return regex_handler;
    }
};
