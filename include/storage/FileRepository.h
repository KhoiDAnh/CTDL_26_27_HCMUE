#pragma once

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

#include "core/LinkedList.h"
#include "core/StringUtils.h"

template <typename T>
class FileRepository {
private:
    static void cleanLine(std::string& line, std::size_t lineNumber);
public:
    template <typename Parser>
    static LinkedList<T> loadAll(const std::string& path, Parser parser);

    template <typename Parser, typename KeyOf>
    static LinkedList<T> loadAll(const std::string& path, Parser parser, KeyOf keyOf);

    template <typename Parser>
    static T loadOne(const std::string& path, Parser parser);

    template <typename KeyOf>
    static void checkUnique(const LinkedList<T>& items, KeyOf keyOf);

    static void saveAll(const std::string& path, const LinkedList<T>& items);
    static void saveOne(const std::string& path, const T& item);
    static bool exists(const std::string& path);
    static void removeFile(const std::string& path);
};

template <typename T>
bool FileRepository<T>::exists(const std::string& path){
    std::ifstream in(path);
    return in.is_open();
}

template <typename T>
void FileRepository<T>::cleanLine(std::string& line, std::size_t lineNumber) {
    // 1. a trailing '\r' (file written on windows, read on another os)
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    // 2. if line starts with "\xEF\xBB\xBF" UTF-8 BOM (byte order mark) -> erase
    if (lineNumber == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
        line.erase(0, 3);
    }
}

template <typename T>
template <typename Parser>
LinkedList<T> FileRepository<T>::loadAll(const std::string& path, Parser parser) {
    // 1. open file, if cant open -> throw
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::invalid_argument("FileRepository: Khong the mo " + path);
    }

    // 2. variables to loop
    LinkedList<T> result;
    std::string line;
    std::size_t lineNumber = 0;

    // 3. load loop
    while (std::getline(file, line)) {
        ++lineNumber; // count every physical line, even blank lines
        cleanLine(line, lineNumber);
        if (StringUtils::isBlank(line))
            continue; // skip if blank

        // d. try/catch errors from file
        try {
            result.pushBack(parser(line));
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("FileRepository: " + path
                + ", dong " + std::to_string(lineNumber) + ": " + e.what());
        }
    }
    return result;
}

template <typename T>
template <typename KeyOf>
void FileRepository<T>::checkUnique(const LinkedList<T>& items, KeyOf keyOf) {
    // O(n^2) pairs, and each at() is O(n) on a LL: fine at this size
    for (std::size_t i = 0; i < items.size(); ++i) {
        const std::string key = keyOf(items.at(i));
        // check every key pair of (i, j) for duplicates
        for (std::size_t j = i + 1; j < items.size(); ++j) {
            if (keyOf(items.at(j))== key) {
                throw std::invalid_argument("FileRepository: Khoa bi trung: " + key);
            }
        }
    }
}

template <typename T>
template <typename Parser, typename KeyOf>
LinkedList<T> FileRepository<T>::loadAll(const std::string& path, Parser parser, KeyOf keyOf) {
    LinkedList<T> result = loadAll(path, parser);
    checkUnique(result, keyOf);
    return result;
}

template <typename T>
template <typename Parser>
T FileRepository<T>::loadOne(const std::string& path, Parser parser) {
    const LinkedList<T> items = loadAll(path, parser);
    if (items.size() != 1) {
        throw std::invalid_argument("FileRepository: " + path + " phai co dung 1 dong du lieu.");
    }
    return items.at(0); // load ONLY the first index of LL
}

template <typename T>
void FileRepository<T>::saveAll(const std::string& path, const LinkedList<T>& items) {
    // 1. truncate path (the old content is gone)
    std::ofstream out(path);
    if (!out.is_open()) {
        throw std::invalid_argument("FileRepository: Khong the mo de ghi " + path);
    }

    // 2. for each item in items, print item of that index
    for (std::size_t i = 0; i < items.size(); ++i) {
        out << items.at(i).toLine() << '\n';
    }

    out.flush(); // writes are buffered; flush so a failure can show up
    // 3. check if stream worked
    if (!out) {
        throw std::invalid_argument("FileRepository: Khong the ghi " + path);
    }

    // *KNOWN LIMIT: a crash mid-write leaves a half-written file*
}

template <typename T>
void FileRepository<T>::saveOne(const std::string& path, const T& item) {
    LinkedList<T> single;
    single.pushBack(item);
    saveAll(path, single);
}

template <typename T>
void FileRepository<T>::removeFile(const std::string& path) {
    // Used when a ticket is returned and its file must be removed (0 = success)
    if (std::remove(path.c_str()) != 0) {
        throw std::invalid_argument("FileRepository: Khong the xoa " + path);
    }
}
