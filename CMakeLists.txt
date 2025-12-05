cmake_minimum_required(VERSION 3.10)
project(mpsst CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#cmake_policy(SET CMP0167 OLD)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Boost REQUIRED COMPONENTS program_options)

# Hyperscan from subtree
add_subdirectory(third_party/hyperscan_mini) 


add_executable(main
    main.cpp
    hs_regex_handler.cpp
    hs_dir_scanner.cpp
    hs_file_scanner.cpp
)


target_link_libraries(main
    PRIVATE
        hs                  
        Boost::program_options
)


target_include_directories(main
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/hyperscan_mini/include
)