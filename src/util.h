#pragma once

#include "common.h"

class Global
{
private:
    Global() {}

public:
    enum IS_FILE_FOLDER_
    {
        IS_NONE,
        IS_FILE,
        IS_FOLDER
    };

    int selected_file_index = -1;
    int selected_history_index = -1;
    int is_file_or_folder = IS_NONE;
    char message_buffer[300] = { 0 };
    char path_buffer[300] = { 0 };
    char filter_buffer[100] = ".xls|.xlsx|.doc|.docx";
    std::vector<std::string> file_list;
    std::vector<std::string> filtered_file_list;

    Global(const Global& g) = delete;
    static Global& Self()
    {
        static Global g_var;
        return g_var;
    }

    static bool IsFileSelected()
    {
        if (IsValidPath() && Self().selected_file_index >= 0 && Self().is_file_or_folder == IS_FILE)
        {
            return true;
        }
        return false;
    }

    static bool IsFolderSelected()
    {
        if (IsValidPath() && Self().selected_file_index >= 0 && Self().is_file_or_folder == IS_FOLDER)
        {
            return true;
        }
        return false;
    }

    static bool IsHistorySelected()
    {
        if (IsFileSelected() && Self().selected_history_index >= 0)
        {
            return true;
        }
        return false;
    }

    static bool IsValidPath()
    {
        // TODO
        return true;
    }
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