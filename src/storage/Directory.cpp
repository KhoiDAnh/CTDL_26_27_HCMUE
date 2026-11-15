#include "storage/Directory.h"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

LinkedList<std::string> Directory::listFiles(const std::string& folder) {
    LinkedList<std::string> names;
    WIN32_FIND_DATAA entry;
    HANDLE handle = FindFirstFileA((folder + "*").c_str(), &entry);
    if (handle == INVALID_HANDLE_VALUE) {
        return names;
    }
    do {
        if ((entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            names.pushBack(std::string(entry.cFileName));
        }
    } while (FindNextFileA(handle, &entry) != 0);
    FindClose(handle);
    return names;
}

#else
#include <dirent.h>

LinkedList<std::string> Directory::listFiles(const std::string& folder) {
    LinkedList<std::string> names;
    DIR* directory = opendir(folder.c_str());
    if (directory == nullptr) {
        return names;
    }
    while (dirent* entry = readdir(directory)) {
        names.pushBack(std::string(entry->d_name));
    }
    closedir(directory);
    return names;
}

#endif