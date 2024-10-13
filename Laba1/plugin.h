#pragma once

struct PluginFunction {
    HMODULE libraryHandle;
    std::function<double(double)> function;
};

class PluginLoader {
    std::string directory;
    std::map<std::string, PluginFunction> functions;
public:
    PluginLoader(const std::string& pluginsDirectory) : directory(pluginsDirectory) {}

    ~PluginLoader() {
        for (auto& pair : functions)
            FreeLibrary(pair.second.libraryHandle);
    }

    void loadPlugins();
    std::function<double(double)> getFunction(const std::string& name);
};