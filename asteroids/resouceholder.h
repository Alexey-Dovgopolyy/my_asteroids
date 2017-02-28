#ifndef RESOUCEHOLDER_H
#define RESOUCEHOLDER_H

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    void        load(Identifier id, const std::string filename);

    template <typename Parameter>
    void        load(Identifier id, const std::string filename,
                     Parameter param);

    Resource&           get(Identifier id);
    const Resource&     get(Identifier id) const;

private:
    void    insertResources(Identifier id, std::unique_ptr<Resource> resource);

private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;

};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id,
                                    const std::string filename)
{
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename)) {
        throw std::runtime_error("Can not load " + filename);
    }
    insertResources(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id,
            const std::__cxx11::string filename, Parameter param)
{
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename, param)) {
        throw std::runtime_error("Can not load " + filename);
    }
    insertResources(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get
                                                (Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResources
                                (Identifier id,
                                 std::unique_ptr<Resource> resource)
{
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

#endif // RESOUCEHOLDER_H
