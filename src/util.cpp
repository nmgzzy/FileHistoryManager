#include "util.h"
#include <algorithm>
#include <string.h>

History::History()
{
}

History::~History()
{
}

Global::Global()
{
}

Global& Global::Self()
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

static void UpdateFilteredFileList(const std::vector<path>& input, const std::vector<std::string_view> &filter_list, std::vector<path>& output)
{
    using namespace std;
    output.clear();
    SDL_Log("UpdateFilteredFileList: ");
    for (const path& item : input)
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

void Global::UpdatePath()
{
    using namespace std::filesystem;
    bool flag_select = false;
    path temp_path = u8path(Self().path_buffer);
    if (temp_path.empty() || !IsFExist(temp_path))
    {
        goto _Error;
    }
    //if (!IsFExist(temp_path))
    //{
    //    temp_path = temp_path.parent_path();
    //    if (temp_path.empty())
    //    {
    //        goto _Error;
    //    }
    //}
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
        const auto& list = Self().filtered_file_list;
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

void Global::UpdatePath(const char * u8str)
{
    strcpy_s(Self().path_buffer, sizeof(Self().path_buffer), u8str);
    UpdatePath();
}

static std::vector<std::string_view> splitSV(std::string_view strv, std::string_view delims = " ")
{
    std::vector<std::string_view> output;
    size_t first = 0;

    while (first < strv.size())
    {
        const auto second = strv.find_first_of(delims, first);

        if (first != second)
            output.emplace_back(strv.substr(first, second - first));

        if (second == std::string_view::npos)
            break;

        first = second + 1;
    }

    return output;
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


bool IsFExist(const path& str)
{
    using namespace std::filesystem;
    return exists(str);
}

bool CreateFolder(const path& str)
{
    using namespace std::filesystem;
    bool ret = create_directories(str);
    SDL_Log("CreateFolder, path:%s, ret:%d", str.string().c_str(), ret);
    return ret;
}

bool DeleteF(const path& str)
{
    using namespace std;
    using namespace std::filesystem;
    error_code err;
    int count = remove_all(str, err);
    SDL_Log("DeleteF, path:%s, cnt:%d, err:%d", str.string().c_str(), count, err.value());
    return count > 0 && err.value() == 0;
}

bool CopyFile(const path &src, const path& dst)
{
    using namespace std::filesystem;
    bool ret = copy_file(src, dst);
    SDL_Log("CopyFile, from:%s, to:%s, ret:%d", src.string().c_str(), dst.string().c_str(), ret);
    return ret;
}

void ReadFList(const path& str, std::vector<path> &output, bool recursive)
{
    using namespace std;
    using namespace std::filesystem;

    output.clear();

    if (recursive)
    {
        for (auto const& dir_entry : recursive_directory_iterator{ str })
        {
            if (dir_entry.is_directory() || dir_entry.is_regular_file())
            {
                output.emplace_back(dir_entry.path());
            }
        }
    }
    else
    {
        for (auto const& dir_entry : directory_iterator{ str })
        {
            if (dir_entry.is_directory() || dir_entry.is_regular_file())
            {
                output.emplace_back(dir_entry.path());
            }
        }
    }
}

//IS_FILE_FOLDER_ IsDirFile(const path& str)
//{
//    using namespace std::filesystem;
//    if (is_directory(str))
//    {
//        return IS_FOLDER;
//    }
//    if (is_regular_file(str))
//    {
//        return IS_FILE;
//    }
//    return IS_NONE;
//}