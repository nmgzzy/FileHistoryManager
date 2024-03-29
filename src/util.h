﻿#pragma once

#include "common.h"

bool CreateFolder(const path &path_);
bool DeleteF(const path &path_);
bool CopyFile(const path &src, const path &dst);
bool ReadFList(const path &path_, std::vector<path> &output, bool recursive, bool dir = true, bool file = true);

void DeleteBackup(const path &path_, bool only_useless, bool recursive);

std::vector<std::string_view> splitSV(std::string_view strv, std::string_view delims = " ");

long long GetTimeNow();
std::string GetTime(long long time = 0);
std::string GetMd5(const path &p);
#ifdef _WINDOW_APP_
void ShowMessageBox(const char *msg);
int ShowMessageBoxWithButton(const char *msg);
#endif