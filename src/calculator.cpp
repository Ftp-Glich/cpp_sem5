#include "calculator.hpp"
#include <algorithm>
#include <iostream>

Calculator::Calculator(sptr<PluginManager> pm):
 pm_(pm) {}

double Calculator::calculate(std::vector<Token>& tokens) {
    std::queue<Token> rpm;
    rpm = shunting_yard(std::move(tokens));
    auto size = rpm.size();
    return 0;
}

bool Calculator::shouldPop(Token top, Token cur) {
    auto get_prec = [this](Token info) -> int {
        if(info.operation != nullptr && pm_->exists(info.operation->name())) {
            return 2;
        } else if (info.sign == "*" || info.sign == "/") {
            return 1;
        } else {
            return 0;
        }
    };
    return (top.operation != nullptr && top.operation->is_operator) && (get_prec(top) > get_prec(cur) || (get_prec(top) == get_prec(cur) && !(top.operation != nullptr && cur.operation->right_associative)));
}

std::queue<Token> Calculator::shunting_yard(const std::vector<Token>& tokens) {
    std::queue<Token> rpm;
    std::stack<Token> op_stack;
    for(auto& token: tokens) {
        switch (token.type) {
        case tokenType::NUMBER:
            rpm.push(token);
            break;
        case tokenType::FUNCTION:
            op_stack.push(token);
            break;
        case tokenType::OPERATOR:
            while (!op_stack.empty() && shouldPop(op_stack.top(), token)) {
                rpm.push(token);
                op_stack.pop();
            }
            op_stack.push(token);
            break;
        case tokenType::LPAREN:
            op_stack.push(token);
            break;
        case tokenType::RPAREN:
            while (!op_stack.empty() && op_stack.top().sign != "(") {
                rpm.push(op_stack.top());
                op_stack.pop();
            }
            if (!op_stack.empty()) {
                throw std::logic_error("Mismatched parentheses");
            }
            op_stack.pop();
            if (!op_stack.empty() && !op_stack.top().operation->is_operator) {
                rpm.push(op_stack.top());
                op_stack.pop();
            }
        default:
            break;
        }
    }    
    while (!op_stack.empty()) {
        if (op_stack.top().sign == "(" || op_stack.top().sign == ")") {
            throw std::logic_error("Mismatched parentheses");
        }
        rpm.push(op_stack.top());
        op_stack.pop();
    }
    return std::move(rpm);
}



