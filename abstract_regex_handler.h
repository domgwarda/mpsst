#include <vector>
#include <string>
#include <hs/hs.h>
using namespace std;

class AbstractRegexHandler {
    protected:
        const string filename;
    public:
        AbstractRegexHandler(const std::string& file): filename(file) {}
        virtual ~AbstractRegexHandler() = default;
        virtual hs_database_t* get_database() = 0;
        virtual void load_regex_file() = 0;
        virtual void compile_regexes() = 0;
};      
