#pragma once

#include <string>

class AbstractFileScanner; // forward

class DirectoryScannerParallel {
public:
    explicit DirectoryScannerParallel(AbstractFileScanner &handler, unsigned workers = 0);
    ~DirectoryScannerParallel() = default;

    // Recursively scan 'root' using a pool of workers that call handler.scan_file()
    // This call blocks until all work is done.
    void scan(const std::string &root);

private:
    AbstractFileScanner &handler_;
    unsigned num_workers_;
};
