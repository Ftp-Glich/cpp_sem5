#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <windows.h>
#include <unordered_map>
#include "plugin_interface.hpp"

struct Plugin {
    HMODULE handle;
    double (*operate)(double, double);
    PluginInfo info;
};

class PluginManager {
public:
    explicit PluginManager(std::string&& dir);
    void load_plugins();
    double execute(const std::string& op, double a, double b = 0) const;
    bool exists(const std::string& op) const;
    std::vector<PluginInfo> get_op_list() const;
    PluginInfo get_info(const std::string& op) const;
private:
    std::unordered_map<std::string, Plugin> plugins_;
    std::string directory;
};
