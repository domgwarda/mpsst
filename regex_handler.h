/*
RegexHandler Class
- Manages regexes loaded from a file and compiles them into a Hyperscan database,

Methods:
- RegexHandler(const string filename):
    Constructs a RegexHandler with a constant filename,

- void load_regex_file():
    Loads regular expressions from the file specified by `filename`.
    Stores them as `const char* const` type,

- void compile_regexes():
    Compiles all loaded regexes into a Hyperscan database.
    Stores the compiled database,

- hs_database_t* get_database():
    Returns the compiled Hyperscan database.
    Must be called only after `precompile_regexes()` has been executed successfully.
*/


#include <vector>
#include <string>

#include <hs/hs.h>



using namespace std;

class RegexHandler {
    private: 
        vector<const char*> rgxs_ptrs_vector;
        const char* const* rgxs;
        hs_database_t *database = nullptr;
    public:
        int size;
        vector<string> rgxs_strings_vector;
        const string filename;
        RegexHandler(string filename_);
        hs_database_t* get_database();
        void load_regex_file();
        void compile_regexes();

};