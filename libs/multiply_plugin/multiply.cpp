#include "plugin_interface.hpp"


extern "C" {

__declspec(dllexport) const PluginInfo get_operation_info() {
    return {false, "*"};
}

__declspec(dllexport) double operate(double a, double b) {
    return a * b;
}

}
