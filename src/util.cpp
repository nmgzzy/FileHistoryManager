#include "util.h"
#include <chrono>

bool CreateFolder(const path &path_)
{
    using namespace std::filesystem;
    bool ret = create_directories(path_);
    SDL_Log("CreateFolder, path:%s, ret:%d", path_.string().c_str(), ret);
    return ret;
}

bool DeleteF(const path &path_)
{
    using namespace std;
    using namespace std::filesystem;
    error_code err;
    int count = remove_all(path_, err);
    SDL_Log("DeleteF, path:%s, cnt:%d, err:%d", path_.string().c_str(), count, err.value());
    return count > 0 && err.value() == 0;
}

bool CopyFile(const path &src, const path &dst)
{
    using namespace std::filesystem;
    bool ret = copy_file(src, dst);
    SDL_Log("CopyFile, from:%s, to:%s, ret:%d", src.string().c_str(), dst.string().c_str(), ret);
    return ret;
}

bool ReadFList(const path &path_, std::vector<path> &output, bool recursive, bool dir, bool file)
{
    using namespace std;
    using namespace std::filesystem;

    int cnt = 0;
    output.clear();

    if (recursive)
    {
        for (auto const &dir_entry : recursive_directory_iterator{path_})
        {
            if (dir && dir_entry.is_directory() || file && dir_entry.is_regular_file())
            {
                output.emplace_back(dir_entry.path());
                cnt++;
            }
            if (cnt > 10000)
            {
#ifdef _WINDOW_APP_
                ShowMessageBox(u8"Error：文件数目过多！");
#endif
                return false;
            }
        }
    }
    else
    {
        for (auto const &dir_entry : directory_iterator{path_})
        {
            if (dir_entry.is_directory() || dir_entry.is_regular_file())
            {
                output.emplace_back(dir_entry.path());
            }
        }
    }
    return true;
}

static bool endsWith(const std::string &str, const std::string suffix = ".history")
{
    if (suffix.length() > str.length())
    {
        return false;
    }
    return (str.rfind(suffix) == (str.length() - suffix.length()));
}

void DeleteBackup(const path &path_, bool only_useless, bool recursive)
{
    using namespace std::filesystem;
    if (!is_directory(path_))
    {
        return;
    }
    std::vector<path> list;
    if (!ReadFList(path_, list, recursive, true, false))
    {
        return;
    }
    for (const auto &i : list)
    {
        if (endsWith(i.u8string()))
        {
            std::string temp_s = i.string();
            path temp_p = path(temp_s.substr(0, temp_s.length() - 8));
            if (!only_useless || !exists(temp_p))
            {
                DeleteF(i);
            }
        }
    }
}

std::vector<std::string_view> splitSV(std::string_view strv, std::string_view delims)
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

std::string GetMd5(const path &p)
{
    extern std::string getFileMD5(const std::string &filename);
    return getFileMD5(p.string());
}

long long GetTimeNow()
{
    using namespace std::chrono;
    return system_clock::to_time_t(system_clock::now());
}

std::string GetTime(long long time)
{
    using namespace std;
    using namespace std::chrono;

    if (time == 0)
    {
        time = system_clock::to_time_t(system_clock::now());
    }

    char buf[100];
    tm local_time;
    localtime_s(&local_time, &time);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local_time);
    return string(buf);
}

#ifdef _WINDOW_APP_
void ShowMessageBox(const char *msg)
{
    extern SDL_Window *g_window;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, u8"注意", msg, g_window);
}

int ShowMessageBoxWithButton(const char *msg)
{
    extern SDL_Window *g_window;

    SDL_MessageBoxButtonData buttons[2] = {
        {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
         0,
         u8"取消"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
         1,
         u8"好的"}};

    SDL_MessageBoxData msgboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        g_window,
        u8"注意",
        msg,
        2,
        buttons,
        NULL};
    int btn_id = 0;

    SDL_ShowMessageBox(&msgboxdata, &btn_id);

    return btn_id;
}
#endif