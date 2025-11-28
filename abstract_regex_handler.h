#pragma once 

#include <vector>
#include <string>
#include <hs/hs.h>
using namespace std;

class AbstractRegexHandler {
    protected:
    public:
        AbstractRegexHandler(){};
        virtual ~AbstractRegexHandler() = default;
        virtual hs_database_t* get_database() = 0;
        virtual void load_regex_file(const string& filename) = 0;
        virtual void compile_regexes() = 0;
        virtual void load_regex_database(const string& filename) = 0;
        virtual void save_regex_database(const string& filename) = 0;

        virtual vector<string> get_regexs_vector() = 0;
        virtual int get_regexs_vector_size() = 0;
};      