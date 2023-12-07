#pragma once

#include "common/events/events.h"
#include "common/utils/logging.h"

#include <ranges>
#include <typeinfo>
#include <unordered_map>
#include <vector>

using Listener = void(*)(GameEvent&);

class EventManager
{
public:
    /**
     * \brief Adds a listener for the specified event.
     * \tparam T The event type.
     * \param listener The listener.
     */
    template <typename T>
    static void AddListener(Listener&& listener)
    {
        s_event_listeners[typeid(T).hash_code()].push_back(listener);
    }

    /**
     * \brief Removes a listener for the specified event.
     * \tparam T The event type.
     * \param listener The listener.
     */
    template <typename T>
    static void RemoveListener(Listener&& listener)
    {
        // First make sure there are listeners for this type.
        const auto listeners_it = s_event_listeners.find(typeid(T).hash_code());
        if (listeners_it == s_event_listeners.end())
        {
            // Return if no listeners exist.
            return;
        }

        // Now look to see if the listener exists, if so, remove it.
        std::vector<Listener>& listeners = s_event_listeners[typeid(T).hash_code()];

        const auto target_listener_it = std::ranges::find(listeners, listener);
        if (target_listener_it != listeners.end())
            listeners.erase(target_listener_it);
    }

    /**
     * \brief Removes all listeners for the specified event.
     * \tparam T The event type.
     */
    template <typename T>
    static void ClearListeners()
    {
        std::vector<Listener>& listeners = s_event_listeners[typeid(T).hash_code()];
        listeners.clear();
    }

    /**
     * \brief Removes all listeners for every event.
     */
    static void ClearAllListeners()
    {
        for (auto& listeners : s_event_listeners | std::views::values)
        {
            listeners.clear();
        }
    }

    /**
     * \brief Broadcasts the given event.
     * \param evt The given event.
     */
    static void Broadcast(GameEvent& evt)
    {
        for (const auto& listener : s_event_listeners[typeid(evt).hash_code()])
        {
            listener(evt);
        }
    }

private:
    inline static std::unordered_map<EventId, std::vector<Listener>> s_event_listeners{};
};
