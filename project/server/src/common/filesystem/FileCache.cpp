#include "FileCache.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>

namespace elevation {

namespace fs = std::experimental::filesystem;

typedef fs::perms perms;

const perms FileCache::FILE_PERMISSIONS = perms::add_perms |
    perms::owner_read  | perms::owner_write |
    perms::group_read  | perms::group_write |
    perms::others_read | perms::others_write;

FileCache::FileCache(const std::string& cachePath)
    : m_path(cachePath)
{
    ensureCacheDirCreated_();
}

FileCache::FileCache(const std::experimental::filesystem::path& cachePath)
    : m_path(cachePath)
{
    ensureCacheDirCreated_();
}

FileCache::FileCache()
    : m_path("")
    , m_isTemporaryFolder(true)
{
    try {
        m_path = fs::temp_directory_path() / "elevation" / "FileCache";
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
    deleteIntermediateFolders_();
    m_isInitialized = false;
    m_isTemporaryFolder = false;
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
    std::transform(fs::begin(cacheIt), fs::end(cacheIt), std::back_inserter(sizes), [](const fs::directory_entry& dirEntry) {
        try {
            return fs::file_size(dirEntry);
        } catch (fs::filesystem_error& e) {
            return (uintmax_t)0;
        }
    });
    return (size_t)std::accumulate(sizes.cbegin(), sizes.cend(), (uintmax_t)0);
}

std::vector<fs::path> FileCache::getFileList() const {
    std::vector<std::experimental::filesystem::path> paths;
    auto cacheIt = fs::directory_iterator(m_path);
    std::transform(fs::begin(cacheIt), fs::end(cacheIt), std::back_inserter(paths), [](const fs::directory_entry& dirEntry) {
        return dirEntry.path();
    });
    return paths;
}

void FileCache::setFileContent(const std::string& fileName, std::string& data) {
    std::stringstream ss(data);
    setFileContent(fileName, ss);
}

void FileCache::setFileContent(const std::string& fileName, std::istream& data) {
    std::ofstream file((m_path / fileName).string());
    fs::permissions(m_path / fileName, FILE_PERMISSIONS);
    std::istreambuf_iterator<char> begin(data);
    std::istreambuf_iterator<char> end;
    std::copy(begin, end, std::ostreambuf_iterator(file));
    file.close();
}

std::string FileCache::getFileContent(const std::string& fileName) const {
    std::string data;
    getFileContent(fileName, data);
    return data;
}

void FileCache::getFileContent(const std::string& fileName, std::string& data) const {
    std::stringstream ss;
    getFileContent(fileName, ss);
    data = ss.str();
}

void FileCache::getFileContent(const std::string& fileName, std::ostream& data) const {
    std::ifstream file((m_path / fileName).string());
    std::istreambuf_iterator<char> begin(file);
    std::istreambuf_iterator<char> end;
    std::copy(begin, end, std::ostreambuf_iterator(data));
    file.close();
}

void FileCache::deleteFile(const std::string& fileName) {
    fs::remove_all(m_path / fileName);
}

bool FileCache::isFileCached(const std::string& fileName) {
    return fs::exists(m_path / fileName);
}

fs::directory_entry FileCache::getFile(const std::string& fileName) const {
    return fs::directory_entry(m_path / fileName);
}

void FileCache::ensureCacheDirCreated_() noexcept {
    try {
        if (!fs::exists(m_path)) {
            fs::create_directories(m_path);
        }
        for (auto& entry : fs::directory_iterator(m_path)) {
            fs::remove_all(entry.path());
        }
        m_isInitialized = true;
        std::cout << "File Cache path : " << m_path.string() << std::endl;
    }
    catch (fs::filesystem_error& e) {
        std::clog << e.what();
    }
}

void FileCache::wipeCachedFiles_() noexcept {
    try {
        for (auto& path: fs::directory_iterator(m_path)) {
            fs::remove_all(path);
        }
    }
    catch (fs::filesystem_error& e) {
        std::clog << e.what();
    }
}

void FileCache::deleteIntermediateFolders_() noexcept {
    try {
        if (m_isTemporaryFolder && m_path.has_parent_path() && m_path.parent_path().has_parent_path()) {
            try {
                fs::remove_all(m_path.parent_path());
            }
            catch (fs::filesystem_error& e) {
                std::clog << e.what();
            }
        }
    }
    catch (...) { }
}

} // namespace elevation
