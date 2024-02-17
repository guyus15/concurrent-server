#define CLOVE_SUITE_NAME PacketTests
#include <clove-unit.h>

#include <common/networking/packet.h>

#include <string>

// Test 1
CLOVE_TEST(TestByteReadAfterWrite)
{
    /**
     * This test ensures that after writing a byte to a packet, the data read from it
     * is identical.
     */

    Packet test_packet{};

    constexpr char write_value = 'a';
    test_packet.Write(write_value);

    char read_value;
    test_packet.Read(read_value);

    CLOVE_CHAR_EQ(write_value, read_value);
}

// Test 2
CLOVE_TEST(TestIntegerReadAfterWrite)
{
    /**
     * This test ensures that after writing an integer to a packet, the data read from it
     * is identical.
     */

    Packet test_packet{};

    constexpr int write_value = 5;
    test_packet.Write(write_value);

    int read_value;
    test_packet.Read(read_value);

    CLOVE_INT_EQ(write_value, read_value);
}

// Test 3
CLOVE_TEST(TestFloatReadAfterWrite)
{
    /**
     * This test ensures that after writing a float to a packet, the data read from it
     * is identical.
     */

    Packet test_packet{};

    constexpr float write_value = 3.5f;
    test_packet.Write(write_value);

    float read_value;
    test_packet.Read(read_value);

    CLOVE_FLOAT_EQ(write_value, read_value);
}

// Test 4
CLOVE_TEST(TestDoublReadAfterWrite)
{
    /**
     * This test ensures that after writing a double to a packet, the data read from it
     * is identical.
     */

    Packet test_packet{};

    constexpr double write_value = 8.7;
    test_packet.Write(write_value);

    double read_value;
    test_packet.Read(read_value);

    CLOVE_DOUBLE_EQ(write_value, read_value);
}

// Test 5
CLOVE_TEST(TestMultipleWritesAndReads)
{
    /**
     * This test ensures that after writing multiple values to a packet, the read data
     * is correct both in order and in value.
     */

    Packet test_packet{};

    constexpr char write_value1 = 'b';
    constexpr float write_value2 = 4.5f;
    constexpr double write_value3 = 6.78;

    test_packet.Write(write_value1);
    test_packet.Write(write_value2);
    test_packet.Write(write_value3);

    char read_value1;
    test_packet.Read(read_value1);

    float read_value2;
    test_packet.Read(read_value2);

    double read_value3;
    test_packet.Read(read_value3);

    CLOVE_CHAR_EQ(write_value1, read_value1);
    CLOVE_FLOAT_EQ(write_value2, read_value2);
    CLOVE_DOUBLE_EQ(write_value3, read_value3);
}

// Test 6
CLOVE_TEST(TestWriteToFullPacket)
{
    /**
     * This test ensures that when writing to a full packet, an appropriate error code
     * is returned and the write is not performed.
     */

    constexpr int no_writes = PACKET_SIZE / sizeof(int);

    Packet test_packet{};

    for (int i = 0; i < no_writes; i++)
    {
        test_packet.Write(i);
    }

    const int error_code = test_packet.Write(10);

    CLOVE_INT_EQ(PacketCode_FullPacketE, error_code);
}

// Test 7
CLOVE_TEST(TestReadOfEmptyPacket)
{
    /**
     * This test ensures that when attempting to read from an empty packet, an appropriate error
     * code is retunred and no value is read.
     */

    Packet test_packet{};

    int read_value = -1;
    const int i_result = test_packet.Read(read_value);

    CLOVE_INT_EQ(PacketCode_NoDataToReadE, i_result);
    CLOVE_INT_EQ(-1, read_value);
}

// Test 8
CLOVE_TEST(TestUnevenReadsAndWrites)
{
    /**
     * This test ensures that when more reads are performed than writes, an appropriate error
     * code is returned and the excess reads are not performed.
     */

    Packet test_packet{};

    // Here we will write 3 times, but read 4 times. The fourth read should return an empty
    // packet error code, but the three before should be successful.
    constexpr int write_value1 = 1;
    constexpr int write_value2 = 2;
    constexpr int write_value3 = 3;

    test_packet.Write(write_value1);
    test_packet.Write(write_value2);
    test_packet.Write(write_value3);

    int i_result = 0;

    int read_value1;
    i_result = test_packet.Read(read_value1);

    CLOVE_INT_EQ(write_value1, read_value1);
    CLOVE_INT_EQ(PacketCode_Success, i_result);

    int read_value2;
    i_result = test_packet.Read(read_value2);

    CLOVE_INT_EQ(write_value2, read_value2);
    CLOVE_INT_EQ(PacketCode_Success, i_result);

    int read_value3;
    i_result = test_packet.Read(read_value3);

    CLOVE_INT_EQ(write_value3, read_value3);
    CLOVE_INT_EQ(PacketCode_Success, i_result);

    int read_value4 = -1;
    i_result = test_packet.Read(read_value4);

    // Read value 4 should be unchanged.
    CLOVE_INT_EQ(-1, read_value4);
    CLOVE_INT_EQ(PacketCode_NoDataToReadE, i_result);
}

// Test 9
CLOVE_TEST(TestWriteReadStringPacket)
{
    /**
     * This test ensures that we can successfully write and read a string from a packet.
     */

    Packet test_packet{};

    const std::string write_value = "this is a test packet";
    test_packet.Write(write_value);

    std::string read_value;
    test_packet.Read(read_value);

    CLOVE_STRING_EQ(write_value.c_str(), read_value.c_str());
}

// Test 10
CLOVE_TEST(TestWriteReadStringPacketThenNonString)
{
    /**
     * This test ensures that we can successfully write a string and a non-string to a packet
     * then read both values correctly.
     */

    Packet test_packet{};

    const std::string write_value1 = "this is a test packet";
    constexpr int write_value2 = 2;

    test_packet.Write(write_value1);
    test_packet.Write(write_value2);

    std::string read_value1;
    test_packet.Read(read_value1);

    int read_value2;
    test_packet.Read(read_value2);

    CLOVE_STRING_EQ(write_value1.c_str(), read_value1.c_str());
    CLOVE_INT_EQ(write_value2, read_value2);
}

// Test 11
CLOVE_TEST(TestWriteReadNonStringThenString)
{
    /**
     * This test ensures that we can successfully write a non-string and a string to a packet
     * then read both values correctly.
     */

    Packet test_packet{};

    constexpr int write_value1 = 4;
    const std::string write_value2 = "this is a test packet";

    test_packet.Write(write_value1);
    test_packet.Write(write_value2);

    int read_value1;
    test_packet.Read(read_value1);

    std::string read_value2;
    test_packet.Read(read_value2);

    CLOVE_INT_EQ(write_value1, read_value1);
    CLOVE_STRING_EQ(write_value2.c_str(), read_value2.c_str());
}

// Test 12
CLOVE_TEST(TestWriteReadTwoStrings)
{
    /**
     * This test ensures that we can successfully write two strings to a packet and
     * read both values correctly.
     */

    Packet test_packet{};

    const std::string write_value1 = "this is the first test string";
    const std::string write_value2 = "this is the second test string";

    test_packet.Write(write_value1);
    test_packet.Write(write_value2);

    std::string read_value1;
    test_packet.Read(read_value1);

    std::string read_value2;
    test_packet.Read(read_value2);

    CLOVE_STRING_EQ(write_value1.c_str(), read_value1.c_str());
    CLOVE_STRING_EQ(write_value2.c_str(), read_value2.c_str());
}
