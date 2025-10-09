#include "PluginLoader.h"

namespace IOD
{
    template <class T>
    PluginLoader<T>::PluginLoader() { }

    template <class T>
    size_t PluginLoader<T>::init(const std::vector<std::string>& sPluginPaths, const std::string& id) {
        this->id = std::string(id);

        for(const std::string& sPath: sPluginPaths) {
            fs::path thePath;
            if (!fs::exists(sPath) || !fs::is_directory(sPath))
                thePath = fs::path(fs::current_path().append("plugins/"));
            else
                thePath = fs::path(sPath);

            if(pluginsPath.find(thePath)!=pluginsPath.end()) // duplicate, skip load
                continue;
            else
                pluginsPath.insert(thePath);

            // discover all plugins and build pluginInit map
            for(const fs::directory_entry& file: fs::directory_iterator(thePath)) {
                std::error_code err;
                if(!file.is_regular_file(err)) continue;
                if(file.path().extension() == ".so") {
                    // open & match with corresponding id
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

                    initMap[std::string(createFn()->name())] = createFn;
                }
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
}
