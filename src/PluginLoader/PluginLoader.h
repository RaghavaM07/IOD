#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include <dlfcn.h>
#include <cstring>
#include <unordered_set>

#include "IRankingStrategy.h"

template <class T>
using PluginInit_t = T* (*)();

using IdFn_t = const char* (*)();

namespace fs = std::filesystem;

template <class T>
class PluginLoader
{
private:
    std::unordered_map<std::string, PluginInit_t<T>> initMap;
    std::unordered_set<fs::path> pluginsPath;
    std::string id;

    static PluginLoader<T> *INSTANCE;

    PluginLoader();

public:
    size_t init(const std::vector<std::string> &pluginPaths, const std::string &id);
    static PluginLoader<T> *instance();
    T* makePlugin(std::string name);
    ~PluginLoader();
};

template <class T>
PluginLoader<T>* PluginLoader<T>::INSTANCE = nullptr;

template class PluginLoader<IOD::IRankingStrategy>;
