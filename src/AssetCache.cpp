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

std::shared_ptr<spine::Atlas> AssetCache::getAtlas(const std::string& path){
    // Return asset if already loaded
    auto itAtlas = m_atlases.find(path);
    if (itAtlas != m_atlases.end()) {
        return itAtlas->second;
    }

    // Construct full path with working directory
    auto assetPath = m_workingDirectory + path;
    m_atlases[path] = std::make_shared<spine::Atlas>(assetPath.c_str(), &m_spineTextureLoader);

    // AssetMissingException is also thrown when the atlas exists but is empty
    if (m_atlases[path]->getPages().size() == 0) {
        m_atlases.erase(path);
        throw AssetMissingException(assetPath);
    }

    return m_atlases[path];
}

std::shared_ptr<spine::SkeletonData> AssetCache::getSkeleton(const std::string& skeletonPath, const std::string& atlasPath) {
    // Return asset if already loaded
    auto itSkeleton = m_skeletons.find(skeletonPath + atlasPath);
    if (itSkeleton != m_skeletons.end()) {
        return itSkeleton->second;
    }

    auto atlas = getAtlas(atlasPath);

    // Construct full path with working directory
    auto skeletonAssetPath = m_workingDirectory + skeletonPath;
    spine::SkeletonBinary binary(atlas.get());

    auto skeletonData = binary.readSkeletonDataFile(skeletonAssetPath.c_str());
    if (!skeletonData) {
        throw AssetMissingException(skeletonAssetPath);
    }

    m_skeletons[skeletonPath] = std::shared_ptr<spine::SkeletonData>(skeletonData);
    return m_skeletons[skeletonPath];
}