#include "FileCache.hpp"
#include <iostream>

namespace elevation {

namespace fs = std::experimental::filesystem;

FileCache::FileCache(const std::string& cachePath)
    :m_path(cachePath), m_isInitialized(false)
{
    ensureCacheDirCreated_();
}

FileCache::FileCache(const std::experimental::filesystem::path& cachePath)
    :m_path(cachePath), m_isInitialized(false)
{
    ensureCacheDirCreated_();
}

FileCache::FileCache()
    :m_path("")
{
    try {
        auto tmpDir = fs::temp_directory_path();
        ensureCacheDirCreated_();
    }
    catch (fs::filesystem_error& e) {
        std::clog << "An error occured while creating a temporary directory "
                     "for the cache." << std::endl;
        std::clog << e.what();
    }
}

FileCache::~FileCache() {
    wipeCachedFiles_();
    m_isInitialized = false;
}

bool FileCache::isInitialized() {
    return m_isInitialized;
}

size_t FileCache::fileCount() const {
    return 0;
}

size_t FileCache::cacheSize() const {
    return 0;
}

std::vector<fs::path> FileCache::getFileList() const {
    return std::vector<std::experimental::filesystem::path>();
}

void FileCache::setFileContent(const std::string& fileName, const std::string& data) {
}

void FileCache::deleteFile(const std::string& fileName) {
}

bool FileCache::isFileCached(const std::string& fileName) {
    return false;
}

fs::directory_entry FileCache::getFile(const std::string& fileName) const {
}

void FileCache::ensureCacheDirCreated_() noexcept {
    try {
        fs::create_directories(m_path);
        m_isInitialized = fs::exists(m_path);
    }
    catch (fs::filesystem_error& e) {
        std::clog << e.what();
    }
}

void FileCache::wipeCachedFiles_() noexcept {
    try {
        fs::remove_all(m_path);
    }
    catch (fs::filesystem_error& e) {
        std::clog << e.what();
    }
}

} // namespace elevation
