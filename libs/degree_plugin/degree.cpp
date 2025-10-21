#include "plugin_interface.hpp"
#include "math.h"


extern "C" {

__declspec(dllexport) const PluginInfo get_operation_info() {
    return {"*", false, true};
}

__declspec(dllexport) double operate(double a, double b) {
    return pow(a, b);
}
}