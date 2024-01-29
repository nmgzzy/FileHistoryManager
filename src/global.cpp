#include "global.h"

Global::Global()
{
}

Global &Global::Self()
{
    static Global g_var;
    return g_var;
}

bool Global::IsFileSelected()
{
    using namespace std::filesystem;
    return is_regular_file(Self().selected_path);
}

bool Global::IsFolderSelected()
{
    using namespace std::filesystem;
    return !Self().folder_path.empty() || is_directory(Self().selected_path);
}

bool Global::IsHistorySelected()
{
    if (IsFileSelected() && Self().selected_history_index >= 0)
    {
        return true;
    }
    return false;
}

static void UpdateFilteredFileList(const std::vector<path> &input, const std::vector<std::string_view> &filter_list, std::vector<path> &output)
{
    using namespace std;
    output.clear();
    SDL_Log("UpdateFilteredFileList: ");
    for (const path &item : input)
    {
        if (is_regular_file(item))
        {
            string ext = item.extension().string();
            string_view ext_view(ext);
            if (std::find(filter_list.begin(), filter_list.end(), ext) != filter_list.end())
            {
                output.emplace_back(item);
                SDL_Log("\tfind:%s", item.string().c_str());
            }
        }
    }
}

static std::string& trim(std::string& s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void Global::UpdatePath()
{
    using namespace std;
    using namespace std::filesystem;
    bool flag_select = false;
    string s_trim = trim(string(Self().path_buffer));
    path temp_path = u8path(s_trim);
    if (temp_path.empty() || !exists(temp_path))
    {
        goto _Error;
    }
    if (is_directory(temp_path))
    {
        Self().folder_path = temp_path;
    }
    else if (is_regular_file(temp_path))
    {
        Self().folder_path = temp_path.parent_path();
        flag_select = true;
    }
    else
    {
        goto _Error;
    }

    SDL_Log("UpdatePath: %s", Self().folder_path.string().c_str());
    ReadFList(Self().folder_path, Self().file_list, false);
    UpdateFilter();
    if (flag_select)
    {
        const auto &list = Self().filtered_file_list;
        auto it = std::find(list.begin(), list.end(), temp_path);
        if (it != list.end())
        {
            Self().set_selected_file_index(std::distance(list.begin(), it));
        }
    }
    return;

_Error:
    SDL_Log("UpdatePath: bad path");
    Self().folder_path = path();
    return;
}

void Global::UpdatePath(const char *u8str)
{
    strcpy_s(Self().path_buffer, sizeof(Self().path_buffer), u8str);
    UpdatePath();
}

void Global::UpdateFilter()
{
    using namespace std;
    string filter(Self().filter_buffer);

    Global::Self().set_selected_file_index(-1);

    if (filter.empty())
    {
        SDL_Log("UpdateFilter: empty filter");
        Self().filtered_file_list = Self().file_list;
        return;
    }
    string_view filter_str(filter);
    Self().filter_list = splitSV(filter_str, "|");
    SDL_Log("UpdateFilter:");

    for (const string_view &item : Self().filter_list)
    {
        SDL_Log("\t%s", string(item).c_str());
    }

    UpdateFilteredFileList(Self().file_list, Self().filter_list, Self().filtered_file_list);
}