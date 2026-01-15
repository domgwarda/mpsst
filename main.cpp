#include <boost/program_options/value_semantic.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <boost/program_options.hpp>

#include "abstract_regex_handler.h"
#include "abstract_file_scanner.h"

#include "dir_scanner.h"

#include "engine_regex_handler.h"
#include "engine_file_scanner.h"


namespace po = boost::program_options;

int main(int argc, char* argv[]){
    po::options_description desc("REQUIRED options");
    desc.add_options()
        ("regex,r", po::value<std::string>(), "Path to regex file or binary database (-b)")
        ("file,f", po::value<std::string>(), "Path to the root of the files to search")
        ("binDatabase,b", po::value<std::string>(), "Path to regex file compressd to binary format or normal regex file (-r)")
        ("engine,e", po::value<std::string>(), "Choose an engine type: hs (Hyperscan) or pcre (PCRE2), if none defult is pcre")
        ("measureTime,t", po::value<int>()->implicit_value(1)->default_value(0), "Number of measured runs, If the -t flag is passed, measure the program's execution time")
        ("warmup,w", po::value<int>()->default_value(0),  "Number of warm-up runs (not measured)")
        ("threads,h", po::value<int>()->default_value(1), "Number of running threads")

    ;

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


   Engine selected_engine;
    if (vm.count("engine")){
        std::string engine_str = vm["engine"].as<std::string>();
        if (engine_str == "hs" || engine_str == "hyperscan") {
            selected_engine = Hyperscan;
        } 
        else if (engine_str == "pcre" || engine_str == "pcre2") {
            selected_engine = PCRE2;
        } 
        else {
            std::cerr << "Error wrong engine" << engine_str << "'.Available: hs, pcre.\n";
            return 1; 
        }
    } else {
        selected_engine = PCRE2; 
    }
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

    // Optional debug / single file test
    //regex_handler.scan_file();
    //regex_handler.debug_scan_literal();
    //vector<string> rgxs_vector = regex_handler->get_regexs_vector();
    //for (int i = 0; i < regex_handler->get_regexs_vector_size(); i++) {
    //    std::cout << rgxs_vector[i] << std::endl;
    //}

    RegexDatabase db_variant = regex_handler->get_database();

    EngineFileScanner engine_file_scanner(selected_engine, db_variant);
    AbstractFileScanner* fscanner = engine_file_scanner.get_engine();

    // fscanner->scan_file(root_path);

    // HSFileScanner fscanner(db_variant);

    DirScanner scanner(*fscanner, vm["threads"].as<int>());

    int warmup = vm["warmup"].as<int>();
    if (warmup < 0)
        warmup = 0;

    int tests = vm["measureTime"].as<int>();
    if (tests < 0)
        tests = 0;
    bool measure = (tests > 0);

    
    if(!measure)
        scanner.scan(root_path);

    else{

        if(warmup > 0){
        
            for(int i=0; i<warmup; i++)
                scanner.scan(root_path);
        }
        
        std::vector<long long> times;
        double avg = 0;
        for(int i=0; i<tests; i++){
            chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            scanner.scan(root_path);
            chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            times.push_back(duration);
            avg += duration;
        }
        
        avg /= tests;

        std::cout << "Times for: engine= " << vm["engine"].as<std::string>() << "; warmup= " << warmup << "; test= " << tests << "; threads= " << vm["threads"].as<int>() <<  ";" <<endl;
        std::cout << "avg= " << avg << " ms" << endl;

    }


    return 0;
}