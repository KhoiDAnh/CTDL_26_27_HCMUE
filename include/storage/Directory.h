#pragma once

#include <string>

#include "core/LinkedList.h"

namespace Directory {

    // Names (not paths) of the files in a folder. The folder path must end with '/'.
    // A missing or empty folder gives an empty list.
    LinkedList<std::string> listFiles(const std::string& folder);

}  // namespace Directory