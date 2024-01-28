#include "util.h"

History::History()
{
}

History::~History()
{
}

Global::Global()
{
    Global::UpdatePath();
}

Global& Global::Self()
{
    static Global g_var;
    return g_var;
}

bool Global::IsFileSelected()
{
    if (IsValidPath() && Self().selected_file_index >= 0 && Self().is_file_or_folder == IS_FILE)
    {
        return true;
    }
    return false;
}

bool Global::IsFolderSelected()
{
    if (IsValidPath() && Self().selected_file_index >= 0 && Self().is_file_or_folder == IS_FOLDER)
    {
        return true;
    }
    return false;
}

bool Global::IsHistorySelected()
{
    if (IsFileSelected() && Self().selected_history_index >= 0)
    {
        return true;
    }
    return false;
}

bool Global::IsValidPath()
{
    // TODO
    return true;
}

void Global::UpdatePath()
{
    SDL_Log("UpdatePath:");
    // TODO: check path exist
    // TODO: ReadFolderList
    // TODO: update file_list
}

std::vector<std::string_view> splitSV(std::string_view strv, std::string_view delims = " ")
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
    string_view filter_str{ Self().filter_buffer };
    Self().filter_list = splitSV(filter_str, "|");
    SDL_Log("UpdateFilter:");
#ifdef MY_DEBUG
    for (int i = 0; i < Self().filter_list.size(); i++)
    {
        cout << "\t" << Self().filter_list[i] << endl;
    }
#endif
    // TODO: update filtered_file_list
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

std::vector<path> ReadFolderList(const path& str)
{
    using namespace std;
    using namespace std::filesystem;
    std::vector<path> list;
    for (auto const& dir_entry : recursive_directory_iterator{ str })
    {
        if (!dir_entry.is_directory())
        {
            list.emplace_back(dir_entry.path());
        }
    }
    return list;
}

IS_FILE_FOLDER_ IsDirFile(const path& str)
{
    using namespace std::filesystem;
    if (is_directory(str))
    {
        return IS_FOLDER;
    }
    if (is_regular_file(str))
    {
        return IS_FILE;
    }
    return IS_NONE;
}