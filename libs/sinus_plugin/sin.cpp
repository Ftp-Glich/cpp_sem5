#define PLUGIN_EXPORTS
#include "plugin_interface.hpp"
#include <string>
#include <cmath>

class SinOperation : public IOperation {
public:
    std::string name() const override { return "sin"; }
    double operate(double a, double b = 0) const override { return sin(a); }
};

extern "C" PLUGIN_API IOperation* create_plugin() {
    auto op = new SinOperation();
    op->arity = 1;
    op->is_operator = false;
    op->right_associative = false;
    return op;
}

extern "C" PLUGIN_API void destroy_plugin(IOperation* plugin) {
    delete plugin;
}
