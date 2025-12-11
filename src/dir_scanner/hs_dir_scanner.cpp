#include "hs_dir_scanner.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

HSDirScanner::HSDirScanner(AbstractFileScanner &handler) : AbstractDirScanner(handler) {}

void HSDirScanner::scan(const std::string &root) {
    std::error_code ec;
    fs::path p(root);

    if (fs::is_regular_file(p, ec)) {
        handler_.scan_file(p.string());
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
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Filesystem error while scanning " << root << ": " << e.what() << "\n";
    }
}
