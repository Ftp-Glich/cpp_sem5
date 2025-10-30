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
            unsigned int arity = 2;
            if (token.operation) {
                arity = token.operation->arity;
            } else {
                if (token.sign == "u-" || token.sign == "u+") arity = 1;
            }
        
            if (st.size() < arity) {
                throw std::logic_error("invalid operator usage");
            }
        
            if (arity == 1) {
                double v = st.top().val; st.pop();
                double res;
                if (token.operation) {
                    res = token.operation->operate(v, 0);
                } else {
                    if (token.sign == "u-") res = -v;
                    else res = v;
                }
                st.push({tokenType::NUMBER, nullptr, res, ""});
            } else {
                double right = st.top().val; st.pop();
                double left  = st.top().val; st.pop();
                if (token.operation) {
                    try {
                        st.push({tokenType::NUMBER, nullptr, token.operation->operate(left, right), ""});
                    } catch(std::exception& e) {
                        throw std::logic_error(std::string("failed to evaluate expression - ") + e.what());
                    }
                } else {
                    if (token.sign == "+") st.push({tokenType::NUMBER, nullptr, left + right, ""});
                    else if (token.sign == "-") st.push({tokenType::NUMBER, nullptr, left - right, ""});
                    else if (token.sign == "*") st.push({tokenType::NUMBER, nullptr, left * right, ""});
                    else if (token.sign == "/") {
                        if (right == 0) throw std::logic_error("division by zero");
                        st.push({tokenType::NUMBER, nullptr, left / right, ""});
                    } else throw std::logic_error("Unknown token");
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
            if (st.size() < n) {
                throw std::logic_error("invalid function usage");
            }
            for (int i = 0; i < n; ++i) {
                args.push_back(st.top().val);
                st.pop();
            }
            std::reverse(args.begin(), args.end()); 

            double res = 0;
            try {
                if (n == 1) {
                    res = token.operation->operate(args[0], 0); 
                } else if (n == 2) {
                    res = token.operation->operate(args[0], args[1]);
                } else {
                    throw std::logic_error("Unsupported arity");
                }
             } catch( std::exception& e) {
                throw std::logic_error(std::string("failed to evaluate expression - ") + std::string(e.what()));
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

int precedence_by_sign(const std::string& s) {
    if (s == "u-" || s == "u+") return 3;     
    if (s == "^") return 4;                   
    if (s == "*" || s == "/") return 2;
    if (s == "+" || s == "-") return 1;
    return 0;
}

bool Calculator::shouldPop(const Token& top, const Token& current) {
    if (top.type != tokenType::OPERATOR)
        return false;

    int prec_top = top.operation ? top.operation->precedence : precedence_by_sign(top.sign);
    int prec_cur = current.operation ? current.operation->precedence : precedence_by_sign(current.sign);

    bool right_assoc = (current.operation && current.operation->right_associative)
                      || current.sign == "^"
                      || current.sign == "u-"  
                      || current.sign == "u+";

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



