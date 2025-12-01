## cpp calculator app repo

# Build
``` bash
    cmake -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_STANDARD=23  -DCMAKE_C_COMPILER=clang -DCMAKE_C_STANDARD=11 -DCMAKE_CXX_STANDARD_REQUIRED=ON -DCMAKE_C_STANDARD_REQUIRED=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-g -fstandalone-debug -fno-limit-debug-info" -B build -S .
    mkdir plugins
    cmake --build build --config Debug
```

# Run
``` bash 
    ./build/src/calculator.exe # run application, type help to know how to use the app
    ./build/tests/calculator_tests.exe # run tests
```

# Plugins exporting
    1. you need to inherit from IOperation class from plugin_interface.hpp to add new plugin
    2. you need to compile plugins using the same compiler as you use to compile main app

# My toolchain
    Windows11 + GNU runtime(cmake, clang, ninja, ldd, lldb) from mingw package