#include "common/networking/packet.h"

Packet::Packet(const PacketType type)
    : m_type{ type },
      m_size{ 0 },
      m_read_head{ 0 },
      m_buffer{}
{
}

void Packet::SetType(const PacketType type)
{
    m_type = type;
}

PacketType Packet::GetType() const
{
    return m_type;
}

int Packet::GetSize() const
{
    return m_size;
}

bool Packet::IsPacketFull(const int new_data_size) const
{
    return m_size + new_data_size >= PACKET_SIZE;
}
