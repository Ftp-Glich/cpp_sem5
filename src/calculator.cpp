#include "calculator.hpp"

Calculator::Calculator(std::shared_ptr<PluginManager> pm):
 pm_(pm) {}

double Calculator::calculate(std::vector<std::pair<std::string, bool>>&& tokens) {
    for(auto& token: tokens) {
        if(token)
    }
}
