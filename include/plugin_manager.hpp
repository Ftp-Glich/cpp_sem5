#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <windows.h>
#include <unordered_map>
#include "plugin_interface.hpp"

template <typename T>
using sptr = std::shared_ptr<T>;

class PluginManager {
public:
    ~PluginManager();
    explicit PluginManager(std::string&& dir);
    void load_plugins();
    double execute(const std::string& op, double a, double b = 0) const;
    bool exists(const std::string& op) const;
    std::vector<sptr<IOperation>> get_op_list() const;
    sptr<IOperation> get_info(const std::string& op) const;
private:
    std::vector<HMODULE> loaded_libs_;
    std::unordered_map<std::string, sptr<IOperation>> plugins_;
    std::string directory;
};
