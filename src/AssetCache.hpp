#ifndef ASSETCACHE_HPP
#define ASSETCACHE_HPP

#include <memory>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <spine/spine-sfml.h>

class AssetMissingException : public std::exception {
public:
    AssetMissingException(const std::string& assetFile) :
            m_message("Cannot load file: " + assetFile) {}

    const char *what() const noexcept {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

/* AssetCache loads and stores assets in a centralized location. */
class AssetCache {
public:
    AssetCache(const std::string& workingDirectory);

    AssetCache(const AssetCache&) = delete;

    ~AssetCache();

    /* get___ methods will return the requested asset.
     * If the asset has not yet been loaded in memory, it will be loaded, stored,
     * and then returned. If the asset has already been loaded, it will just be
     * returned.
     * Throws:
     *  - AssetMissingException: The asset cannot be found in the working directory */
    std::shared_ptr<sf::Texture> getTexture(const std::string&);

    std::shared_ptr<sf::Font> getFont(const std::string&);

    std::shared_ptr<spine::Atlas> getAtlas(const std::string&);

    std::shared_ptr<spine::SkeletonData> getSkeleton(const std::string& skeletonPath, const std::string& atlasPath);

private:
    std::string m_workingDirectory;
    spine::SFMLTextureLoader m_spineTextureLoader;

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> m_fonts;
    std::unordered_map<std::string, std::shared_ptr<spine::Atlas>> m_atlases;
    std::unordered_map<std::string, std::shared_ptr<spine::SkeletonData>> m_skeletons;
};

#endif // ASSETCACHE_HPP
