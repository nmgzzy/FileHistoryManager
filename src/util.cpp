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

void ReadFList(const path &path_, std::vector<path> &output, bool recursive)
{
    using namespace std;
    using namespace std::filesystem;

    output.clear();

    if (recursive)
    {
        for (auto const &dir_entry : recursive_directory_iterator{path_})
        {
            if (dir_entry.is_directory() || dir_entry.is_regular_file())
            {
                output.emplace_back(dir_entry.path());
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

void ShowMessageBox(const char *msg)
{
    extern SDL_Window *g_window;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, u8"注意", msg, g_window);
}