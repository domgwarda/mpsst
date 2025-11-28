#include "directory_scanner.h"
#include <filesystem>
#include <iostream>

#include <chrono>

namespace fs = std::filesystem;

DirectoryScanner::DirectoryScanner(AbstractFileScanner &handler) : handler_(handler) {}

void DirectoryScanner::scan(const std::string &root) {
    using clock = std::chrono::steady_clock;
    auto t_start = clock::now();
    
    std::error_code ec;
    fs::path p(root);
    std::uint64_t files_processed = 0;
    
    if (fs::is_regular_file(p, ec)) {
        handler_.scan_file(p.string());
        files_processed = 1;
        auto t_end = clock::now();
        std::chrono::duration<double> elapsed = t_end - t_start;
        std::cerr << "[timing] scanned_files=" << files_processed
                  << " elapsed=" << elapsed.count() << "s\n";
        return;
    }

    if (!fs::is_directory(p, ec)) {
        std::cerr << "Path is neither file nor directory: " << root << " (" << ec.message() << ")\n";
        return;
    }

    try {
        for (auto &entry : fs::recursive_directory_iterator(p)) {
            if (!entry.is_regular_file()) continue;
            handler_.scan_file(entry.path().string());
            ++files_processed;
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Filesystem error while scanning " << root << ": " << e.what() << "\n";
    }
    
    auto t_end = clock::now();
    std::chrono::duration<double> elapsed = t_end - t_start;
    std::cerr << "[timing] scanned_files=" << files_processed
              << " elapsed=" << elapsed.count() << "s"
              << " avg_time_per_file=" 
              << ((files_processed > 0) ? (elapsed.count() / static_cast<double>(files_processed)) : 0.0)
              << "s\n";
}
