#pragma once

#include <stack>
#include <queue>
#include <memory>
#include <vector>
#include <string>
#include <cstdint>

#include "plugin_manager.hpp"

enum tokenType{
    NUMBER,
    OPERATOR,
    LPAREN, RPAREN,
    FUNCTION, 
};

class Calculator
{
public:
    Calculator() = delete;
    explicit Calculator(std::shared_ptr<PluginManager> pm);
    double calculate(std::vector<std::pair<PluginInfo, tokenType>>&& tokens);
    ~Calculator() = default;
private:    
    std::queue<std::pair<std::string, bool>> rpm_;
    std::shared_ptr<PluginManager> pm_;
};
