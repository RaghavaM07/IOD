#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include <dlfcn.h>
#include <cstring>

template <class T>
using PluginInit_t = T* (*)();

using IdFn_t = const char* (*)();

namespace fs = std::filesystem;

template <class T>
class PluginLoader
{
private:
    std::unordered_map<std::string, PluginInit_t<T>> initMap;
    fs::path pluginsPath;
    std::string id;

public:
    PluginLoader(const std::string& pluginsPath, const std::string& id);
    ~PluginLoader();

    T* makePlugin(std::string name);
};