#pragma once

#include <common/ui/ui.h>

#include <imgui.h>

class ConnectMenu final : public UserInterface
{
public:
    void Update(const double delta_time) override
    {
        m_show = true; // Temporary

        if (m_show)
        {
            ImGui::ShowDemoWindow();
        }
    }
};