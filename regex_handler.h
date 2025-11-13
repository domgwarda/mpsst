#include <vector>
#include <string>

#include <hs/hs.h>

using namespace std;

class RegexHandler {
    private: 
        vector<const char*> rgxs_ptrs_vector;
        const char* const* rgxs;
        
    public:
        hs_database_t *database;
        int size;
        string filename;

        vector<string> rgxs_strings_vector;
        void read_regex_file();
        void precompile_regexes();

        RegexHandler(string filename_);
};