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

struct Token {
    tokenType type;
    sptr<IOperation> operation;
    double val;
    std::string sign;
};

class Calculator
{
public:
    Calculator() = delete;
    explicit Calculator(sptr<PluginManager> pm);
    double calculate(std::vector<Token>& tokens);
    ~Calculator() = default;
private:    
    bool shouldPop(Token top, Token cur);
    std::queue<Token> shunting_yard(const std::vector<Token>& tokens);
    sptr<PluginManager> pm_;
};
