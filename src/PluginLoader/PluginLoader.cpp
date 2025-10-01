#include "PluginLoader.h"

template <class T>
PluginLoader<T>::PluginLoader() { }

template <class T>
size_t PluginLoader<T>::init(const std::string& sPluginsPath, const std::string& id) {
    this->id = std::string(id);

    if(!fs::exists(sPluginsPath) || !fs::exists(sPluginsPath) || !fs::is_directory(sPluginsPath))
        pluginsPath = fs::path(fs::current_path().append("plugins/"));
    else 
        pluginsPath = fs::path(sPluginsPath);

    // discover all plugins and build pluginInit map
    for(const fs::directory_entry& file: fs::directory_iterator(pluginsPath)) {
        std::error_code err;
        if(!file.is_regular_file(err)) continue;
        if(file.path().extension() == ".so") {
            // match with corresponding id
            void *handle = dlopen(file.path().c_str(), RTLD_NOW);
            if(!handle)
                continue;

            IdFn_t idFn = (IdFn_t) dlsym(handle, "plugin_interface_id");
            if(!idFn || strcmp(idFn(), id.c_str())!=0) {
                dlclose(handle);
                continue;
            }

            PluginInit_t<T> createFn = (PluginInit_t<T>) dlsym(handle, "create_instance");
            if(!createFn)
                continue;

            initMap[std::string(idFn())] = createFn;
        }
    }

    return initMap.size();
}

template <class T>
PluginLoader<T>::~PluginLoader() {
    delete INSTANCE;
}

template <class T>
PluginLoader<T>* PluginLoader<T>::instance() {
    if(INSTANCE == nullptr) {
        return INSTANCE = new PluginLoader<T>();
    }
    return INSTANCE;
}

template <class T>
T* PluginLoader<T>::makePlugin(std::string name) {
    if(initMap.find(name) == initMap.end())
        return nullptr;

    return initMap[name]();
}
