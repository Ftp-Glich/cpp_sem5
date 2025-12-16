#include "../include/engine.hpp"
#include <iostream>

struct Subject {
    double f3(int a, int b) { return static_cast<double>(a) / b; }
};

int main() {
    Subject subj;
    Wrapper wrapper(&subj, &Subject::f3, std::vector<std::pair<std::string, int>>{{"arg1",0}, {"arg2", 1}});

    Engine engine;
    engine.register_command(std::move(wrapper), "command1");
    auto res = engine.execute("command1", {{"arg1",2}, {"arg2", 3}});
    std::cout << std::any_cast<double>(res) << std::endl;
}