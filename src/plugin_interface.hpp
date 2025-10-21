#pragma once

extern "C" {

struct PluginInfo
{
    const char* name;
    unsigned int arity;
    bool right_associative;
    bool is_op;
};


__declspec(dllexport) const PluginInfo get_operation_info();

__declspec(dllexport) double operate(double a, double b);
}
