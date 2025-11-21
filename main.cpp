#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "regex_handler.h"
#include "directory_scanner.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){
    po::options_description desc("REQUIRED options");
    desc.add_options()
        ("regex,r", po::value<std::string>(), "Path to regex file")
        ("file,f", po::value<std::string>(), "Path to the root of the files to search");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    if (!vm.count("regex") || !vm.count("file")) {
        std::cout << "Usage:\n" << desc << "\n";
        return 1;
    }

    std::string regex_path = vm["regex"].as<std::string>();
    std::string root_path  = vm["file"].as<std::string>();

    RegexHandler regex_handler(regex_path);
    regex_handler.load_regex_file();
    regex_handler.compile_regexes();

    // Optional debug / single file test
    //regex_handler.scan_file();
    //regex_handler.debug_scan_literal();
    
    DirectoryScanner scanner(regex_handler);
    scanner.scan(root_path);

    return 0;
}
