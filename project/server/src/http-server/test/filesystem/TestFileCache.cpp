#define BOOST_TEST_MODULE FileCache
#include <boost/test/unit_test.hpp>

#include <filesystem/FileCache.hpp>

#include <TestConfiguration.hpp>

namespace elevation {

using namespace fs = std::experimental::filesystem;

/**
 * Cleans up after all FileCache test case.
 */
void cleanup() {
    fs::path fileCacheDir(FILE_CACHE_TEST_DIRECTORY);
    // Ensure the cache directory is deleted.
    fs::remove_all(fileCacheDir);
}

BOOST_AUTO_TEST_CASE(simpleConstructionDestruction) {
    fs::path fileCacheDir(FILE_CACHE_TEST_DIRECTORY);
    // Ensure the cache directory is deleted.
    fs::remove_all(fileCacheDir);

    FileCache* fileCache = new FileCache(FILE_CACHE_TEST_DIRECTORY);
    BOOST_TEST(fs::exists(fileCacheDir));
    delete fileCache;
    BOOST_TEST(!fs::exists(fileCacheDir));
    cleanup();
}

BOOST_AUTO_TEST_CASE(afterCrashConstructionDestruction) {
    fs::path fileCacheDir(FILE_CACHE_TEST_DIRECTORY);
    fs::path dummyMp3SourceFile(TEST_MP3_PATH);
    fs::path dummyMp3DestinationFile(FileCache);
    dummyMp3DestinationFile /= dummyMp3SourceFile.filename();
    // Ensure the cache directory exists.
    fs::create_directories(fileCacheDir);
    // Copy a file over
    fs::copy_file(dummyMp3SourceFile, dummyMp3DestinationFile);
    BOOST_REQUIRE(fs::exists(dummyMp3DestinationFile))

    FileCache* fileCache = new FileCache(FILE_CACHE_TEST_DIRECTORY);
    BOOST_TEST(fs::exists(fileCacheDir));
    BOOST_TEST(!fs::exists(dummyMp3DestinationFile))
    delete fileCache;
    BOOST_TEST(!fs::exists(fileCacheDir));
    cleanup();
}

} // namespace elevation
