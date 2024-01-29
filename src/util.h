#pragma once

#include "common.h"
#include <filesystem>

using std::filesystem::path;

bool IsFExist(const path &str);
bool CreateFolder(const path &str);
bool DeleteF(const path &str);
bool CopyFile(const path &src, const path &dst);
void ReadFList(const path &str, std::vector<path> &output, bool recursive);
