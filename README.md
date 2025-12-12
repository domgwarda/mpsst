# MPSST (Multi-Pattern String Search Tool)

## Overview
MPSST is a high-performance regular expression scanning tool designed to search through files efficiently. It supports multiple regex engines, including Hyperscan and PCRE2, providing an abstraction layer to switch between them easily.

## Build Instructions

### 1. Build PCRE2 Dependency
Before building the main project, you must build the PCRE2 library.

```bash
git clone https://github.com/PCRE2Project/pcre2.git
cd pcre2/
cmake -B build .
cmake --build build
cd ..
```

### 2. Build MPSST
You can build the project using CMake from the project root.

Option A: Clean Build (Recommended)
```bash
rm -rf build
cmake -S . -B build
cmake --build build -j$(nproc)
```

Option B:
```bash
# From the project root
cmake -S . -B build
cmake --build build
```
to generate the build files and build inside the `build` directory

# Run mpsst


## Arguments:
```
REQUIRED options:
  -r [ --regex ] arg       Path to regex file or binary database (-b)
  -f [ --file ] arg        Path to the root of the files to search
  -b [ --binDatabase ] arg Path to regex file compressd to binary format or 
                           normal regex file (-r)
```
For example to scan files from regex file and save compiled HS database run: `./build/main -b regexTest.bin -f test_tree -r regexTest.rgx`

## Test file
From the project root run 
`base64 /dev/urandom | head -c 2GB > test2gb.txt`
to create test file (size 2GB) 

# Run tests
From the project root run 
`./test/unit_test/all_tests.sh`
to run all tests

From the project root run 
`./test/unit_test/test1.sh`
to run test1 (.rgx regex file)

From the project root run
`./test/unit_test/test2.sh`
to run test2 (binary regex file)

## Performance test
From the project root run
`./test/performance_test/test.sh`
to run test

# Docs

### Engine Abstraction
The project uses an abstraction layer to support multiple regex engines.

#### Engine Enum
Defines the supported backends:
- Hyperscan
- PCRE2

#### EngineRegex Class
Acts as a wrapper to manage "Regex Handlers" for the specific engine implementation.

- Attributes:
    - `RegexDatabase regex_handler`: Pointer to the active `AbstractRegexHandler` instance.
- Methods:
    - `EngineRegex(enum Engine eng)`: Factory constructor that initializes the appropriate handler (HS or PCRE2).
    - `get_engine()`: Returns the underlying `AbstractRegexHandler*`.

#### RegexDatabase (Type Alias)
Defines a variant type capable of holding either a Hyperscan database or a vector of PCRE2 codes.
`variant<hs_database_t*, vector<pcre2_code*>>`

### Regex Handlers
Manages the lifecycle of regular expressions.

- Classes: AbstractRegexHandler (Base), HSRegexHandler, PCRERegexHandler.
- Key Methods:
  - `void load_regex_file(string filename)`: Loads patterns from the specified file into memory.
  - `void compile_regexes()`: Compiles loaded patterns into an engine-specific database.
  - `void load_regex_database(const string& filename)`: Loads compiled database from the specified file.
  - `void save_regex_database(const string& filename)`: Saves compiled database into file.
  - `RegexDatabase get_database()`: Returns the compiled database variant (  `variant<hs_database_t*, vector<pcre2_code*>>`  ).

#### EngineDirScanner Class
Acts as a wrapper to manage "Directory Scanners" for the specific engine implementation.

- Attributes:
    - `AbstractDirScanner* regex_dir_scanner`: Pointer to the active `AbstractDirScanner` instance.
- Methods:
    - `EngineDirScanner(enum Engine eng, AbstractFileScanner* file_scanner)`: Factory constructor that initializes the appropriate directory scanner (HS or PCRE2).
    - `AbstractDirScanner* get_engine()`: Returns the underlying `AbstractDirScanner*`.

### Directory Scanners

TODO

#### EngineFileScanner Class
Acts as a wrapper to manage "File Scanners" for the specific engine implementation.

- Attributes:
    - `AbstractFileScanner* regex_file_scanner`: Pointer to the active `AbstractFileScanner*` instance.
- Methods:
    - `EngineFileScanner(enum Engine eng, RegexDatabase database_variant)`: Factory constructor that initializes the appropriate file scanner (HS or PCRE2).
    - `AbstractFileScanner* get_engine()`: Returns the underlying `AbstractFileScanner*`.


### File Scanners

TODO