#include "window.h"
#include "global.h"

using namespace ImGui;

static void OperationWindowClearModule(bool recursive)
{
    std::string lab1 = std::string(u8"清空无效历史") + (recursive ? u8"##1" : u8"##2");
    std::string lab2 = std::string(u8"清空所有历史") + (recursive ? u8"##1" : u8"##2");

    if (Button(lab1.c_str(), ImVec2(-1, 0)))
    {
        SDL_Log("Current Dir Clear Useless Button");
        if (!Global::Self().IsFolderSelected())
        {
            ShowMessageBox(u8"请先选择一个文件夹！");
        }
        else
        {
            if (ShowMessageBoxWithButton(u8"确定清空无效历史？"))
            {
                DeleteBackup(Global::Self().folder_path.string().c_str(), true, recursive);
                SDL_Log("Clear Useless, recursive:%d", recursive);
            }
            else
            {
                SDL_Log("Not Clear Useless, recursive:%d", recursive);
            }
        }
    }

    if (Button(lab2.c_str(), ImVec2(-1, 0)))
    {
        SDL_Log("Current Dir Clear All Button");
        if (!Global::Self().IsFolderSelected())
        {
            ShowMessageBox(u8"请先选择一个文件夹！");
        }
        else
        {
            if (ShowMessageBoxWithButton(u8"确定清空全部历史？"))
            {
                DeleteBackup(Global::Self().folder_path.string().c_str(), false, recursive);
                SDL_Log("Clear Useless, recursive:%d", recursive);
                SDL_Log("Clear All, recursive:%d", recursive);
            }
            else
            {
                SDL_Log("Not Clear All, recursive:%d", recursive);
            }
        }
    }
}

static void OperationWindow()
{
    Begin("Operation", nullptr, 0);
    SeparatorText(u8"文件操作");
    Spacing();
    {
        InputTextWithHint(u8"备注", u8"表1增加xxx，表2修改xxx", Global::Self().message_buffer, sizeof(Global::Self().message_buffer), 0, nullptr, nullptr);

        if (Button(u8"备份", ImVec2(-1, 0)))
        {
            SDL_Log("Backup Button");
            if (!Global::Self().IsFileSelected())
            {
                ShowMessageBox(u8"请先选择一个文件！");
            }
            else if (strlen(Global::Self().message_buffer) == 0)
            {
                ShowMessageBox(u8"备注信息不能为空！");
            }
            else
            {
                Global::Self().selected_history.Add();
                Global::Self().message_buffer[0] = 0;
            }
        }

        if (Button(u8"删除历史节点", ImVec2(-1, 0)))
        {
            SDL_Log("Delete One Node Button");
            if (!Global::Self().IsHistorySelected())
            {
                ShowMessageBox(u8"请先选择一个文件历史！");
            }
            else
            {
                Global::Self().selected_history.Delete(Global::Self().get_selected_history_index());
            }
        }

        if (Button(u8"清空文件历史", ImVec2(-1, 0)))
        {
            SDL_Log("Delete All Node Button");
            if (!Global::Self().IsFileSelected())
            {
                ShowMessageBox(u8"请先选择一个文件！");
            }
            else
            {
                Global::Self().selected_history.ClearAll();
            }
        }

        if (Button(u8"恢复文件历史", ImVec2(-1, 0)))
        {
            SDL_Log("Recover Node Button");
            if (!Global::Self().IsHistorySelected())
            {
                ShowMessageBox(u8"请先选择一个文件历史！");
            }
            else
            {
                // todo
            }
        }
    }

    Spacing();
    SeparatorText(u8"当前文件夹操作");
    Spacing();

    OperationWindowClearModule(false);

    Spacing();
    SeparatorText(u8"当前文件夹和子文件夹操作");
    Spacing();

    OperationWindowClearModule(true);

    End();
}

static void FileWindow()
{
    Begin("File", nullptr, 0);
    if (InputTextWithHint(u8"文件路径", "C:\\Users\\Admin\\Desktop", Global::Self().path_buffer, sizeof(Global::Self().path_buffer), ImGuiInputTextFlags_None, nullptr, nullptr))
    {
        Global::UpdatePath();
    }

    if (InputTextWithHint(u8"过滤", ".xls|.xlsx|.doc|.docx", Global::Self().filter_buffer, sizeof(Global::Self().filter_buffer), ImGuiInputTextFlags_CharsNoBlank, nullptr, nullptr))
    {
        Global::UpdateFilter();
    }

    BeginChild("FileList", ImVec2(-1, -1), ImGuiChildFlags_Border, 0);
    for (int i = 0; i < Global::Self().filtered_file_list.size(); i++)
    {
        const path &item = Global::Self().filtered_file_list[i];
        if (Selectable(item.filename().u8string().c_str(), Global::Self().get_selected_file_index() == i))
        {
            Global::Self().set_selected_file_index(i);
        }
    }
    EndChild();

    End();
}

static void HistoryWindow()
{
    Begin("History", nullptr, 0);

    Text(u8"文件名：");
    SameLine();
    Text(Global::Self().selected_path.u8string().c_str());
    SeparatorText(u8"历史备份");
    Spacing();

    ImGui::Columns(4, "mycolumns"); // 4-ways, with border
    ImGui::Separator();
    ImGui::Text(u8"时间");
    ImGui::NextColumn();
    ImGui::Text(u8"名称");
    ImGui::NextColumn();
    ImGui::Text(u8"备注");
    ImGui::NextColumn();
    ImGui::Text(u8"校验");
    ImGui::NextColumn();
    ImGui::Separator();
    const auto &info = Global::Self().selected_history.info;
    for (int i = 0; i < info.size(); i++) // row
    {
        if (ImGui::Selectable(info[i].time_s.c_str(), Global::Self().get_selected_history_index() == i, ImGuiSelectableFlags_SpanAllColumns))
        {
            Global::Self().set_selected_history_index(i);
        }
        bool hovered = ImGui::IsItemHovered();
        ImGui::NextColumn();
        ImGui::Text(info[i].name.c_str());
        ImGui::NextColumn();
        ImGui::Text(info[i].message.c_str());
        ImGui::NextColumn();
        ImGui::Text(info[i].hash.c_str());
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();

    End();
}

void AllWindow()
{
    OperationWindow();
    FileWindow();
    HistoryWindow();
}