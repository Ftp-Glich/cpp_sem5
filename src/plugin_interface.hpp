#pragma once

extern "C" {

struct PluginInfo
{
    bool is_func;
    const char* sign;
};


__declspec(dllexport) const PluginInfo get_operation_info();

__declspec(dllexport) double operate(double a, double b);
}
