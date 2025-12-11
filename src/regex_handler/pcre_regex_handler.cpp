#include "pcre_regex_handler.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

PCRERegexHandler::PCRERegexHandler() {
    database = vector<pcre2_code*>{};
}

PCRERegexHandler::~PCRERegexHandler() {
    if (vector<pcre2_code*>* vector_db_ptr = get_if<vector<pcre2_code*>>(&database)) {
        for (pcre2_code* re : *vector_db_ptr) {
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
        cerr << "Error: unable to open regex file: " << filename << endl;
    }

    size = rgxs_strings_vector.size();
    file.close();
}

void PCRERegexHandler::compile_regexes() {
    if (rgxs_strings_vector.empty()) {
        cerr << "Error: no regexes to compile." << endl;
        return;
    }

    int error_number;
    PCRE2_SIZE error_offset;

    for (string pattern_string : rgxs_strings_vector) {
        PCRE2_SPTR pattern = (PCRE2_SPTR)pattern_string.c_str();
        pcre2_code* re = pcre2_compile(
            pattern,
            PCRE2_ZERO_TERMINATED,
            0,
            &error_number,
            &error_offset,
            NULL
        );

        if (!re) {
            cerr << "Error: invalid pattern: " << pattern_string << endl;
            return;
        }
        vector<pcre2_code*>* db = get_if<vector<pcre2_code*>>(&database);
        if (!db) {
            cerr << "Error: database does not hold vector<pcre2_code*>." << endl;
        return; 
        }

        db->push_back(re);
    }
}

void PCRERegexHandler::load_regex_database(const std::string& filename){
    ifstream file(filename, std::ios::binary|std::ios::ate);

    if (!file.good()){
        cerr<< "Error: unabele to open binary regex file: " << filename << endl;
        return;
    }

    streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size == 0) {
        cerr << "Error: File " << filename << " is empty!" << endl;
        return;
    }

    std::vector<char> buffer(size);

    if(!file.read(buffer.data(), size)){
        cerr << "Error while reading from file" << endl;
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
    vector<pcre2_code*> *db = get_if<vector<pcre2_code*>>(&database);
    vector<const pcre2_code*> const_db(db->begin(), db->end());

    int errorcode;
    uint8_t *bytes;
    PCRE2_SIZE erroroffset;
    PCRE2_SIZE bytescount;

    errorcode = pcre2_serialize_encode(
        const_db.data(),
        const_db.size(),
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