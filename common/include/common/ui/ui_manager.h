#pragma once

#include "common/ui/ui.h"
#include "common/utils/assertion.h"

#include <memory>
#include <ranges>

class UiManager
{
public:
    UiManager(const UiManager&) = delete;
    UiManager& operator=(const UiManager&) = delete;

    UiManager(UiManager&&) noexcept = default;
    UiManager& operator=(UiManager&&) noexcept = default;

    // Register UIs within the implementation files.
    static void Initialise();

    static void Update(const double delta_time)
    {
        for (const auto& ui : std::views::values(Get().m_ui_registry))
        {
            ui->Update(delta_time);
        }
    }

    template <typename T>
    static [[nodiscard]] std::shared_ptr<UserInterface> GetRegisteredUi()
    {
        const auto it = Get().m_ui_registry.find(typeid(T).hash_code());
        if (it != Get().m_ui_registry.end())
            return it->second;

        SCX_ASSERT(0, "Could not find registered UI with this name");

        return nullptr;
    }

    static void Dispose()
    {
        for (const auto& ui : std::views::values(Get().m_ui_registry))
        {
            ui->Dispose();
        }

        Get().m_ui_registry.clear();
    }

private:
    std::unordered_map<size_t, std::shared_ptr<UserInterface>> m_ui_registry;

    UiManager() = default;
    ~UiManager() = default;

    static UiManager s_instance;
    static UiManager& Get()
    {
        return s_instance;
    }

    template <typename T, typename... Args>
    static void RegisterUi(Args... args)
    {
        std::shared_ptr<T> ui = std::make_shared<T>(std::forward<Args>(args)...);
        Get().m_ui_registry[typeid(T).hash_code()] = ui;

        ui->Initialise();
    }

    template <typename T>
    static void UnregisterUi()
    {
        Get().m_ui_registry.erase(typeid(T).hash_code());
    }
};
