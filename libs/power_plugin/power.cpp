#define PLUGIN_EXPORTS
#include "plugin_interface.hpp"
#include <string>
#include <cmath>

class PowOperation : public IOperation {
public:
    std::string name() const override { return "^"; }
    double operate(double a, double b) const override { return pow(a, b); }
};

extern "C" PLUGIN_API IOperation* create_plugin() {
    auto op = new PowOperation();
    op->arity = 2;
    op->is_operator = true;
    op->right_associative = true;
    op->precedence = 3;
    return op;
}

extern "C" PLUGIN_API void destroy_plugin(IOperation* plugin) {
    delete plugin;
}
