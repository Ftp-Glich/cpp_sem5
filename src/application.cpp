#include "application.hpp"
#include "plugin_interface.hpp"
#include <numbers>

Application::Application(std::string&& plugin_dir):
    prev_result_(std::nullopt) {
        pm_ = std::make_shared<PluginManager>(std::move(plugin_dir));
        pm_->load_plugins();
        calculator_ = std::make_unique<Calculator>(pm_);
        supported_ops_ = pm_->get_op_list();
        constants_.insert({"pi", std::numbers::pi});
        constants_.insert({"e", std::numbers::e});
    }

void Application::run(std::istream& istream) {
    std::string input;
    std::getline(istream, input);
    while(input != "exit") {
        if ( input.size() == 0) {
            std::getline(istream, input);
            continue;
        }
        if (input == "help") {
            std::cout << "Commands:\n type exit to quit\n type flush to reset previos calculation result\n type operations to\
             get supported operations\n give a math example to calculate value\n    !!! Use '.' symbol to use floating point nums and $ before math constants" << std::endl; 
        } else if (input == "flush") {
            prev_result_ = std::nullopt;
        } else if (input == "operations") {
            std::cout << "operations: ";
            for(auto& op : supported_ops_) {
                std::cout << op->name() << " ";
            }
            std::cout << std::endl;
            std::cout << "constants: ";
            for(auto& constant: constants_) {
                std::cout << constant.first << " ";
            }
            std::cout << std::endl;
        } else {
            std::vector<Token> tokens;
            tokens.reserve(input.size());
            if (prev_result_.has_value()) {
                tokens.push_back({tokenType::NUMBER, nullptr, prev_result_.value()});
                prev_result_ = std::nullopt;
            }
            try {
                tokenize(input, tokens);
                prev_result_ = calculator_->calculate(tokens);
                std::cout << prev_result_.value() << " "; 
            } catch(std::runtime_error& e) {
                std::cerr << "Failed to parse math example: " << e.what() << "\nFix issues and try again: \n" << std::endl;
            } catch(std::logic_error& e) {
                std::cerr << "Failed to calculate value: " << e.what() << "\nFix issues and try again: " << std::endl;
            }
        }
        std::getline(istream, input);
    }
}

double Application::test(std::string&& input) {
    std::vector<Token> tokens;
    tokenize(input, tokens);
    auto res = calculator_->calculate(tokens);
    return res;
}

void Application::tokenize(std::string& input, std::vector<Token>& tokens) {
    std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c){ return std::tolower(c); });
    std::string token;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        
        if (std::isspace(static_cast<unsigned char>(c))) continue;

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            token.clear();
            bool dotUsed = (c == '.');
            token += c;
            while (i + 1 < input.size() && (std::isdigit(static_cast<unsigned char>(input[i + 1])) || (!dotUsed && input[i + 1] == '.'))) {
                if (input[i + 1] == '.') dotUsed = true;
                token += input[++i];
            }
            tokens.push_back({tokenType::NUMBER, nullptr, std::stod(token)});
        } else if (c == '(') {
            tokens.push_back({tokenType::LPAREN});
        } else if (c == ')') {
            tokens.push_back({tokenType::RPAREN});
        } else if (c == '+' || c == '-') {
            bool unary = tokens.empty()
                         || tokens.back().type == tokenType::OPERATOR
                         || tokens.back().type == tokenType::LPAREN
                         || tokens.back().type == tokenType::COMMA;
            std::string op = unary ? std::string("u") + c : std::string(1, c); 
            tokens.push_back({tokenType::OPERATOR, nullptr, 0, std::move(op)});
        } else if (c == '*' || c == '/') {
            std::string op(1, c);
            tokens.push_back({tokenType::OPERATOR, nullptr, 0, std::move(op)});
        } else if (c == '$') {
            token.clear();
            while (std::isalpha(static_cast<unsigned char>(input[i + 1]))) {
                token += input[++i];
            }
            auto it = constants_.find(token);
            if(it != constants_.end()) {
                tokens.push_back({tokenType::NUMBER, nullptr, it->second});
            } else {
                throw std::runtime_error("Unknown math constant");
            }
        } else if (c == ',') {
            tokens.push_back({tokenType::COMMA});
        } else {
            token.clear();
            token = c;
            bool found = false;
            while (i < input.size() && !std::isspace(static_cast<unsigned char>(input[i]))) {
                auto it = std::find_if(supported_ops_.begin(), supported_ops_.end(), [token](sptr<IOperation> info) { return info->name() == token; });
                if(it != supported_ops_.end()) {
                    auto type = (*it)->is_operator ? tokenType::OPERATOR : tokenType::FUNCTION;
                    tokens.push_back({type, *it});
                    found = true;
                    break;
                }
                token += input[++i];
            }
            if(!found) {
                throw std::runtime_error("Unsupported operations founded");
            }
        }
    }
}


