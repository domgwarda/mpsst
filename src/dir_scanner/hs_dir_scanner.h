/*
DirectoryScanner Class
----------------------
Recursively scans a directory tree and uses a provided RegexHandler
to scan every file encountered.


- DirectoryScanner(RegexHandler &handler):
    Constructs the DirectoryScanner with a reference to an already
    initialized RegexHandler. The handler must remain alive for the
    lifetime of the DirectoryScanner.

- void scan(const std::string &root):
    If 'root' is a file, scans it directly.
    If 'root' is a directory, recursively traverses all subdirectories
    and scans every regular file found.
    Prints filesystem errors but continues where possible.
*/


#pragma once
#include <string>
#include "abstract_dir_scanner.h"

class HSDirScanner : public AbstractDirScanner {
public:
    explicit HSDirScanner(AbstractFileScanner &handler);
    void scan(const std::string &root);
};
