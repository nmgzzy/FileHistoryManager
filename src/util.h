#pragma once

#include "common.h"

bool CreateFolder(const path &path_);            //
bool DeleteF(const path &path_);                 //
bool CopyFile(const path &src, const path &dst); //
void ReadFList(const path &path_, std::vector<path> &output, bool recursive);

std::vector<std::string_view> splitSV(std::string_view strv, std::string_view delims = " ");

long long GetTimeNow();
std::string GetTime(long long time = 0);
std::string GetMd5(const path &p);
void ShowMessageBox(const char *msg);
