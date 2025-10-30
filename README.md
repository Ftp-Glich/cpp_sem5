## cpp calculator app repo

# Build
``` bash
    git clone gtest # clone to project source directory 
    mkdir build && mkdir plugins
    cmake --build build --configure Debug
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