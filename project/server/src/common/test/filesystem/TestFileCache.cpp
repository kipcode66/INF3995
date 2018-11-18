#define BOOST_TEST_MODULE FileCache
#include <boost/test/unit_test.hpp>

#include <filesystem/FileCache.hpp>

#include <TestResources.hpp>

#include <fstream>
#include <iostream>
#include <thread>

namespace elevation {

namespace fs = std::experimental::filesystem;

/**
 * Cleans up after all FileCache test case.
 */
void cleanup() {
    fs::path fileCacheDir = std::string(FILE_CACHE_TEST_DIRECTORY);
    // Ensure the cache directory is deleted.
    fs::remove_all(fileCacheDir);
}

std::string getFileContent(std::ifstream& t) {
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
    return str;
}

BOOST_AUTO_TEST_CASE(simpleConstructionDestruction) {
    fs::path fileCacheDir = std::string(FILE_CACHE_TEST_DIRECTORY);
    // Ensure the cache directory is deleted.
    fs::remove_all(fileCacheDir);

    FileCache* fileCache = new FileCache(std::string(FILE_CACHE_TEST_DIRECTORY));
    BOOST_TEST(fs::exists(fileCacheDir));
    delete fileCache;
    BOOST_TEST(!fs::exists(fileCacheDir));
    cleanup();
}

BOOST_AUTO_TEST_CASE(afterCrashConstructionDestruction) {
    fs::path fileCacheDir = std::string(FILE_CACHE_TEST_DIRECTORY);
    fs::path dummyMp3SourceFile = TEST_MP3_PATH;
    fs::path dummyMp3DestinationFile = fileCacheDir;
    dummyMp3DestinationFile /= dummyMp3SourceFile.filename();
    // Ensure the cache directory exists.
    fs::create_directories(fileCacheDir);
    // Copy a file over
    fs::copy_file(dummyMp3SourceFile, dummyMp3DestinationFile);
    BOOST_REQUIRE(fs::exists(dummyMp3DestinationFile));

    FileCache* fileCache = new FileCache(std::string(FILE_CACHE_TEST_DIRECTORY));
    BOOST_TEST(fs::exists(fileCacheDir));
    BOOST_TEST(!fs::exists(dummyMp3DestinationFile));
    delete fileCache;
    BOOST_TEST(!fs::exists(fileCacheDir));
    cleanup();
}

BOOST_AUTO_TEST_CASE(checkFileCount) {
    fs::path fileCacheDir = std::string(FILE_CACHE_TEST_DIRECTORY);
    fs::path dummyMp3SourceFile = TEST_MP3_PATH;
    fs::path dummyMp3DestinationFile = fileCacheDir;
    dummyMp3DestinationFile /= dummyMp3SourceFile.filename();

    FileCache* fileCache = new FileCache(std::string(FILE_CACHE_TEST_DIRECTORY));
    // Ensure the cache directory exists.
    fs::create_directories(fileCacheDir);
    // Copy a file over
    fs::copy_file(dummyMp3SourceFile, dummyMp3DestinationFile);
    BOOST_REQUIRE(fs::exists(dummyMp3DestinationFile));

    BOOST_TEST(fileCache->fileCount() == 1);
    BOOST_TEST(fileCache->cacheSize() > 0);
    BOOST_TEST(fileCache->getFileList().size() > 0);
    delete fileCache;
    BOOST_TEST(!fs::exists(fileCacheDir));
    cleanup();
}

BOOST_AUTO_TEST_CASE(getSetDeleteFileContent) {
    fs::path fileCacheDir = std::string(FILE_CACHE_TEST_DIRECTORY);
    fs::path dummyMp3SourceFile = TEST_MP3_PATH;
    fs::path dummyMp3DestinationFile = fileCacheDir;
    dummyMp3DestinationFile /= dummyMp3SourceFile.filename();

    FileCache fileCache(FILE_CACHE_TEST_DIRECTORY);
    BOOST_TEST(!fs::exists(dummyMp3DestinationFile));

    std::ifstream is(dummyMp3SourceFile.string());
    BOOST_REQUIRE(!is.fail());

    fileCache.setFileContent(dummyMp3DestinationFile.filename(), is);
    is.close();

    BOOST_TEST(fs::exists(dummyMp3DestinationFile));
    BOOST_TEST(fs::file_size(dummyMp3DestinationFile) == fs::file_size(dummyMp3SourceFile));

    is.open(dummyMp3SourceFile.string());
    BOOST_REQUIRE(!is.fail());
    std::string content = getFileContent(is);
    is.close();

    std::string content2;
    fileCache.getFileContent(dummyMp3DestinationFile.filename(), content2);
    BOOST_TEST(content == content2);

    fileCache.deleteFile(dummyMp3DestinationFile.filename());

    BOOST_TEST(!fs::exists(dummyMp3DestinationFile));

    cleanup();
}

BOOST_AUTO_TEST_CASE(readWhileRightThreaded) {
    fs::path fileCacheDir = std::string(FILE_CACHE_TEST_DIRECTORY);
    fs::path dummyMp3SourceFile = TEST_MP3_PATH;
    fs::path dummyMp3DestinationFile = fileCacheDir;
    dummyMp3DestinationFile /= dummyMp3SourceFile.filename();

    FileCache fileCache(FILE_CACHE_TEST_DIRECTORY);
    BOOST_TEST(!fs::exists(dummyMp3DestinationFile));

    std::ifstream is(dummyMp3SourceFile.string());
    BOOST_REQUIRE(!is.fail());
    auto t = std::thread([&]() {
        fileCache.setFileContent(dummyMp3DestinationFile.filename(), is);
        is.close();
    });

    auto t2 = std::thread([&]() {
        std::string content2;
        fileCache.getFileContent(dummyMp3DestinationFile.filename(), content2);
    });

    BOOST_CHECK_NO_THROW(t.join());
    BOOST_CHECK_NO_THROW(t2.join());

    fileCache.deleteFile(dummyMp3DestinationFile.filename());

    BOOST_TEST(!fs::exists(dummyMp3DestinationFile));

    cleanup();
}

} // namespace elevation
