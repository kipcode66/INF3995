#include "FileCache.hpp"

namespace elevation {

FileCache::FileCache(const std::string& cachePath)
    :m_path(cachePath)
{
    ensureCacheDirCreated_();
}

FileCache::FileCache(const std::experimental::filesystem::path& cachePath)
    :m_path(cachePath)
{
    ensureCacheDirCreated_();
}

FileCache::~FileCache() {
    wipeCachedFiles_();
}

size_t FileCache::fileCount() const {
}

size_t FileCache::cacheSize() const {
}

std::vector<std::experimental::filesystem::path::path> FileCache::getFileList() const {
}

void FileCache::setFileContent(const std::string& fileName, const std::string& data) {
}

void FileCache::deleteFile(const std::string& fileName) {
}

bool FileCache::isFileCached(const std::string& fileName) {
}

std::experimental::filesystem::directory_entry FileCache::getFile(const std::string& fileName) const {
}

} // namespace elevation
