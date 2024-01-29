#pragma once

#include "common.h"
#include <filesystem>

using std::filesystem::path;

enum IS_FILE_FOLDER_
{
    IS_NONE,
    IS_FILE,
    IS_FOLDER
};

class Global
{
private:
    int selected_file_index = -1;
    int selected_history_index = -1;
    Global();
public:
    char message_buffer[300] = { 0 };
    char path_buffer[300] = { 0 };
    char filter_buffer[100] = ".xls|.xlsx|.doc|.docx";

    path folder_path;
    path selected_path;

    std::vector<path> file_list;
    std::vector<path> filtered_file_list;
    std::vector<std::string_view> filter_list;

    Global(const Global& g) = delete;

    void set_selected_file_index(int i)
    {
        selected_file_index = i;
        if (i < 0 || i >= filtered_file_list.size())
        {
            selected_path = path();
        }
        else
        {
            selected_path = filtered_file_list[i];
        }
    }

    int get_selected_file_index() const
    {
        return selected_file_index;
    }

    void set_selected_history_index(int i)
    {
        selected_history_index = i;
    }

    int get_selected_history_index() const
    {
        return selected_history_index;
    }

    static Global& Self();
    static bool IsFileSelected();
    static bool IsFolderSelected();
    static bool IsHistorySelected();
    static void UpdatePath();
    static void UpdatePath(const char* u8str);
    static void UpdateFilter();
};

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
    //void Save();
    //void Load();
public:
    History(/* args */);
    ~History();
    //void Add();
    //void Delete();
    //void ClearAll();
};

bool IsFExist(const path& str);
bool CreateFolder(const path& str);
bool DeleteF(const path& str);
bool CopyFile(const path& src, const path& dst);
void ReadFList(const path& str, std::vector<path>& output, bool recursive);
//IS_FILE_FOLDER_ IsDirFile(const path& str);