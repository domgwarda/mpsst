#include "hs_regex_handler.h"

#include <cstddef>
#include <fstream>
#include <hs/hs_common.h>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <numeric>
#include <cstdio>
#include <cstring> // strdup
#include <cstdlib> // free


using namespace std;

HSRegexHandler::HSRegexHandler() {}

HSRegexHandler::~HSRegexHandler() {
    if (database) {
        hs_free_database(database);
        database = nullptr;
    }
}

void HSRegexHandler::load_regex_file(const string& filename) {
    ifstream file(filename);
    string line;

    rgxs_strings_vector.clear();
    rgxs_ptrs_vector.clear();
    rgxs = nullptr;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.empty()) continue;
            rgxs_strings_vector.push_back(line);
        }
        file.close();
    } else {
        cerr << "Unable to open regex file: " << filename << endl;
    }

    size = static_cast<int>(rgxs_strings_vector.size());

    for (int i = 0; i < size; ++i) {
        rgxs_ptrs_vector.push_back(rgxs_strings_vector[i].c_str());
    }

    if (!rgxs_ptrs_vector.empty()) {
        rgxs = rgxs_ptrs_vector.data();
    } else {
        rgxs = nullptr;
    }
}

void HSRegexHandler::compile_regexes() {
    if (size == 0 || rgxs == nullptr) {
        cerr << "No regexes to compile\n";
        return;
    }

    hs_compile_error_t *compile_err = nullptr;

    vector<unsigned int> flags(size, HS_FLAG_DOTALL);
    vector<unsigned int> ids(size);
    iota(ids.begin(), ids.end(), 1);

    unsigned int mode = HS_MODE_STREAM;

    if (hs_compile_multi(rgxs, flags.data(), ids.data(), size, mode, nullptr,
                         &database, &compile_err) != HS_SUCCESS) {
        if (compile_err && compile_err->message) {
            fprintf(stderr, "ERROR: Unable to compile pattern: %s\n", compile_err->message);
            hs_free_compile_error(compile_err);
        } else {
            fprintf(stderr, "ERROR: Unable to compile patterns.\n");
        }
        database = nullptr;
    } else {
        cout << "Compiled " << size << " regex(es) into Hyperscan database\n";
    }
}

void HSRegexHandler::load_regex_database(const std::string& filename){
    ifstream file(filename, std::ios::binary|std::ios::ate);

    if (!file.good()){
        cerr<<"Unabele to open binary regex file: "<<filename<<"\n";
        return;
    }

    streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size == 0) {
        cerr << "Error: File " << filename << " is empty!\n";
        return;
    }

    std::vector<char> buffer(size);

    if(!file.read(buffer.data(), size)){
        cerr<<"Error while reading from file\n";
        return;
    }

    hs_error_t err = hs_deserialize_database(buffer.data(), size, &database);

    if (err!=HS_SUCCESS){
        cerr<<"Error deserializing database from binary file. Error code: "<<err<<"\n";
        database=nullptr;
        return;
    } else {
        cout<<"Successfully loaded compiled database from " << filename << "\n";
    }
}

void HSRegexHandler::save_regex_database(const string& filename){
    if (database==nullptr) {
        cerr<<"No database to save, compile regexs first\n";
    }

    char *bytes = nullptr;
    size_t length = 0;

    hs_error_t err = hs_serialize_database(database, &bytes, &length);

    if (err!=HS_SUCCESS){
        cerr<<"Error serializing database. Error code: "<<err<<"\n";
    } 

    ofstream file(filename,ios::binary);
    if (file.is_open()){
        file.write(bytes, length);
        file.close();
        cout<<"Database saved to: "<<filename<<"\n";
    } else {
        cerr<<"Unable to open file:"<<filename<<"\n";
    }

    free(bytes);
}

hs_database_t* HSRegexHandler::get_database() {
    return database;
}


vector<string> HSRegexHandler::get_regexs_vector() {
    return rgxs_strings_vector;
};

int HSRegexHandler::get_regexs_vector_size() {
    return size;
};