/*
Regexes-file:
Regex0\n
Regex1\n
Regex2\n
...
RegexN\n
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

#include <hs/hs.h>
#include "./regex_handler.h"

using namespace std;

RegexHandler::RegexHandler(string filename_) : filename(filename_) {}

void RegexHandler::load_regex_file() {
    ifstream file(filename);
    string line;

    if (file.is_open()) {

        while (getline(file, line)) {
            rgxs_strings_vector.push_back(line);
        }
        file.close();

    } else {
        cerr << "Unable to open regex file!" << endl; 
    }

    size = rgxs_strings_vector.size();

    for (int i = 0; i < size; i++) {
        rgxs_ptrs_vector.push_back((rgxs_strings_vector[i]).c_str());
    }

    rgxs = rgxs_ptrs_vector.data();
    
}


void RegexHandler::compile_regexes() {
    hs_compile_error_t *compile_err;

    vector<unsigned int> flags(size, HS_FLAG_DOTALL);

    vector<unsigned int> ids(size);
    iota(ids.begin(), ids.end(), 1);

    unsigned int mode = HS_MODE_BLOCK;

    if (hs_compile_multi(rgxs, flags.data(), ids.data(), size, mode, nullptr, 
    &database, &compile_err) != HS_SUCCESS) {
            fprintf(stderr, "ERROR: Unable to compile pattern \"%s\": %s\n",
            rgxs, compile_err->message);
            hs_free_compile_error(compile_err);
    }
}

hs_database_t* RegexHandler::get_database() {
    return database;
}
