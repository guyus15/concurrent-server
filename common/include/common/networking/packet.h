#pragma once

#include <cstring>
#include <string>
#include <functional>

constexpr int PACKET_SIZE = 128;

enum class PacketType
{
    Unspecified,
    Welcome,
    WelcomeReceived,
    Disconnect,
    ChatMessage
};

// Return codes utilised by packet methods.
const int PacketCode_Success = 0;
const int PacketCode_FullPacketE = -1;
const int PacketCode_NoDataToReadE = -2;

class Packet
{
public:
    Packet(PacketType type = PacketType::Unspecified);

    Packet(const Packet&) = default;
    Packet& operator=(const Packet&) = default;

    Packet(Packet&&) = default;
    Packet& operator=(Packet&&) = default;

    template <typename T>
    int Write(T value)
    {
        int type_size = sizeof(T);

        if (IsPacketFull(type_size))
            return PacketCode_FullPacketE;

        std::memcpy(m_buffer + m_size, &value, type_size);
        m_size += type_size;

        return PacketCode_Success;
    }

    template <>
    int Write(std::string value)
    {
        for (const auto& c : value)
        {
            int i_result = Write(c);
            if (i_result != PacketCode_Success) return i_result;
        }

        // Write an additional character to null-terminate the string.
        Write('\0');

        return PacketCode_Success;
    }

    template <typename T>
    int Read(T& dest)
    {
        if (m_read_head == m_size)
            return PacketCode_NoDataToReadE;

        int type_size = sizeof(T);
        std::memcpy(&dest, &(m_buffer[m_read_head]), type_size);
        m_read_head += type_size;

        return PacketCode_Success;
    }

    template <>
    int Read(std::string& dest)
    {
        int start_pos = m_read_head;
        char buffer[PACKET_SIZE];

        int i = start_pos;
        for (i = start_pos; m_buffer[i] != '\0'; i++)
        {
            buffer[i - start_pos] = m_buffer[i];
        }

        // Increment the read head to account for the buffer size with null-terminatation character.
        m_read_head += (i - start_pos + 1) * sizeof(char);

        dest.assign(buffer, i - start_pos);

        return PacketCode_Success;
    }

    void SetType(PacketType type);
    [[nodiscard]] PacketType GetType() const;
    [[nodiscard]] int GetSize() const;

private:
    PacketType m_type;
    int m_size;
    int m_read_head;
    unsigned char m_buffer[PACKET_SIZE];

    [[nodiscard]] bool IsPacketFull(int new_data_size) const;
};

class IPacketDispatcher;
using PacketHandler = std::function<void(Packet&, const IPacketDispatcher*)>;
