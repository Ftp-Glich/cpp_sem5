#include "calculator.hpp"
#include <algorithm>
#include <iostream>

Calculator::Calculator(sptr<PluginManager> pm):
 pm_(pm) {}

double Calculator::calculate(std::vector<Token>& tokens) {
    std::deque<Token> rpm;
    rpm = shunting_yard(std::move(tokens));
    std::stack<Token> st;
    for(auto& token: rpm) {
        switch (token.type) {
        case tokenType::NUMBER:
            st.push(token);
            break; 
        case tokenType::OPERATOR: {
            auto right = st.top().val;
            st.pop();
            auto left = st.top().val;
            st.pop();
            if(token.operation != nullptr) {
                st.push({tokenType::NUMBER, nullptr, token.operation->operate(left, right), ""});
            } else {
                if (token.sign == "+") {
                    st.push({tokenType::NUMBER, nullptr, left + right, ""});
                } else if (token.sign == "-") {
                    st.push({tokenType::NUMBER, nullptr, left - right, ""});
                } else if (token.sign == "*") {
                    st.push({tokenType::NUMBER, nullptr, left * right, ""});
                } else if (token.sign == "/") {
                    st.push({tokenType::NUMBER, nullptr, left / right, ""});
                } else {
                    throw std::logic_error("Unknown token"); 
                }
            }
            break;
        }
        case tokenType::FUNCTION: {
            auto arg = st.top().val;
            st.pop();
            auto res = token.operation->operate(arg);
            st.push({tokenType::NUMBER, nullptr, res, ""});
            break;
        }
        default:
            throw std::logic_error("Unknown token"); 
            break;
        }
    }
    return st.top().val;
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

std::deque<Token> Calculator::shunting_yard(const std::vector<Token>& tokens) {
    std::deque<Token> rpm;
    std::stack<Token> op_stack;
    for(auto& token: tokens) {
        switch (token.type) {
        case tokenType::NUMBER:
            rpm.push_back(token);
            break;
        case tokenType::FUNCTION:
            op_stack.push(token);
            break;
        case tokenType::OPERATOR:
            while (!op_stack.empty() && shouldPop(op_stack.top(), token)) {
                rpm.push_back(op_stack.top());
                op_stack.pop();
            }
            op_stack.push(token);
            break;
        case tokenType::LPAREN:
            op_stack.push(token);
            break;
        case tokenType::RPAREN:
            while (!op_stack.empty() && op_stack.top().type != tokenType::LPAREN) {
                rpm.push_back(op_stack.top());
                op_stack.pop();
            }
            if (op_stack.empty()) {
                throw std::logic_error("Mismatched parentheses");
            }
            op_stack.pop();
            if (!op_stack.empty() && op_stack.top().type == tokenType::FUNCTION) {
                rpm.push_back(op_stack.top());
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
        rpm.push_back(op_stack.top());
        op_stack.pop();
    }
    return std::move(rpm);
}



