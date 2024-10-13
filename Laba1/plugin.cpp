#include <iostream>
#include <windows.h>
#include <map>
#include <string>
#include <functional>
#include <stdexcept>
#include <filesystem>
#include "plugin.h"

void PluginLoader::loadPlugins() {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            std::string pluginPath = entry.path().string();

            HMODULE hModule = LoadLibraryA(pluginPath.c_str());
            if (!hModule) {
                std::cerr << "Failed to load plugin: " << pluginPath << std::endl;
                continue;
            }

            auto getFunctionName = (const char* (*)()) GetProcAddress(hModule, "getFunctionName");
            auto calculate = (double (*)(double)) GetProcAddress(hModule, "calculate");

            if (!getFunctionName || !calculate) {
                FreeLibrary(hModule);
                std::cerr << "Invalid plugin format: " << pluginPath << std::endl;
                continue;
            }

            std::string functionName = getFunctionName();
            functions[functionName] = { hModule, calculate };
            std::cout << "Loaded plugin: " << functionName << " from " << pluginPath << std::endl;
        }
    }
}

std::function<double(double)> PluginLoader::getFunction(const std::string& name) {
    if (functions.find(name) == functions.end()) {
        throw std::runtime_error("Function not found: " + name); return nullptr;
    }
    return functions[name].function;
}