#include "history.h"
#include "global.h"
#include <fstream>

History::History(path _path_)
{
    path_ = _path_;
    backup_path = path(path_.string() + ".history");
    backup_info_path = backup_path / "info.txt";
    Load();
    std::sort(info.begin(), info.end(), [](HisNode &a, HisNode &b)
              { return a.time > b.time; });
}

History::History()
{
}

void History::Save()
{
    using namespace std;

    CreateFolder(backup_path);

    ofstream f(backup_info_path, ios::trunc | ios::out);
    for (auto &i : info)
    {
        f << std::to_string(i.time) << "|"
          << i.time_s << "|"
          << i.name << "|"
          << i.message << "|"
          << i.hash << std::endl;
    }
    f.close();
}
void History::Load()
{
    using namespace std;

    if (!filesystem::exists(backup_path))
        return;

    ifstream f(backup_info_path);

    while (f.peek() != EOF)
    {
        char bufLine[200] = {0};
        f.getline(bufLine, 200);
        auto v = splitSV(bufLine, "|");
        if (v.size() == 5)
        {
            HisNode node;
            node.time = stoll(string(v[0]));
            node.time_s = string(v[1]);
            node.name = string(v[2]);
            node.message = string(v[3]);
            node.hash = string(v[4]);
            info.emplace_back(node);
        }
    }
    f.close();
}

path History::GetBackupFileName(const HisNode &node)
{
    char buff[300] = {0};
    sprintf_s(buff, sizeof(buff),
              u8"%s\\%s.%04x.%s.history",
              backup_path.u8string().c_str(),
              path_.filename().u8string().c_str(),
              (int)(node.time % 0x10000),
              node.hash.substr(0, 4).c_str());

    return std::filesystem::u8path(buff);
}

#ifdef _WINDOW_APP_
void History::Add()
{
    Add(std::string(Global::Self().message_buffer));
}
#endif

void History::Add(std::string msg)
{
    HisNode node;

    if (path_.empty() || !std::filesystem::exists(path_))
        return;

    node.time = GetTimeNow();
    node.time_s = GetTime(node.time);
    node.message = msg;
    node.hash = GetMd5(path_);
    path backup_file = GetBackupFileName(node);
    node.name = backup_file.filename().u8string();
    info.emplace_back(node);

    SDL_Log("History::Add: %s", backup_file.string().c_str());
    CreateFolder(backup_path);
    CopyFile(path_, backup_file);
    Save();
}

void History::Delete(int i)
{
    if (i < 0 || i >= info.size())
        return;
    if (info.size() == 1 && i == 0)
    {
        ClearAll();
        return;
    }
    auto &it = info.begin() + i;
    path del_path = GetBackupFileName(info[i]);
    DeleteF(del_path);
    info.erase(it);
    Save();
}

void History::ClearAll()
{
    using namespace std::filesystem;
    if (exists(backup_path))
    {
        DeleteF(backup_path);
    }
    info.clear();
}