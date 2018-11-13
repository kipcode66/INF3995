#ifndef SRC_HTTP_FS_FILECACHE_HPP
#define SRC_HTTP_FS_FILECACHE_HPP

#include <string>
#include <iostream>
#include <experimental/filesystem>

namespace elevation {

class FileCache {
public:
    static const std::experimental::filesystem::perms FILE_PERMISSIONS;

public:
    explicit FileCache(const std::string& cachePath);
    FileCache(const char *cachePath) : FileCache(std::string(cachePath)) {} // Explicit conversion to string.
    FileCache(const std::experimental::filesystem::path& cachePath);
    FileCache(const FileCache&) = delete;
    FileCache();
    virtual ~FileCache();

    FileCache& operator=(const FileCache&) = delete;

    bool isInitialized();

    size_t fileCount() const;
    size_t cacheSize() const;
    std::vector<std::experimental::filesystem::path> getFileList() const;
    void setFileContent(const std::string& fileName, std::string& data);
    void setFileContent(const std::string& fileName, std::istream& data);
    std::string getFileContent(const std::string& fileName) const;
    void getFileContent(const std::string& fileName, std::string& data) const;
    void getFileContent(const std::string& fileName, std::ostream& data) const;
    void deleteFile(const std::string& fileName);
    bool isFileCached(const std::string& fileName);
    std::experimental::filesystem::directory_entry getFile(const std::string& fileName) const;

protected:
    void ensureCacheDirCreated_() noexcept;
    void wipeCachedFiles_() noexcept;
    void deleteIntermediateFolders_() noexcept;

protected:
    std::experimental::filesystem::path m_path;
    bool m_isTemporaryFolder = false;
    bool m_isInitialized = false;
};

} // namespace elevation

#endif // !SRC_HTTP_FS_FILECACHE_HPP