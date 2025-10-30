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
                    if (right != 0) {
                        st.push({tokenType::NUMBER, nullptr, left / right, ""});
                    } else {
                        throw std::logic_error("division by zero");
                    }
                } else {
                    throw std::logic_error("Unknown token"); 
                }
            }
            break;
        }
        case tokenType::FUNCTION: {
            if (!token.operation)
                throw std::logic_error("Function has no implementation");

            unsigned int n = token.operation->arity;
            if (st.size() < n)
                throw std::logic_error("Not enough arguments for function " + token.operation->name());

            std::vector<double> args;
            args.reserve(n);
            for (int i = 0; i < n; ++i) {
                args.push_back(st.top().val);
                st.pop();
            }
            std::reverse(args.begin(), args.end()); 

            double res = 0;
            if (n == 1) {
                res = token.operation->operate(args[0], 0); 
            } else if (n == 2) {
                res = token.operation->operate(args[0], args[1]);
            } else {
                throw std::logic_error("Unsupported arity");
            }

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

bool Calculator::shouldPop(const Token& top, const Token& current) {
    if (top.type != tokenType::OPERATOR)
        return false;

    int prec_top = top.operation ? top.operation->precedence
                                 : (top.sign == "+" || top.sign == "-" ? 1 :
                                    (top.sign == "*" || top.sign == "/" ? 2 :
                                    (top.sign == "^" ? 3 : 0)));

    int prec_cur = current.operation ? current.operation->precedence
                                     : (current.sign == "+" || current.sign == "-" ? 1 :
                                        (current.sign == "*" || current.sign == "/" ? 2 :
                                        (current.sign == "^" ? 3 : 0)));

    bool right_assoc = (current.operation && current.operation->right_associative)
                        || current.sign == "^";

    return (prec_top > prec_cur) || (prec_top == prec_cur && !right_assoc);
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
            break;
        case tokenType::COMMA: 
            while (!op_stack.empty() && op_stack.top().type != tokenType::LPAREN) {
                rpm.push_back(op_stack.top());
                op_stack.pop();
            }
            if (op_stack.empty()) {
                throw std::logic_error("Misplaced comma or mismatched parentheses");
            }
            break;
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



