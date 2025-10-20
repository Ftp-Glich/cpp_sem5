#include "calculator.hpp"

Calculator::Calculator(std::shared_ptr<PluginManager> pm):
 pm_(pm) {}

double Calculator::calculate(std::vector<std::string>&& tokens) {
    return pm_->execute("+", 2, 2);
}