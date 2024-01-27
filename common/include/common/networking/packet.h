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
    PlayerConnected,
    PlayerDisconnected,
    ServerShutdown,
    ChatMessage
};

// Return codes utilised by packet methods.
enum PacketCode
{
    PacketCode_Success = 0,
    PacketCode_FullPacketE = -1,
    PacketCode_NoDataToReadE = -1
};

class Packet
{
public:
    Packet(PacketType type = PacketType::Unspecified);

    Packet(const Packet &) = default;
    Packet &operator=(const Packet &) = default;

    Packet(Packet &&) = default;
    Packet &operator=(Packet &&) = default;

    /**
     * \brief Writes data of type \code T\endcode to the packet.
     * \tparam T The type of data to write.
     * \param value The value of data to write.
     * \return An error code indicating if the write was successful.
     * \code PacketCode_Success\endcode is returned if the operation performed successfully.
     * \code PacketCode_FullPacketE\endcode is returned if the packet is full and cannot be written to.
     */
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

    /**
     * \brief Reads data of type \code T\endcode from the packet.
     * \tparam T The type of data to read.
     * \param dest A reference to the destination of the read value.
     * \return An error code indicating if the read was successful.
     * \code PacketCode_Success\endcode is returned if the operation performed successfully.
     * \code PacketCode_FullPacketE\endcode is returned if the packet is empty and cannot be read from.
     */
    template <typename T>
    int Read(T &dest)
    {
        if (m_read_head == m_size)
            return PacketCode_NoDataToReadE;

        int type_size = sizeof(T);
        std::memcpy(&dest, m_buffer + m_read_head, type_size);
        m_read_head += type_size;

        return PacketCode_Success;
    }

    /**
     * \brief Sets the type of the packet.
     * \param type The new packet type.
     */
    void SetType(PacketType type);

    /**
     * \brief Gets the type of the packet.
     * \return A \code PacketType\endcode value indicating the type of the packet.
     */
    [[nodiscard]] PacketType GetType() const;

    /**
     * \brief Gets the current size of the packet.
     * \return A value indicating the size of the packet.
     */
    [[nodiscard]] int GetSize() const;

private:
    PacketType m_type;
    unsigned int m_size;
    unsigned int m_read_head;
    unsigned char m_buffer[PACKET_SIZE];

    [[nodiscard]] bool IsPacketFull(int new_data_size) const;
};

template <>
inline int Packet::Write(std::string value)
{
    for (const auto &c : value)
    {
        int i_result = Write(c);
        if (i_result != PacketCode_Success)
            return i_result;
    }

    // Write an additional character to null-terminate the string.
    Write('\0');

    return PacketCode_Success;
}

template <>
inline int Packet::Read(std::string &dest)
{
    unsigned int start_pos = m_read_head;
    char buffer[PACKET_SIZE]{};

    unsigned int i = start_pos;
    for (i = start_pos; m_buffer[i] != '\0'; i++)
    {
        buffer[i - start_pos] = m_buffer[i];
    }

    // Increment the read head to account for the buffer size with null-terminatation character.
    m_read_head += (static_cast<unsigned long long>(i) - start_pos + 1) * sizeof(char);

    dest.assign(buffer, static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(i) - start_pos);

    return PacketCode_Success;
}

class IPacketDispatcher;
using PacketHandler = std::function<void(unsigned int, Packet&, const IPacketDispatcher*)>;

/*
 * \brief A structure containing a packet with its relevant additional information
 * when it is received from a client.
 */
struct PacketInfoFromClient
{
    unsigned int from_client;
    Packet packet;
};

/**
 * \brief A structure containing a packet with additional information specifying
 * how it will be sent to a client.
 */
struct PacketInfoToClient
{
    unsigned int to_client;
    Packet packet;
};
