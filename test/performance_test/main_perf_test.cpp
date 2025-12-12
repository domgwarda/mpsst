#include <boost/program_options/value_semantic.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <boost/program_options.hpp>

#include "abstract_regex_handler.h"
#include "abstract_dir_scanner.h"
#include "abstract_file_scanner.h"

#include "engine_regex_handler.h"
#include "engine_dir_scanner.h"
#include "engine_file_scanner.h"

#include "pcre_regex_handler.h"
// #include "pcre_file_scanner.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){
    po::options_description desc("REQUIRED options");
    desc.add_options()
        ("regex,r", po::value<std::string>(), "Path to regex file or binary database (-b)")
        ("file,f", po::value<std::string>(), "Path to the root of the files to search")
        ("binDatabase,b", po::value<std::string>(), "Path to regex file compressd to binary format or normal regex file (-r)");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    bool has_any_regex = vm.count("regex") || vm.count("binDatabase");

    if ( !(has_any_regex) || !vm.count("file")) {
        std::cout << "Usage:\n" << desc << "\n";
        return 1;
    }

    std::string bin_regex_path, regex_path, root_path;
    if (vm.count("binDatabase")){
        bin_regex_path = vm["binDatabase"].as<std::string>();
    }

    if (vm.count("regex")){
        regex_path = vm["regex"].as<std::string>();
    }

    root_path  = vm["file"].as<std::string>();

    // EngineRegex engine(Hyperscan);
    Engine selected_engine = Hyperscan; //PCRE2;
    EngineRegex engine(selected_engine);
    AbstractRegexHandler* regex_handler = engine.get_engine();
    
    if(!regex_path.empty()){
        regex_handler->load_regex_file(regex_path); 
        regex_handler->compile_regexes();

        if (!bin_regex_path.empty()) {
            regex_handler->save_regex_database(bin_regex_path);
        }
    } else if (!bin_regex_path.empty()) {
        regex_handler->load_regex_database(bin_regex_path);
    } else {
        cerr<< "Error: You must provide either a regex source file (-r) or a binary database (-b).\n";
        return 1;
    }

    RegexDatabase db_variant = regex_handler->get_database();

    EngineFileScanner engine_file_scanner(selected_engine, db_variant);
    AbstractFileScanner* fscanner = engine_file_scanner.get_engine();

    EngineDirScanner engine_dir_scanner(selected_engine, fscanner);
    AbstractDirScanner* scanner = engine_dir_scanner.get_engine();

    

////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int iterations = 5;
    std::vector<double> times(iterations);

    for(int i = 0; i < iterations; i++)
    {
        auto start = std::chrono::steady_clock::now();

        scanner->scan(root_path);

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();  //seconds or microseconds
        times[i] = duration;
    }
    double mean = 0;
    for(int i = 0; i < iterations; i++)
    {
        if(i > 1)   //discard first two runs (warm-ups)
            mean += times[i];
        std::cout << i+1 << ": " << times[i] << "s" << std::endl;

    }
    std::cout << "mean = " << mean/(iterations-2) << std::endl;

    return 0;
}