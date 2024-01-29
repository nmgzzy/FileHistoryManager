#pragma once
#include "common.h"

struct HisNode
{
    std::string time;
    std::string name;
    std::string message;
    std::string hash;
};

class History
{
private:
    std::vector<HisNode> info;
    // void Save();
    // void Load();
public:
    History(/* args */);
    ~History();
    // void Add();
    // void Delete();
    // void ClearAll();
};