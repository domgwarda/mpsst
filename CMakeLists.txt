cmake_minimum_required(VERSION 3.10)
project(mpsst CXX)
set(CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0167 OLD)

find_package(PkgConfig REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(Boost REQUIRED COMPONENTS program_options)
pkg_check_modules(PC_HYPERSCAN REQUIRED IMPORTED_TARGET libhs)

include_directories(/usr/local/include)
add_executable(main
    main.cpp
    hs_regex_handler.cpp
    directory_scanner.cpp
)

target_link_libraries(main 
    PkgConfig::PC_HYPERSCAN
    Boost::program_options
)
