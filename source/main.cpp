#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <Shlobj.h>
#define PATH_SEPARATOR "\\"
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <cstring>
#define PATH_SEPARATOR "/"
#endif

std::string getDownloadsFolderPath() {
    std::string downloadsPath;
#ifdef _WIN32
    // Get the path to the Downloads folder on Windows
    PWSTR path;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path))) {
        wchar_t* widePath = path;
        char narrowPath[MAX_PATH];
        wcstombs(narrowPath, widePath, MAX_PATH);
        downloadsPath = narrowPath;
        CoTaskMemFree(path);
    }
#else
    // Get the path to the Downloads folder on Linux
    const char* homeDir = getenv("HOME");
    if (homeDir != NULL) {
        downloadsPath = std::string(homeDir) + "/Downloads";
    }
#endif
    return downloadsPath;
}

#ifdef _WIN32
time_t getFileCreationTime(const std::string& filePath) {
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (GetFileAttributesEx(filePath.c_str(), GetFileExInfoStandard, &fileInfo)) {
        ULARGE_INTEGER fileTime;
        fileTime.LowPart = fileInfo.ftCreationTime.dwLowDateTime;
        fileTime.HighPart = fileInfo.ftCreationTime.dwHighDateTime;
        return static_cast<time_t>(fileTime.QuadPart / 10000000ULL - 11644473600ULL);
    }
    return -1; // Error occurred
}
#else
time_t getFileCreationTime(const std::string& filePath) {
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) == 0) {
        return fileInfo.st_ctime;
    }
    return -1; // Error occurred
}
#endif

int main() {
    // Get the path to the Downloads folder
    std::string downloadsFolder = getDownloadsFolderPath();

    // Initialize variables to track the oldest file
    std::string oldestFile;
    time_t oldestCreationTime = std::numeric_limits<time_t>::max();

    // Iterate through files in the Downloads folder
    for (const auto& entry : std::filesystem::directory_iterator(downloadsFolder)) {
        if (entry.is_regular_file()) {
            // Get the creation time of the file
            time_t creationTime = getFileCreationTime(entry.path().string());
            if (creationTime != -1 && creationTime < oldestCreationTime) {
                oldestCreationTime = creationTime;
                oldestFile = entry.path().string();
            }
        }
    }

    if (!oldestFile.empty()) {
        // Print the oldest file and its creation time
        std::cout << "Oldest file: " << oldestFile << std::endl;
        std::cout << "Creation time: " << std::ctime(&oldestCreationTime);
    } else {
        std::cerr << "No files found in the Downloads folder." << std::endl;
    }

    return 0;
}