#include "common.h"
#include "history.h"

using namespace std;

int main(int argc, char **argv)
{
    path input_path;
    string message;
    string cmd;

#ifdef MY_DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("debug mode!");
#else
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);
#endif

    SDL_Log("argc:%d", argc);
    for (int i = 0; i < argc; i++)
    {
        SDL_Log("arg%d:%s", i, argv[i]);
    }

#ifdef MY_DEBUG
    if (argc == 1)
    {
        cout << "Please Input Path:" << endl;
        cin >> input_path;
    }else
#endif
    if (argc == 2)
    {
        input_path = path(argv[1]);
    }
    else if (argc == 3)
    {
        cmd = string(argv[1]);
        input_path = path(argv[2]);
    }
    else
    {
        cerr << "Parameter Error!" << endl << endl;
        return -1;
    }

    if (!filesystem::exists(input_path))
    {
        cerr << "Path Error!" << endl << endl;
        return -2;
    }

    do
    {
        cout << "Please Input Backup Message:" << endl;
        message.clear();
        cin >> message;
        if (message.empty())
        {
            cout << "Message CANNOT be Empty!" << endl;
        }
    } while (message.empty());

    path temp(message);
    message = temp.u8string();

    History history(input_path);
    history.Add(message);

    cout << "Backup Finish." << endl;

    return 0;
}
