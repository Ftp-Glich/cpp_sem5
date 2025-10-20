#include "application.hpp"

#include <iostream>

int main() {
    PluginManager pm("plugins");
    pm.load_plugins();
    pm.execute("+", 2, 2);
    return 0;
}