cmake_minimum_required(VERSION 3.10)
project(mpsst CXX)
set(CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0167 NEW)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
pkg_check_modules(PC_HYPERSCAN REQUIRED IMPORTED_TARGET libhs)

add_executable(main main.cpp)

target_link_libraries(main 
    PkgConfig::PC_HYPERSCAN
    Boost::program_options
)