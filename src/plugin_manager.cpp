#include "plugin_manager.hpp"
#include <filesystem>
#include <iostream>


PluginManager::PluginManager(std::string&& dir): directory(std::move(dir)) { }

void PluginManager::load_plugins() {
    using namespace std::filesystem;

    for (auto& entry : directory_iterator(directory)) {
        if (entry.path().extension() != ".dll")
            continue;

        HMODULE lib = LoadLibraryA(entry.path().string().c_str());
        if (!lib) {
            std::cerr << "Failed to load " << entry.path() << "\n";
            continue;
        }

        auto create = (IOperation* (*)())GetProcAddress(lib, "create_plugin");
        auto destroy = (void (*)(IOperation*))GetProcAddress(lib, "destroy_plugin");

        if (!create || !destroy) {
            std::cerr << "Invalid plugin: " << entry.path() << "\n";
            FreeLibrary(lib);
            continue;
        }

        auto instance = sptr<IOperation>(create());
        plugins_[instance->name()] = { lib, instance };
        std::cout << "Loaded plugin: " << instance->name() << "\n";
    }
}

double PluginManager::execute(const std::string& op, double a, double b) const{
    auto it = plugins_.find(op);
    if (it != plugins_.end()) {
        return it->second.op->operate(a, b);
    }
}

bool PluginManager::exists(const std::string& op) const{
    return plugins_.find(op) != plugins_.end();
}
 
std::vector<sptr<IOperation>> PluginManager::get_op_list() const{
    std::vector<sptr<IOperation>> res;
    res.reserve(plugins_.size());
    std::transform(plugins_.begin(), plugins_.end(), std::back_inserter(res),
        [](const auto& key_value_pair) { return key_value_pair.second.op; });
    return std::move(res);
}

sptr<IOperation> PluginManager::get_info(const std::string& op) const{
    auto it = plugins_.find(op);
    if (it != plugins_.end()) {
        return it->second.op;
    }
    throw std::runtime_error("Have no loaded plugin " + op);
}

PluginManager::~PluginManager() {
    for (auto& [name, p] : plugins_) {
        auto destroy = (void (*)(IOperation*))GetProcAddress(p.handle, "destroy_plugin");
        if (destroy) destroy(p.op.get());
        FreeLibrary(p.handle);
    }
}