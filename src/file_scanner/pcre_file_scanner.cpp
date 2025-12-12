#include "pcre_file_scanner.h"
#include "abstract_file_scanner.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <pcre2.h>
#include <variant>
#include <vector>

PCREFileScanner::PCREFileScanner(RegexDatabase database_variant_) 
    : AbstractFileScanner(database_variant_) {
    
    compiled_regexs = std::get_if<std::vector<pcre2_code*>>(&this->database_variant);

    if (!compiled_regexs) {
        throw std::runtime_error("PCREFileScanner: Invalid database type provided!");
    }
}

std::string PCREFileScanner::read_file_content(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return "";
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void PCREFileScanner:: scan_file(const std::string &path){
    if (!compiled_regexs || compiled_regexs->empty()) {
        std::cerr << "Warning: No rules to scan with." << std::endl;
        return;
    };

    std::string content = read_file_content(path);
    if (content.empty()){
        std::cerr<<"File is empty or could not read"<< std::endl;
        return;
    };

    // conversion of ptr types 
    PCRE2_SPTR subject = reinterpret_cast<PCRE2_SPTR>(content.c_str());
    PCRE2_SIZE subject_size = content.length();

    for (pcre2_code* re : *compiled_regexs){
        if (!re) continue;

        pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, NULL);
        PCRE2_SIZE start_offset = 0;

        while (true){
            int rc = pcre2_match(
                re,          // what are we searching
                subject,     // where 
                subject_size,// how many bites
                start_offset,           // start offset
                0,
                match_data, // storage for results
                NULL
            );

            if (rc < 0) {
                    break;
                }

            PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
            std::cout << "Pattern found in " << path 
                        << " at offset: " << ovector[0] 
                        << " to " << ovector[1] << std::endl;

            start_offset = ovector[1];
            if (ovector[0] == ovector[1]) {
                if (start_offset < subject_size) {
                    start_offset++;
                } else {
                    break; 
                }
            }
        }
        pcre2_match_data_free(match_data);
    }
    
}

