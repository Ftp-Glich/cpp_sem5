#include "calculator.hpp"
#include <algorithm>

Calculator::Calculator(std::shared_ptr<PluginManager> pm):
 pm_(pm) {}

double Calculator::calculate(std::vector<std::pair<PluginInfo, tokenType>>&& tokens) {
    
}



