#include "regex_handler.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <cstdio>
#include <cstdint>
#include <filesystem>
#include <cstring> // strdup
#include <cstdlib> // free


using namespace std;
namespace fs = std::filesystem;

RegexHandler::RegexHandler(string filename_) : filename(filename_) {}
RegexHandler::~RegexHandler() {
    if (database) {
        hs_free_database(database);
        database = nullptr;
    }
}

void RegexHandler::load_regex_file() {
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

void RegexHandler::compile_regexes() {
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

hs_database_t* RegexHandler::get_database() {
    return database;
}

static int on_match(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx) {
    const char* path = static_cast<const char*>(ctx);
    printf("%s:%llu:%llu:id=%u\n", path, from, to, id);
    fprintf(stderr, "[on_match] %s %llu..%llu id=%u\n", path, from, to, id);
    return 0; 
}

void RegexHandler::scan_file(const std::string &path) {
    if (!database) {
        std::cerr << "Database not compiled. Call compile_regexes() first.\n";
        return;
    }
    
    hs_scratch_t *scratch = nullptr;    // hs_scratch_t in Hyperscan is a temporary, per-thread workspace required for scanning

    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        std::cerr << "Failed to allocate scratch space\n";
        return;
    }

    hs_stream_t *stream = nullptr;
    if (hs_open_stream(database, 0, &stream) != HS_SUCCESS) {
        std::cerr << "Failed to open Hyperscan stream\n";
        hs_free_scratch(scratch);
        return;
    }

    const size_t BUFSIZE = 1 << 20;
    std::vector<char> buf(BUFSIZE);

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open file for streaming scan: " << path << "\n";
        hs_close_stream(stream, scratch, nullptr, nullptr);
        hs_free_scratch(scratch);
        return;
    }

    char *ctx_path = strdup(path.c_str());

    while (in) {
        in.read(buf.data(), static_cast<std::streamsize>(buf.size()));
        std::streamsize readn = in.gcount();
        std::cerr << "[scan_file] read bytes: " << readn << " from file " << path << std::endl;

        if (readn <= 0) break;

        hs_error_t rv = hs_scan_stream(stream,
                               buf.data(),
                               static_cast<unsigned int>(readn),
                               0,
                               scratch,
                               on_match,
                               (void*)ctx_path);

        if (rv != HS_SUCCESS) {
            std::cerr << "hs_scan_stream failed with code " << rv << " for file " << path << "\n";
            break;
        }
    }

    hs_error_t close_rv = hs_close_stream(stream, scratch, nullptr, nullptr);

    std::cerr << "[scan_file] hs_close_stream rv=" << close_rv << " for file " << path << std::endl;

    free(ctx_path);

    hs_free_scratch(scratch);
    in.close();
}

void RegexHandler::debug_scan_literal() {
    if (!database) {
        std::cerr << "[debug] Database not compiled.\n";
        return;
    }

    const char *data = "xxx\naaa\nyyy\naba\ncc\n";
    const unsigned int len = static_cast<unsigned int>(strlen(data));
    const char *ctx_literal = "debug_literal";

    hs_scratch_t *scratch_block = nullptr;
    if (hs_alloc_scratch(database, &scratch_block) != HS_SUCCESS) {
        std::cerr << "[debug] Failed to allocate scratch for block\n";
    } else {
        hs_error_t rv_block = hs_scan(database, data, len, 0, scratch_block, on_match, (void*)ctx_literal);
        std::cerr << "[debug] hs_scan (block) rv=" << rv_block << " for literal (" << len << " bytes)\n";
        hs_free_scratch(scratch_block);
    }

    hs_scratch_t *scratch = nullptr;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        std::cerr << "[debug] Failed to allocate scratch for stream\n";
        return;
    }

    hs_stream_t *stream = nullptr;
    if (hs_open_stream(database, 0, &stream) != HS_SUCCESS) {
        std::cerr << "[debug] Failed to open stream\n";
        hs_free_scratch(scratch);
        return;
    }

    hs_error_t rv_stream_scan = hs_scan_stream(stream, data, len, 0, scratch, on_match, (void*)ctx_literal);
    std::cerr << "[debug] hs_scan_stream (with on_match) rv=" << rv_stream_scan << " for literal (" << len << " bytes)\n";

    hs_error_t close_rv = hs_close_stream(stream, scratch, nullptr, nullptr);
    std::cerr << "[debug] hs_close_stream rv=" << close_rv << "\n";

    hs_free_scratch(scratch);
}

