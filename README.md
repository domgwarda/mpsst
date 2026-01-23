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

## Build options

### Enable debug logging (cerr)
Enables internal debug (non-error) messages printed to `cerr`.

```bash
cmake -S . -B build -DDEBUG_LOG=ON
```
### Disable regex match output (stdout):
Disables printing of regex match results to stdout (useful for benchmarks).

```bash
cmake -S . -B build -DNO_OUTPUT=ON
```

# Run mpsst


## Arguments:
```
REQUIRED options:
  -r [ --regex ] arg       Path to regex file or binary database (-b)
  -f [ --file ] arg        Path to the root of the files to search

  -b [ --binDatabase ] arg Path to regex file compressd to binary format or ormal regex file (-r)

  -t [ --measureTime ]     If the -t flag is passed, measure the program's execution time

  -e [ --engine ] arg      Choose an engine type: hs (Hyperscan) or pcre, if none defult is pcre

  -t [ --measureTime ]     Number of measured runs, If the -t flag is passed, measure the program's execution time
  
  -w [ --warmup ]          Number of warm-up runs (not measured), if none default is 0

  -h [ --threads ]         Number of running threads, if none default is 0
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

### DirScanner 

Directory scanner responsible for filesystem traversal and file dispatching.

DirScanner processes a root path that may represent either a file or a directory.

For directories, it goes through all files in each subfolder and distributes them to worker threads for parallel scanning. Each worker invokes the configured file scanner on the files it receives.

The traversal follows a depth-first search (DFS) order.

* **Attributes:**

  * `AbstractFileScanner& handler_`: Reference to the file scanner used to process individual files.
  * `int threads_`: Number of worker threads used for parallel file scanning.

* **Methods:**

  * `void scan(const std::string &root)`: Scans a single file or recursively scans a directory, ensuring that all regular files are passed to the file scanner.
  * `DirScanner(AbstractFileScanner& handler, int threads)`: Initializes the directory scanner with a file scanner instance and the number of worker threads to use.
  * `void worker(TQueue<std::string>& queue, AbstractFileScanner& handler)`: Continuously retrieves file paths from the queue and calls scan_file on the file scanner.

#### EngineFileScanner Class
Acts as a wrapper to manage "File Scanners" for the specific engine implementation.

- Attributes:
    - `AbstractFileScanner* regex_file_scanner`: Pointer to the active `AbstractFileScanner*` instance.
- Methods:
    - `EngineFileScanner(enum Engine eng, RegexDatabase database_variant)`: Factory constructor that initializes the appropriate file scanner (HS or PCRE2).
    - `AbstractFileScanner* get_engine()`: Returns the underlying `AbstractFileScanner*`.


### File Scanners

### HSFileScanner 

Scans the contents of a single file against a compiled Hyperscan database and reports all pattern matches.

HSFileScanner extracts a Hyperscan database from the generic `RegexDatabase` variant and uses it to scan files.

It relies on Hyperscan’s streaming API, allowing large files to be scanned incrementally.
Hyperscan uses a compiled automaton to scan input data incrementally in chunks, rather than evaluating patterns one-by-one.

The engine is designed to match many regular expressions simultaneously.

* **Attributes:**

  * `hs_database_t* database`: Pointer to the compiled regex database used for HS file scanning.
  * `RegexDatabase database_variant (inherited)`: Container holding regex database.

* **Methods:**

  * `HSFileScanner(RegexDatabase db_variant)`: Constructs the scanner and initializes it with a compiled regex database.
  * `~HSFileScanner()`: Destroys the file scanner instance.
  * `void scan_file(const std::string &path)`: Opens the specified file and scans its contents using the Hyperscan engine, emitting detected matches.
  * `static int on_match(...)`: Hyperscan match callback used internally to report pattern matches during scanning.

# Tests

## Test 4
Test4 checks how HS/PCRE handle regular expressions (6), simulating sensitive data detection in 10MB files.

## Test 6
Test4 checks how HS/PCRE handle simple regular expression detection in many (100,000) small files (10 KB).