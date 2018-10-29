#ifndef SRC_HTTP_FS_FILECACHE_HPP
#define SRC_HTTP_FS_FILECACHE_HPP

#include <string>
#include <experimental/filesystem>

namespace elevation {

class FileCache {
public:
    explicit FileCache(const std::string& cachePath);
    FileCache(const std::experimental::filesystem::path& cachePath);
    FileCache(const FileCache&) = delete;
    FileCache();
    virtual ~FileCache();

    FileCache& operator=(const FileCache&) = delete;

    bool isInitialized();

    size_t fileCount() const;
    size_t cacheSize() const;
    std::vector<std::experimental::filesystem::path> getFileList() const;
    void setFileContent(const std::string& fileName, const std::string& data);
    void deleteFile(const std::string& fileName);
    bool isFileCached(const std::string& fileName);
    std::experimental::filesystem::directory_entry getFile(const std::string& fileName) const;

protected:
    void ensureCacheDirCreated_();
    void wipeCachedFiles_();

protected:
    std::experimental::filesystem::path m_path;
    bool m_isInitialized;
};

} // namespace elevation

#endif // !SRC_HTTP_FS_FILECACHE_HPP