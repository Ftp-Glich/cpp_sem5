#include "plugin_interface.hpp"
#include "math.h"


extern "C" {

__declspec(dllexport) const PluginInfo get_operation_info() {
    return {"sin", 1, false, false};
}

__declspec(dllexport) double operate(double a, double b = 0) {
    return sin(a);
}

}