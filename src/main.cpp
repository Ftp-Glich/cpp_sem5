#include "application.hpp"

#include <iostream>

int main() {
    Application app("plugins");
    app.run(std::cin);
    return 0;
}