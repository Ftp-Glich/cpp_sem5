#pragma once

#include <stack>
#include <memory>
#include <vector>
#include <string>

#include "plugin_manager.hpp"

class Calculator
{
public:
    Calculator() = delete;
    explicit Calculator(std::shared_ptr<PluginManager> pm);
    double calculate(std::vector<std::string>&& tokens);
    ~Calculator() = default;
private:
    std::stack<std::string> op_stack;
    std::stack<double> num_stack;
    std::shared_ptr<PluginManager> pm_;
};
