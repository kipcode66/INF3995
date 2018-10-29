#include "FileCache.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>

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
    :m_path(""), m_isInitialized(false)
{
    try {
        m_path = fs::temp_directory_path();
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
    auto cacheIt = fs::directory_iterator(m_path);
    return std::distance(fs::begin(cacheIt), fs::end(cacheIt));
}

size_t FileCache::cacheSize() const {
    auto cacheIt = fs::directory_iterator(m_path);
    std::vector<uintmax_t> sizes;
    std::transform(fs::begin(cacheIt), fs::end(cacheIt), std::back_inserter(sizes), [](const fs::directory_entry& dirEntry){
        try {
            return fs::file_size(dirEntry);
        } catch (fs::filesystem_error& e) {
            return (uintmax_t)0;
        }
    });
    return (size_t)std::accumulate(sizes.cbegin(), sizes.cend(), (uintmax_t)0);
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
        if (fs::exists(m_path)) {
            for (auto& entry : fs::directory_iterator(m_path)) {
                fs::remove_all(entry.path());
            }
            m_isInitialized = true;
        }
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
