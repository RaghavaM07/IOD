#include "PluginLoader.h"

template <class T>
PluginLoader<T>::PluginLoader(const std::string& sPluginsPath, const std::string& id) {
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

            PluginInit_t<T> createFn = dlsym(handle, "create_instance");
            if(!createFn)
                continue;

            initMap[std::string(idFn())] = createFn;
        }
    }
}

template <class T>
PluginLoader<T>::~PluginLoader() {}

template <class T>
T* PluginLoader<T>::makePlugin(std::string name) {
    if(initMap.find(name) == initMap.end())
        return nullptr;

    return initMap[name]();
}
