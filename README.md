# mpsst
Multi Pattern String Search Tool

## Building
Building:\
From the project root run
`rm -rf build && cmake -S . -B build && cmake --build build -j$(nproc)`
or
`cmake -S . -B build && cmake --build build`

# to generate the build files and build inside the `build` directory

# PCRE2

From the project root run 

```bash
git clone https://github.com/PCRE2Project/pcre2.git
cd pcre2/
cmake -B build .
cmake --build build

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
`./test/all_tests.sh`
to run all tests

From the project root run 
`./test/test1.sh`
to run test1
