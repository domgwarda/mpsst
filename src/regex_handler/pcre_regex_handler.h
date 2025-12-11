#pragma once

#include <vector>
#include <string>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>


#include "abstract_regex_handler.h"

using namespace std;

class PCRERegexHandler : public AbstractRegexHandler {
    private: 
        RegexDatabase database;
        int size;
        vector<string> rgxs_strings_vector;
    public:
        PCRERegexHandler();
        ~PCRERegexHandler();

        RegexDatabase get_database() override;
        void load_regex_file(const string& filename) override;
        void load_regex_database(const string& filename) override; 
        void save_regex_database(const string& filename) override;
        void compile_regexes() override;

        vector<string> get_regexs_vector() override;
        int get_regexs_vector_size() override;
};      
