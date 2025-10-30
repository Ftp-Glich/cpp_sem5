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
    FUNCTION, COMMA
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
    bool shouldPop(const Token& top, const Token& current);
    std::deque<Token> shunting_yard(const std::vector<Token>& tokens);
    sptr<PluginManager> pm_;
};
