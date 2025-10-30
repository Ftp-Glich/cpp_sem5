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
        sptr<IOperation> instance(
            create(),
            [destroy, lib](IOperation* p) {
                if (p) destroy(p);
            }
        );
        plugins_[instance->name()] = instance;
        loaded_libs_.push_back(lib);
    }
}

double PluginManager::execute(const std::string& op, double a, double b) const{
    auto it = plugins_.find(op);
    if (it != plugins_.end()) {
        return it->second->operate(a, b);
    }
}

bool PluginManager::exists(const std::string& op) const{
    return plugins_.find(op) != plugins_.end();
}
 
std::vector<sptr<IOperation>> PluginManager::get_op_list() const{
    std::vector<sptr<IOperation>> res;
    res.reserve(plugins_.size());
    std::transform(plugins_.begin(), plugins_.end(), std::back_inserter(res),
        [](const auto& key_value_pair) { return key_value_pair.second; });
    return std::move(res);
}

sptr<IOperation> PluginManager::get_info(const std::string& op) const{
    auto it = plugins_.find(op);
    if (it != plugins_.end()) {
        return it->second;
    }
    throw std::runtime_error("Have no loaded plugin " + op);
}

PluginManager::~PluginManager() {
    plugins_.clear();
    for (auto& lib: loaded_libs_) {
        FreeLibrary(lib);
    }
}