#include "plugin_manager.hpp"
#include "plugin_interface.hpp"
#include <filesystem>
#include <iostream>


PluginManager::PluginManager(std::string&& dir): directory(std::move(dir)) { }

void PluginManager::load_plugins() {
    namespace fs = std::filesystem;
    auto plugin_dir = fs::current_path().string() + "\\" + directory;
    for (auto& entry : fs::directory_iterator(plugin_dir)) {
        if (entry.path().extension() == ".dll") {
            HMODULE lib = LoadLibraryA(entry.path().string().c_str());
            if (!lib) {
                std::cerr << "Failed to load: " << entry.path() << "\n";
                continue;
            }

            auto get_info = (PluginInfo (*)())GetProcAddress(lib, "get_operation_info");
            auto op_func  = (double (*)(double, double))GetProcAddress(lib, "operate");

            if (!get_info || !op_func) {
                std::cerr << "Invalid plugin: " << entry.path() << "\n";
                FreeLibrary(lib);
                continue;
            }

            std::string op_name = get_info().sign;
            plugins_[op_name] = {lib, op_name, op_func, get_info().is_func};
            std::cout << "Loaded plugin: " << op_name << "\n";
        }
    }
}

double PluginManager::execute(const std::string& op, double a, double b) {
    auto it = plugins_.find(op);
    if (it != plugins_.end()) {
        return plugins_[op].operate(a, b);
    }
    throw std::runtime_error("Unknown operation: " + op);
}

bool PluginManager::is_func(const std::string& op) {
    auto it = plugins_.find(op);
    if (it != plugins_.end()) {
        return plugins_[op].is_func;
    }
    throw std::runtime_error("Unknown operation" + op);
}

std::vector<std::string> PluginManager::get_op_list() {
    std::vector<std::string> res;
    res.reserve(plugins_.size());
    std::transform(plugins_.begin(), plugins_.end(), std::back_inserter(res),
        [](const auto& key_value_pair) { return key_value_pair.first; });
    return std::move(res);
}