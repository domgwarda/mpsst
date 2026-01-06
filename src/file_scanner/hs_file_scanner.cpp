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
#include <cstring>
#include <cstdlib>
#include "debug_log.h"

#include <hs/hs.h>


#include "hs_file_scanner.h"


using namespace std;

HSFileScanner::HSFileScanner(RegexDatabase db_variant): AbstractFileScanner(db_variant) {        
    if (auto hs_db_ptr = get_if<hs_database_t*>(&db_variant)) {
        database = *hs_db_ptr;
    } else {
        cerr << "Error HS: Cannot get database"  << endl;
    }    
};

HSFileScanner::~HSFileScanner() {
    // if (database) {
        // hs_free_database(database);
        // database = nullptr;
    // }
};

static int on_match(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx) {
    const char* path = static_cast<const char*>(ctx);
    printf("%s:%llu:%llu:id=%u\n", path, from, to, id);
    DEBUG("[on_match] %s %llu..%llu id=%u",
         path, from, to, id);
    return 0; 
}

void HSFileScanner::scan_file(const string &path) {
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
        DEBUG("[scan_file] read bytes: %lld from file %s",
            (long long)readn, path.c_str());

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

    DEBUG("[scan_file] hs_close_stream rv=%d for file %s",
        (int)close_rv, path.c_str());

    free(ctx_path);

    hs_free_scratch(scratch);
    in.close();
}

// void HSFileScanner::debug_scan_literal(hs_database_t *database) {
//     if (!database) {
//         std::cerr << "[debug] Database not compiled.\n";
//         return;
//     }

//     const char *data = "xxx\naaa\nyyy\naba\ncc\n";
//     const unsigned int len = static_cast<unsigned int>(strlen(data));
//     const char *ctx_literal = "debug_literal";

//     hs_scratch_t *scratch_block = nullptr;
//     if (hs_alloc_scratch(database, &scratch_block) != HS_SUCCESS) {
//         std::cerr << "[debug] Failed to allocate scratch for block\n";
//     } else {
//         hs_error_t rv_block = hs_scan(database, data, len, 0, scratch_block, on_match, (void*)ctx_literal);
//         std::cerr << "[debug] hs_scan (block) rv=" << rv_block << " for literal (" << len << " bytes)\n";
//         hs_free_scratch(scratch_block);
//     }

//     hs_scratch_t *scratch = nullptr;
//     if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
//         std::cerr << "[debug] Failed to allocate scratch for stream\n";
//         return;
//     }

//     hs_stream_t *stream = nullptr;
//     if (hs_open_stream(database, 0, &stream) != HS_SUCCESS) {
//         std::cerr << "[debug] Failed to open stream\n";
//         hs_free_scratch(scratch);
//         return;
//     }

//     hs_error_t rv_stream_scan = hs_scan_stream(stream, data, len, 0, scratch, on_match, (void*)ctx_literal);
//     std::cerr << "[debug] hs_scan_stream (with on_match) rv=" << rv_stream_scan << " for literal (" << len << " bytes)\n";

//     hs_error_t close_rv = hs_close_stream(stream, scratch, nullptr, nullptr);
//     std::cerr << "[debug] hs_close_stream rv=" << close_rv << "\n";

//     hs_free_scratch(scratch);
// }

