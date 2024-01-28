#include "window.h"
#include "util.h"

using namespace ImGui;

void OperationWindow()
{
    Begin("Operation", nullptr, 0);
    SeparatorText(u8"文件操作");
    Spacing();

    InputTextWithHint(u8"备注", u8"表1增加xxx，表2修改xxx", Global::Self().message_buffer, sizeof(Global::Self().message_buffer), 0, nullptr, nullptr);

    if (Global::Self().IsFileSelected())
    {
        if (Button(u8"备份", ImVec2(-1, 0)))
        {
            SDL_Log("Backup Button");
        }
    }
    else
    {
        Button(u8"[选择一个文件]", ImVec2(-1, 0));
    }

    if (Global::Self().IsHistorySelected())
    {
        if (Button(u8"删除历史节点", ImVec2(-1, 0)))
        {
            SDL_Log("Delete One Node Button");
        }
    }
    else
    {
        Button(u8"[选择一个文件历史]", ImVec2(-1, 0));
    }

    if (Global::Self().IsFileSelected())
    {
        if (Button(u8"清空文件历史", ImVec2(-1, 0)))
        {
            SDL_Log("Delete All Node Button");
        }
    }
    else
    {
        Button(u8"[选择一个文件]", ImVec2(-1, 0));
    }

    Spacing();
    SeparatorText(u8"当前文件夹操作");
    Spacing();

    if (Global::Self().IsFolderSelected())
    {
        if (Button(u8"清空无效历史", ImVec2(-1, 0)))
        {
            SDL_Log("Current Dir Clear Useless Button");
        }
    }
    else
    {
        Button(u8"[选择一个文件夹]", ImVec2(-1, 0));
    }

    if (Global::Self().IsFolderSelected())
    {
        if (Button(u8"清空所有历史", ImVec2(-1, 0)))
        {
            SDL_Log("Current Dir Clear All Button");
        }
    }
    else
    {
        Button(u8"[选择一个文件夹]", ImVec2(-1, 0));
    }

    Spacing();
    SeparatorText(u8"当前文件夹和子文件夹操作");
    Spacing();

    if (Global::Self().IsFolderSelected())
    {
        if (Button(u8"清空无效历史", ImVec2(-1, 0)))
        {
            SDL_Log("Clear Useless Button");
        }
    }
    else
    {
        Button(u8"[选择一个文件夹]", ImVec2(-1, 0));
    }

    if (Global::Self().IsFolderSelected())
    {
        if (Button(u8"清空所有历史", ImVec2(-1, 0)))
        {
            SDL_Log("Clear All Button");
        }
    }
    else
    {
        Button(u8"[选择一个文件夹]", ImVec2(-1, 0));
    }

    End();
}

void FileWindow()
{
    Begin("File", nullptr, 0);
    InputTextWithHint(u8"文件路径", "C:\\Users\\Admin\\Desktop", Global::Self().path_buffer, sizeof(Global::Self().path_buffer), 0, nullptr, nullptr);
    InputTextWithHint(u8"过滤", ".xls|.xlsx|.doc|.docx", Global::Self().filter_buffer, sizeof(Global::Self().filter_buffer), 0, nullptr, nullptr);

    BeginChild("FileList", ImVec2(-1, -1), ImGuiChildFlags_Border, 0);
    for (int n = 0; n < 5; n++)
    {
        char buf[32];
        sprintf(buf, "Object %d", n);
        if (Selectable(buf, Global::Self().selected_file_index == n))
            Global::Self().selected_file_index = n;
        // TODO: file or folder?
    }
    EndChild();

    End();
}

void HistoryWindow()
{
    Begin("History", nullptr, 0);

    if (1)
    {
        Text(u8"文件名：");
        Text(u8"历史备份：");
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
        const char *names[3] = {"One", "Two", "Three"};
        const char *paths[3] = {"/path/one", "/path/two", "/path/three"};
        for (int i = 0; i < 3; i++) // row
        {
            char label[32];
            sprintf(label, "%04d", i);
            if (ImGui::Selectable(label, Global::Self().selected_history_index == i, ImGuiSelectableFlags_SpanAllColumns))
                Global::Self().selected_history_index = i;
            bool hovered = ImGui::IsItemHovered();
            ImGui::NextColumn();
            ImGui::Text(names[i]);
            ImGui::NextColumn();
            ImGui::Text(paths[i]);
            ImGui::NextColumn();
            ImGui::Text(paths[i]);
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::Separator();
    }

    End();
}

void AllWindow()
{
    OperationWindow();
    FileWindow();
    HistoryWindow();
}