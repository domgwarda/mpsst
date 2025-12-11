cmake_minimum_required(VERSION 3.10)
project(mpsst CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#cmake_policy(SET CMP0167 OLD)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Boost REQUIRED COMPONENTS program_options)

# Hyperscan from subtree
add_subdirectory(third_party/hyperscan_mini) 
add_subdirectory(pcre2)


add_executable(main
    main.cpp
    src/file_scanner/hs_file_scanner.cpp
    src/dir_scanner/hs_dir_scanner.cpp
    src/regex_handler/hs_regex_handler.cpp
    src/regex_handler/regex_database.cpp
    src/regex_handler/pcre_regex_handler.cpp
)



target_link_libraries(main
    PRIVATE
        hs                  
        Boost::program_options
        pcre2-8

)



target_include_directories(main
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        src/file_scanner
        src/dir_scanner
        src/regex_handler
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/hyperscan_mini/include
)