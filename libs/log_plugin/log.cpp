#define PLUGIN_EXPORTS
#include "plugin_interface.hpp"
#include <string>
#include <cmath>
#include <exception>

class LogOperation : public IOperation {
public:
    std::string name() const override { return "log"; }
    double operate(double a, double b) const override { 
        if(a <= 0 || b <= 0) {
            std::__throw_logic_error("wrong usage of log");
        }
        return std::log2(a) / std::log2(b); 
    }
};

extern "C" PLUGIN_API IOperation* create_plugin() {
    auto op = new LogOperation();
    op->arity = 2;
    op->is_operator = false;
    op->right_associative = true;
    op->precedence = 4;
    return op;
}

extern "C" PLUGIN_API void destroy_plugin(IOperation* plugin) {
    delete plugin;
}
