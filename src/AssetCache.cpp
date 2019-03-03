#include "AssetCache.hpp"
#include <algorithm>

AssetCache::AssetCache(const std::string& workingDirectory) : m_workingDirectory(workingDirectory) {
    // Ensure path separator character is '/' and the working directory
    // ends in '/'
    std::replace(m_workingDirectory.begin(), m_workingDirectory.end(), '\\', '/');
    if (m_workingDirectory.back() != '/') {
        m_workingDirectory += '/';
    }
}

AssetCache::~AssetCache() {
}

std::shared_ptr<sf::Texture> AssetCache::getTexture(const std::string& path) {
    // Return asset if already loaded
    auto itTexture = m_textures.find(path);
    if (itTexture != m_textures.end()) {
        return itTexture->second;
    }

    // Construct full path with working directory
    auto assetPath = m_workingDirectory + path;
    m_textures[path] = std::make_shared<sf::Texture>();
    if (!m_textures[path]->loadFromFile(assetPath)) {
        m_textures.erase(path);
        throw AssetMissingException(assetPath);
    }

    return m_textures[path];
}

std::shared_ptr<sf::Font> AssetCache::getFont(const std::string& path) {
    // Return asset if already loaded
    auto itFont = m_fonts.find(path);
    if (itFont != m_fonts.end()) {
        return itFont->second;
    }

    // Construct full path with working directory
    auto assetPath = m_workingDirectory + path;
    m_fonts[path] = std::make_shared<sf::Font>();
    if (!m_fonts[path]->loadFromFile(assetPath)) {
        m_fonts.erase(path);
        throw AssetMissingException(assetPath);
    }

    return m_fonts[path];
}
