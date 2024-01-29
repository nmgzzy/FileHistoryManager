#pragma once
#include "common.h"
#include "util.h"

struct HisNode
{
    long long time;
    std::string time_s;
    std::string name;
    std::string message;
    std::string hash;
};

class History
{
private:
    path path_;
    path backup_path;
    path backup_info_path;

    void Save();
    void Load();
    path GetBackupFileName(const HisNode &hash);

public:
    std::vector<HisNode> info;

    History(path _path_);
    History();
    void Add();
    void Delete(int i);
    void ClearAll();
};