#include "calculator.hpp"
#include <algorithm>
#include <iostream>

Calculator::Calculator(std::shared_ptr<PluginManager> pm):
 pm_(pm) {}

double Calculator::calculate(std::vector<std::pair<PluginInfo, tokenType>>& tokens) {
    std::queue<std::pair<std::string, bool>> rpm;
    rpm = shunting_yard(std::move(tokens));
    auto size = rpm.size();
    for(int i = 0; i < size; ++i) {
        std::cout << rpm.front().first << " "; 
        rpm.pop();
    }
    return 0;
}

bool Calculator::shouldPop(PluginInfo top, PluginInfo cur) {
    auto get_prec = [this](PluginInfo info) -> int {
        if(pm_->exists(info.name)) {
            return 2;
        } else if (info.name == "*" || info.name == "/") {
            return 1;
        } else {
            return 0;
        }
    };
    return top.is_op && (get_prec(top) > get_prec(cur) || (get_prec(top) == get_prec(cur) && !cur.right_associative));
}

std::queue<std::pair<std::string, bool>> Calculator::shunting_yard(const std::vector<std::pair<PluginInfo, tokenType>>& tokens) {
    std::queue<std::pair<std::string, bool>> rpm;
    std::stack<PluginInfo> op_stack;
    for(auto& token: tokens) {
        switch (token.second) {
        case tokenType::NUMBER:
            rpm.push({token.first.name, true});
            break;
        case tokenType::FUNCTION:
            op_stack.push(token.first);
            break;
        case tokenType::OPERATOR:
            while (!op_stack.empty() && shouldPop(op_stack.top(), token.first)) {
                rpm.push({op_stack.top().name, false});
                op_stack.pop();
            }
            op_stack.push(token.first);
            break;
        case tokenType::LPAREN:
            op_stack.push(token.first);
            break;
        case tokenType::RPAREN:
            while (!op_stack.empty() && op_stack.top().name != "(") {
                rpm.push({op_stack.top().name, false});
                op_stack.pop();
            }
            if (!op_stack.empty()) {
                throw std::logic_error("Mismatched parentheses");
            }
            op_stack.pop();
            if (!op_stack.empty() && !op_stack.top().is_op) {
                rpm.push({op_stack.top().name, false});
                op_stack.pop();
            }
        default:
            break;
        }
    }    
    while (!op_stack.empty()) {
        if (op_stack.top().name == "(" || op_stack.top().name == ")") {
            throw std::logic_error("Mismatched parentheses");
        }
        rpm.push({op_stack.top().name, false});
        op_stack.pop();
    }
    return std::move(rpm);
}



