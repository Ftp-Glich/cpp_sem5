#pragma once

extern "C" {

struct PluginInfo
{
    const char* name;
    bool is_single;
    bool right_associative;
};


__declspec(dllexport) const PluginInfo get_operation_info();

__declspec(dllexport) double operate(double a, double b);
}
