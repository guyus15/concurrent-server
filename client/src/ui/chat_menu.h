#pragma once

#include <common/ui/ui.h>

#include <common/utils/platform.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <iomanip>
#include <string>
#include <time.h>

struct ChatMessage
{
    std::string timestamp;
    std::string author;
    std::string content;
};

constexpr int CHAT_MAX_MESSAGE_LENGTH = 96;
constexpr float CHAT_MENU_PADDING_BOTTOM = 1.75f,
                CHAT_MENU_MARGIN = 5.0f;

void OnChatVisible(GameEvent& evt);
void OnChatReceive(GameEvent& evt);

bool HasContent(char input[CHAT_MAX_MESSAGE_LENGTH]);

class ChatMenu final : public UserInterface
{
public:
    void Initialise() override
    {
        EventManager::AddListener<OnChatVisibleEvent>(OnChatVisible);
        EventManager::AddListener<OnChatReceiveEvent>(OnChatReceive);

        m_title = "Chat";
        m_show = false;
    }

    void Update(const double delta_time) override
    {
        if (!m_show) return;

        const bool start_show = m_show;

        const ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(CHAT_MENU_MARGIN, io.DisplaySize.y - CHAT_MENU_MARGIN), ImGuiCond_Always,
                                ImVec2(0.0f, 1.0f));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 3.0f, io.DisplaySize.y / 4.0f), ImGuiCond_Always);

        constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        ImGui::Begin(m_title.c_str(), &m_show, window_flags);

        const float textbox_height = ImGui::GetTextLineHeight() * CHAT_MENU_PADDING_BOTTOM;

        constexpr ImGuiWindowFlags child_flags = ImGuiWindowFlags_None;
        ImGui::BeginChild(
            "##chat-messages",
            ImVec2(0, ImGui::GetContentRegionAvail().y - textbox_height),
            child_flags);

        // List all received messages.
        for (const auto& [timestamp, author, content] : m_messages)
            ImGui::TextWrapped("[%s] %s: %s", timestamp.c_str(), author.c_str(), content.c_str());

        if (m_received_new_message)
        {
            ImGui::SetScrollHereY(1.0f);
            m_received_new_message = false;
        }

        ImGui::EndChild();

        constexpr ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;

        // Limit the message length to the maximum packet size.
        static char input[CHAT_MAX_MESSAGE_LENGTH];

        // The has content variables determines whether the user can send a message or not.
        // When false, pressing the enter or clicking the send button will do nothing.
        const bool has_content = HasContent(input);

        // Set the keyboard focus the text box, but only if no items are currently active.
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !
            ImGui::IsMouseClicked(0))
            ImGui::SetKeyboardFocusHere(0);

        if (ImGui::InputTextWithHint("##chat-input", "Enter message", input, IM_ARRAYSIZE(input), input_flags) &&
            has_content)
            SendUserMessage(input);

        ImGui::SameLine();

        if (!has_content)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        if (ImGui::Button("Send") && has_content)
            SendUserMessage(input);

        if (!has_content)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        ImGui::End();

        if (!m_show && start_show)
        {
            OnChatVisibleEvent evt{};
            evt.visible = false;
            EventManager::Broadcast(evt);
        }
    }

private:
    std::vector<ChatMessage> m_messages;
    bool m_received_new_message{};

    void SendUserMessage(char input[CHAT_MAX_MESSAGE_LENGTH])
    {
        // Send a chat message packet to the client.
        OnChatSendEvent evt{};
        evt.message = input;
        EventManager::Broadcast(evt);

        // Create a local message.
        CreateNewMessage(std::chrono::system_clock::now(), "You", std::string{ input });

        // Clear the text box.
        std::memset(input, 0, CHAT_MAX_MESSAGE_LENGTH);
    }

    void CreateNewMessage(const std::chrono::time_point<std::chrono::system_clock> timestamp, const std::string& author,
                          const std::string& content)
    {
        const auto timestamp_t = std::chrono::system_clock::to_time_t(std::chrono::floor<std::chrono::seconds>(timestamp));
        std::tm timestamp_tm{};

#if defined(SCX_PLATFORM_WINDOWS)
        if (const int code = localtime_s(&timestamp_tm, &timestamp_t); code != 0)
        {
            SCX_CORE_ERROR("Failed to convert timestamp to local time representation. [Code: {0}]", code);
        }
#else
        localtime_r(&timestamp_t, &timestamp_tm);
#endif

        std::ostringstream oss;
        oss << std::put_time(&timestamp_tm, "%H:%M:%S");

        m_messages.emplace_back(oss.str(), author, content);
        m_received_new_message = true;
    }

    friend void OnChatVisible(GameEvent& evt);
    friend void OnChatReceive(GameEvent& evt);
};

inline void OnChatVisible(GameEvent& evt)
{
    const auto& on_chat_visible_event = dynamic_cast<OnChatVisibleEvent&>(evt);

    const std::shared_ptr<UserInterface> ui = UiManager::GetRegisteredUi<ChatMenu>();
    const auto chat_menu = std::dynamic_pointer_cast<ChatMenu>(ui);

    chat_menu->m_show = on_chat_visible_event.visible;
}

inline void OnChatReceive(GameEvent& evt)
{
    const auto& on_chat_receive_event = dynamic_cast<OnChatReceiveEvent&>(evt);

    const std::shared_ptr<UserInterface> ui = UiManager::GetRegisteredUi<ChatMenu>();
    const auto chat_menu = std::dynamic_pointer_cast<ChatMenu>(ui);

    chat_menu->CreateNewMessage(on_chat_receive_event.timestamp, on_chat_receive_event.author,
                                on_chat_receive_event.message);
}

inline bool HasContent(char input[CHAT_MAX_MESSAGE_LENGTH])
{
    for (int i = 0; i < CHAT_MAX_MESSAGE_LENGTH; i++)
    {
        if (input[i] != 0)
            return true;
    }

    return false;
}
