#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "regex_handler.h"

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
    
    std::string regex_path;
    std::string root_path;

    if (vm.count("regex")) {
        regex_path = vm["regex"].as<std::string>();
    } else {
        std::cout << "Regex path is missing \n";
        std::cout << desc <<"\n";
        return 2;
    }

    if (vm.count("file")) {
        root_path = vm["file"].as<std::string>();
    } else {
        std::cout << "Root path is missing \n" ;
        std::cout << desc <<"\n";
        return 3;
    }

    RegexHandler regex_handler(regex_path);
    regex_handler.load_regex_file();
    regex_handler.compile_regexes();

    return 0;
};