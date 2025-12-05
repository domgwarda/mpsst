#include "pcre_regex_handler.h"

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
#include <memory>


using namespace std;

PCRERegexHandler::PCRERegexHandler() {}

PCRERegexHandler::~PCRERegexHandler() {
    if (auto vector_db_ptr = std::get_if<std::vector<pcre2_code*>>(&database)) {
        for (auto re : *vector_db_ptr) {
            pcre2_code_free(re);
        }
        vector_db_ptr->clear();
    }
    database = vector<pcre2_code*>{};
}


void PCRERegexHandler::load_regex_file(const string& filename) {
    ifstream file(filename);
    string line;

    rgxs_strings_vector.clear();

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
}

void PCRERegexHandler::compile_regexes() {
    if (rgxs_strings_vector.empty()) {
        cerr << "No regexes to compile\n";
        return;
    }

    auto compiled_rgxs = make_shared<vector<pcre2_code*>>();

    int error_number;
    PCRE2_SIZE error_offset;

    for (const auto& pattern : rgxs_strings_vector) {
        pcre2_code* re = pcre2_compile(
            (PCRE2_SPTR)pattern.c_str(),
            PCRE2_ZERO_TERMINATED,
            0,
            &error_number,
            &error_offset,
            NULL
        );

        if (!re) {
            cerr << "Invalid pattern: " << pattern << endl;
            for (auto r : *compiled_rgxs) pcre2_code_free(r);
            return;
        }

        compiled_rgxs->push_back(re);
    }

    // Bezpośrednie przypisanie do variant z dynamicznego wektora
    database = *compiled_rgxs;
}



void PCRERegexHandler::load_regex_database(const std::string& filename){
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

    int errorcode;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(buffer.data());
    int32_t number_of_codes = pcre2_serialize_get_number_of_codes(bytes);
    vector<pcre2_code*> compiled_rgxs;

    errorcode = pcre2_serialize_decode(compiled_rgxs.data(), number_of_codes, bytes, NULL);
    if (errorcode < 0) {
        cerr << "Error deserializing regex database\n";
        return;
    }

    database = compiled_rgxs;
}

void PCRERegexHandler::save_regex_database(const string& filename){

    vector<pcre2_code*> compiled_rgxs;

    if (auto vector_db_ptr = std::get_if<std::vector<pcre2_code*>>(&database)) {
        for (auto re : *vector_db_ptr) {
            compiled_rgxs.push_back(re);
        }
    }

    int errorcode;
    uint8_t *bytes;
    PCRE2_SIZE erroroffset;
    PCRE2_SIZE bytescount;

    vector<const pcre2_code_8*> const_rgxs(compiled_rgxs.begin(), compiled_rgxs.end());

    errorcode = pcre2_serialize_encode(
        const_rgxs.data(),
        const_rgxs.size(),
        &bytes,
        &bytescount,
        NULL
    );

    if (errorcode != 0) {
        cerr << "Error serializing regex database\n";
        return;
    }


    ofstream file(filename,ios::binary);
    if (file.is_open()){
        file.write(reinterpret_cast<char*>(bytes), bytescount);
        file.close();
        cout<<"Database saved to: "<<filename<<"\n";
    } else {
        cerr<<"Unable to open file:"<<filename<<"\n";
    }

    pcre2_serialize_free(bytes);
}

RegexDatabase PCRERegexHandler::get_database() {
    return database;
}


vector<string> PCRERegexHandler::get_regexs_vector() {
    return rgxs_strings_vector;
};

int PCRERegexHandler::get_regexs_vector_size() {
    return size;
};