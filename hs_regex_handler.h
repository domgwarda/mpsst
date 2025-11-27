/*
RegexHandler Class
- Manages regexes loaded from a file and compiles them into a Hyperscan database,

Methods:
- RegexHandler(const string filename):
    Constructs a RegexHandler with a constant filename,

- ~RegexHandler():
    Frees the compiled Hyperscan database if allocated.

- void load_regex_file():
    Loads regular expressions from the file specified by `filename`.
    Stores them as `const char* const` type,

- void compile_regexes():
    Compiles all loaded regexes into a Hyperscan database.
    Stores the compiled database,

- hs_database_t* get_database():
    Returns the compiled Hyperscan database.
    Must be called only after `precompile_regexes()` has been executed successfully.

- void scan_file(const std::string &path):
    Scans an entire file using the Hyperscan Streaming API.
    Reads the file in fixed-size chunks (e.g. 1 MB), and feeds each
    chunk into hs_scan_stream().
    Uses a match callback that prints matches in the format:
        path:from:to:id=<pattern_id>
    Allocates and frees its own hs_scratch_t workspace.
    Safe to call concurrently from multiple threads (each scan allocates
    its own scratch).

- debug_scan_literal
    Diagnostic helper used to test Hyperscan on a single
    in-memory byte buffer.

Notes:
- Scratch spaces (hs_scratch_t*) are NOT thread-safe and must not be shared.
  scan_file() automatically creates its own scratch for each scan,
  which makes concurrent file scanning safe.

  Added experimentation on hs_scan for smaller files to test performance
*/

#pragma once

#include <vector>
#include <string>
#include <hs/hs.h>

#include "abstract_regex_handler.h"

using namespace std;

class HSRegexHandler : public AbstractRegexHandler {
    private: 
        vector<const char*> rgxs_ptrs_vector;
        const char* const* rgxs;
        hs_database_t *database = nullptr;
    public:
        int size;
        vector<string> rgxs_strings_vector;

        // block scan threshold
        static constexpr std::size_t BLOCK_THRESHOLD = 4 * 1024 * 1024; // 4 MB

        HSRegexHandler();
        ~HSRegexHandler();

        hs_database_t* get_database() override;
        void load_regex_file(const string& filename) override;
        void load_regex_database(const string& filename) override; 
        void save_regex_database(const string& filename) override;
        void compile_regexes() override;
        void scan_file(const string &path);
        void debug_scan_literal();
};      

