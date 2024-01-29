#include "util.h"

bool IsFExist(const path &str)
{
    using namespace std::filesystem;
    return exists(str);
}

bool CreateFolder(const path &str)
{
    using namespace std::filesystem;
    bool ret = create_directories(str);
    SDL_Log("CreateFolder, path:%s, ret:%d", str.string().c_str(), ret);
    return ret;
}

bool DeleteF(const path &str)
{
    using namespace std;
    using namespace std::filesystem;
    error_code err;
    int count = remove_all(str, err);
    SDL_Log("DeleteF, path:%s, cnt:%d, err:%d", str.string().c_str(), count, err.value());
    return count > 0 && err.value() == 0;
}

bool CopyFile(const path &src, const path &dst)
{
    using namespace std::filesystem;
    bool ret = copy_file(src, dst);
    SDL_Log("CopyFile, from:%s, to:%s, ret:%d", src.string().c_str(), dst.string().c_str(), ret);
    return ret;
}

void ReadFList(const path &str, std::vector<path> &output, bool recursive)
{
    using namespace std;
    using namespace std::filesystem;

    output.clear();

    if (recursive)
    {
        for (auto const &dir_entry : recursive_directory_iterator{str})
        {
            if (dir_entry.is_directory() || dir_entry.is_regular_file())
            {
                output.emplace_back(dir_entry.path());
            }
        }
    }
    else
    {
        for (auto const &dir_entry : directory_iterator{str})
        {
            if (dir_entry.is_directory() || dir_entry.is_regular_file())
            {
                output.emplace_back(dir_entry.path());
            }
        }
    }
}
