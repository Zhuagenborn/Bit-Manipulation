#include "bit_manip.h"

#include <gtest/gtest.h>

using namespace bit;

TEST(BitManip, GetBits) {
    constexpr std::uint32_t val {0x12345678};

    // Get 0 bit.
    EXPECT_EQ(GetBits(val, 0, 0), 0);

    // Get 4 bits.
    {
        constexpr std::size_t half_byte_len {byte_len / 2};

        EXPECT_EQ(GetBits(val, 0 * half_byte_len, half_byte_len), 0x08);
        EXPECT_EQ(GetBits(val, 1 * half_byte_len, half_byte_len), 0x07);
        EXPECT_EQ(GetBits(val, 2 * half_byte_len, half_byte_len), 0x06);
        EXPECT_EQ(GetBits(val, 3 * half_byte_len, half_byte_len), 0x05);
        EXPECT_EQ(GetBits(val, 4 * half_byte_len, half_byte_len), 0x04);
        EXPECT_EQ(GetBits(val, 5 * half_byte_len, half_byte_len), 0x03);
        EXPECT_EQ(GetBits(val, 6 * half_byte_len, half_byte_len), 0x02);
        EXPECT_EQ(GetBits(val, 7 * half_byte_len, half_byte_len), 0x01);
    }

    // Get 8 bits.
    {
        constexpr std::size_t count {sizeof(std::uint8_t) * byte_len};

        EXPECT_EQ(GetBits(val, 0 * byte_len, byte_len), 0x78);
        EXPECT_EQ(GetBits(val, 1 * byte_len, byte_len), 0x56);
        EXPECT_EQ(GetBits(val, 2 * byte_len, byte_len), 0x34);
        EXPECT_EQ(GetBits(val, 3 * byte_len, byte_len), 0x12);
    }

    // Get 16 bits.
    {
        constexpr std::size_t word_len {sizeof(std::uint16_t) * byte_len};

        EXPECT_EQ(GetBits(val, 0, word_len), 0x5678);
        EXPECT_EQ(GetBits(val, byte_len, word_len), 0x3456);
        EXPECT_EQ(GetBits(val, word_len, word_len), 0x1234);
        EXPECT_EQ(GetBits(val, word_len + byte_len, word_len), 0x0012);
    }

    // Get 24 bits.
    {
        constexpr std::size_t word_byte_len {3 * sizeof(std::uint8_t) * byte_len};

        EXPECT_EQ(GetBits(val, 0, word_byte_len), 0x345678);
        EXPECT_EQ(GetBits(val, byte_len, word_byte_len), 0x123456);
        EXPECT_EQ(GetBits(val, word_byte_len, word_byte_len), 0x000012);
    }

    // Get 32 bits.
    EXPECT_EQ(GetBits(val, 0, sizeof(val) * byte_len), val);

    // Get 64 bits from a 32-bit value.
    EXPECT_EQ(GetBits(val, 0, sizeof(std::uint64_t) * byte_len), val);
}

TEST(BitManip, FillBits) {
    // Fill 0 bit.
    {
        std::uint32_t val {0x12345678};
        FillBits(val, 0, 0);
        EXPECT_EQ(val, val);
    }

    // Fill 4 bits.
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t half_byte_len {byte_len / 2};

        FillBits(val, 0 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x1234567F);

        FillBits(val, 1 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x123456FF);

        FillBits(val, 2 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12345FFF);

        FillBits(val, 3 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x1234FFFF);

        FillBits(val, 4 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x123FFFFF);

        FillBits(val, 5 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12FFFFFF);

        FillBits(val, 6 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x1FFFFFFF);

        FillBits(val, 7 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    // Fill 8 bits.
    {
        std::uint32_t val {0x12345678};

        FillBits(val, 0 * byte_len, byte_len);
        EXPECT_EQ(val, 0x123456FF);

        FillBits(val, 1 * byte_len, byte_len);
        EXPECT_EQ(val, 0x1234FFFF);

        FillBits(val, 2 * byte_len, byte_len);
        EXPECT_EQ(val, 0x12FFFFFF);

        FillBits(val, 3 * byte_len, byte_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    // Fill 16 bits.
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t word_len {sizeof(std::uint16_t) * byte_len};

        FillBits(val, 0 * word_len, word_len);
        EXPECT_EQ(val, 0x1234FFFF);

        FillBits(val, 1 * word_len, word_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    // Fill 32 bits.
    {
        std::uint32_t val {0x12345678};
        FillBits(val, 0, sizeof(val) * byte_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }
}

TEST(BitManip, ClearBits) {
    // Clear 0 bit.
    {
        std::uint32_t val {0x12345678};
        ClearBits(val, 0, 0);
        EXPECT_EQ(val, val);
    }

    // Clear 4 bits.
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t half_byte_len {byte_len / 2};

        ClearBits(val, 0 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12345670);

        ClearBits(val, 1 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12345600);

        ClearBits(val, 2 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12345000);

        ClearBits(val, 3 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12340000);

        ClearBits(val, 4 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12300000);

        ClearBits(val, 5 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12000000);

        ClearBits(val, 6 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x10000000);

        ClearBits(val, 7 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x00000000);
    }

    // Clear 8 bits.
    {
        std::uint32_t val {0x12345678};

        ClearBits(val, 0 * byte_len, byte_len);
        EXPECT_EQ(val, 0x12345600);

        ClearBits(val, 1 * byte_len, byte_len);
        EXPECT_EQ(val, 0x12340000);

        ClearBits(val, 2 * byte_len, byte_len);
        EXPECT_EQ(val, 0x12000000);

        ClearBits(val, 3 * byte_len, byte_len);
        EXPECT_EQ(val, 0x00000000);
    }

    // Clear 16 bits.
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t word_len {sizeof(std::uint16_t) * byte_len};

        ClearBits(val, 0 * word_len, word_len);
        EXPECT_EQ(val, 0x12340000);

        ClearBits(val, 1 * word_len, word_len);
        EXPECT_EQ(val, 0x00000000);
    }

    // Clear 32 bits.
    {
        std::uint32_t val {0x12345678};
        ClearBits(val, 0, sizeof(val) * byte_len);
        EXPECT_EQ(val, 0x00000000);
    }
}

TEST(BitManip, SetBits) {
    // Set 0 bit to `0`.
    {
        std::uint32_t val {0x12345678};
        SetBits(val, 0, 0, 0);
        EXPECT_EQ(val, val);
    }

    // Set 4 bits to `0xF`.
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t half_byte_len {byte_len / 2};

        SetBits(val, 0xF, 0 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x1234567F);

        SetBits(val, 0xF, 1 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x123456FF);

        SetBits(val, 0xF, 2 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12345FFF);

        SetBits(val, 0xF, 3 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x1234FFFF);

        SetBits(val, 0xF, 4 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x123FFFFF);

        SetBits(val, 0xF, 5 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x12FFFFFF);

        SetBits(val, 0xF, 6 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0x1FFFFFFF);

        SetBits(val, 0xF, 7 * half_byte_len, half_byte_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    // Set 8 bits to `0xFF`.
    {
        std::uint32_t val {0x12345678};

        SetBits(val, 0xFF, 0 * byte_len, byte_len);
        EXPECT_EQ(val, 0x123456FF);

        SetBits(val, 0xFF, 1 * byte_len, byte_len);
        EXPECT_EQ(val, 0x1234FFFF);

        SetBits(val, 0xFF, 2 * byte_len, byte_len);
        EXPECT_EQ(val, 0x12FFFFFF);

        SetBits(val, 0xFF, 3 * byte_len, byte_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    // Set 16 bits to `0xFFFF`.
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t word_len {sizeof(std::uint16_t) * byte_len};

        SetBits(val, 0xFFFF, 0 * word_len, word_len);
        EXPECT_EQ(val, 0x1234FFFF);

        SetBits(val, 0xFFFF, 1 * word_len, word_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    // Set 32 bits to `0xFFFFFFFF`.
    {
        std::uint32_t val {0x12345678};
        SetBits(val, 0xFFFFFFFF, 0, sizeof(val) * byte_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }
}

TEST(BitManip, IsBitSet) {
    constexpr std::uint8_t val {0b1001'0001};

    EXPECT_TRUE(IsBitSet(val, 0));
    EXPECT_FALSE(IsBitSet(val, 1));
    EXPECT_TRUE(IsBitSet(val, 4));
    EXPECT_FALSE(IsBitSet(val, 6));
    EXPECT_TRUE(IsBitSet(val, 7));
}

TEST(BitManip, SetBit) {
    std::uint8_t val {0};

    SetBit(val, 0);
    EXPECT_EQ(val, 0b0000'0001);

    SetBit(val, 1);
    EXPECT_EQ(val, 0b0000'0011);

    SetBit(val, 4);
    EXPECT_EQ(val, 0b0001'0011);

    SetBit(val, 7);
    EXPECT_EQ(val, 0b1001'0011);
}

TEST(BitManip, ClearBit) {
    std::uint8_t val {0b1111'1111};

    ClearBit(val, 0);
    EXPECT_EQ(val, 0b1111'1110);

    ClearBit(val, 1);
    EXPECT_EQ(val, 0b1111'1100);

    ClearBit(val, 4);
    EXPECT_EQ(val, 0b1110'1100);

    ClearBit(val, 7);
    EXPECT_EQ(val, 0b0110'1100);
}

TEST(BitManip, GetByte) {
    {
        constexpr std::uint32_t val {0x12345678};

        EXPECT_EQ(GetByte(val, 0 * byte_len), 0x78);
        EXPECT_EQ(GetByte(val, 1 * byte_len), 0x56);
        EXPECT_EQ(GetByte(val, 2 * byte_len), 0x34);
        EXPECT_EQ(GetByte(val, 3 * byte_len), 0x12);
    }

    {
        constexpr std::uint16_t val {0x1234};

        EXPECT_EQ(GetLowByte(val), 0x34);
        EXPECT_EQ(GetHighByte(val), 0x12);
    }
}

TEST(BitManip, FillByte) {
    {
        std::uint32_t val {0x12345678};

        FillByte(val, 0 * byte_len);
        EXPECT_EQ(val, 0x123456FF);

        FillByte(val, 1 * byte_len);
        EXPECT_EQ(val, 0x1234FFFF);

        FillByte(val, 2 * byte_len);
        EXPECT_EQ(val, 0x12FFFFFF);

        FillByte(val, 3 * byte_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    {
        std::uint16_t val {0x1234};

        FillLowByte(val);
        EXPECT_EQ(val, 0x12FF);

        FillHighByte(val);
        EXPECT_EQ(val, 0xFFFF);
    }
}

TEST(BitManip, ClearByte) {
    {
        std::uint32_t val {0x12345678};

        ClearByte(val, 0 * byte_len);
        EXPECT_EQ(val, 0x12345600);

        ClearByte(val, 1 * byte_len);
        EXPECT_EQ(val, 0x12340000);

        ClearByte(val, 2 * byte_len);
        EXPECT_EQ(val, 0x12000000);

        ClearByte(val, 3 * byte_len);
        EXPECT_EQ(val, 0x00000000);
    }

    {
        std::uint16_t val {0x1234};

        ClearLowByte(val);
        EXPECT_EQ(val, 0x1200);

        ClearHighByte(val);
        EXPECT_EQ(val, 0x0000);
    }
}

TEST(BitManip, SetByte) {
    {
        std::uint32_t val {0x12345678};

        SetByte(val, 0xFF, 0 * byte_len);
        EXPECT_EQ(val, 0x123456FF);

        SetByte(val, 0xFF, 1 * byte_len);
        EXPECT_EQ(val, 0x1234FFFF);

        SetByte(val, 0xFF, 2 * byte_len);
        EXPECT_EQ(val, 0x12FFFFFF);

        SetByte(val, 0xFF, 3 * byte_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    {
        std::uint16_t val {0x1234};

        SetLowByte(val, 0xFF);
        EXPECT_EQ(val, 0x12FF);

        SetHighByte(val, 0xFF);
        EXPECT_EQ(val, 0xFFFF);
    }
}

TEST(BitManip, GetWord) {
    constexpr std::uint32_t val {0x12345678};

    {
        constexpr std::size_t word_len {sizeof(std::uint16_t) * byte_len};

        EXPECT_EQ(GetWord(val, 0 * word_len), 0x5678);
        EXPECT_EQ(GetWord(val, 1 * word_len), 0x1234);
    }

    {
        EXPECT_EQ(GetLowWord(val), 0x5678);
        EXPECT_EQ(GetHighWord(val), 0x1234);
    }
}

TEST(BitManip, FillWord) {
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t word_len {sizeof(std::uint16_t) * byte_len};

        FillWord(val, 0 * word_len);
        EXPECT_EQ(val, 0x1234FFFF);

        FillWord(val, 1 * word_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    {
        std::uint32_t val {0x12345678};

        FillLowWord(val);
        EXPECT_EQ(val, 0x1234FFFF);

        FillHighWord(val);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }
}

TEST(BitManip, ClearWord) {
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t word_len {sizeof(std::uint16_t) * byte_len};

        ClearWord(val, 0 * word_len);
        EXPECT_EQ(val, 0x12340000);

        ClearWord(val, 1 * word_len);
        EXPECT_EQ(val, 0x00000000);
    }

    {
        std::uint32_t val {0x12345678};

        ClearLowWord(val);
        EXPECT_EQ(val, 0x12340000);

        ClearHighWord(val);
        EXPECT_EQ(val, 0x00000000);
    }
}

TEST(BitManip, SetWord) {
    {
        std::uint32_t val {0x12345678};
        constexpr std::size_t word_len {sizeof(std::uint16_t) * byte_len};

        SetWord(val, 0xFFFF, 0 * word_len);
        EXPECT_EQ(val, 0x1234FFFF);

        SetWord(val, 0xFFFF, 1 * word_len);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }

    {
        std::uint32_t val {0x12345678};

        SetLowWord(val, 0xFFFF);
        EXPECT_EQ(val, 0x1234FFFF);

        SetHighWord(val, 0xFFFF);
        EXPECT_EQ(val, 0xFFFFFFFF);
    }
}

TEST(BitManip, GetDword) {
    constexpr std::uint64_t val {0x0123456789ABCDEF};

    {
        constexpr std::size_t dword_len {sizeof(std::uint32_t) * byte_len};

        EXPECT_EQ(GetDword(val, 0 * dword_len), 0x89ABCDEF);
        EXPECT_EQ(GetDword(val, 1 * dword_len), 0x01234567);
    }

    {
        EXPECT_EQ(GetLowDword(val), 0x89ABCDEF);
        EXPECT_EQ(GetHighDword(val), 0x01234567);
    }
}

TEST(BitManip, FillDword) {
    {
        std::uint64_t val {0x0123456789ABCDEF};
        constexpr std::size_t dword_len {sizeof(std::uint32_t) * byte_len};

        FillDword(val, 0 * dword_len);
        EXPECT_EQ(val, 0x01234567FFFFFFFF);

        FillDword(val, 1 * dword_len);
        EXPECT_EQ(val, 0xFFFFFFFFFFFFFFFF);
    }

    {
        std::uint64_t val {0x0123456789ABCDEF};

        FillLowDword(val);
        EXPECT_EQ(val, 0x01234567FFFFFFFF);

        FillHighDword(val);
        EXPECT_EQ(val, 0xFFFFFFFFFFFFFFFF);
    }
}

TEST(BitManip, ClearDword) {
    {
        std::uint64_t val {0x0123456789ABCDEF};
        constexpr std::size_t dword_len {sizeof(std::uint32_t) * byte_len};

        ClearDword(val, 0 * dword_len);
        EXPECT_EQ(val, 0x0123456700000000);

        ClearDword(val, 1 * dword_len);
        EXPECT_EQ(val, 0x0000000000000000);
    }

    {
        std::uint64_t val {0x0123456789ABCDEF};

        ClearLowDword(val);
        EXPECT_EQ(val, 0x0123456700000000);

        ClearHighDword(val);
        EXPECT_EQ(val, 0x0000000000000000);
    }
}

TEST(BitManip, SetDword) {
    {
        std::uint64_t val {0x0123456789ABCDEF};
        constexpr std::size_t dword_len {sizeof(std::uint32_t) * byte_len};

        SetDword(val, 0xFFFFFFFF, 0 * dword_len);
        EXPECT_EQ(val, 0x01234567FFFFFFFF);

        SetDword(val, 0xFFFFFFFF, 1 * dword_len);
        EXPECT_EQ(val, 0xFFFFFFFFFFFFFFFF);
    }

    {
        std::uint64_t val {0x0123456789ABCDEF};

        SetLowDword(val, 0xFFFFFFFF);
        EXPECT_EQ(val, 0x01234567FFFFFFFF);

        SetHighDword(val, 0xFFFFFFFF);
        EXPECT_EQ(val, 0xFFFFFFFFFFFFFFFF);
    }
}

TEST(BitManip, CombineBits) {
    EXPECT_EQ((CombineBits<std::uint16_t, std::uint8_t>(0x12, 0x34)), 0x1234);
    EXPECT_EQ((CombineBits<std::uint32_t, std::uint16_t>(0x1234, 0x5678)), 0x12345678);
    EXPECT_EQ((CombineBits<std::uint64_t, std::uint32_t>(0x01234567, 0x89ABCDEF)),
              0x0123456789ABCDEF);
}

TEST(BitManip, CombineBytes) {
    EXPECT_EQ(CombineBytes(0x12, 0x34), 0x1234);
}

TEST(BitManip, CombineWords) {
    EXPECT_EQ(CombineWords(0x1234, 0x5678), 0x12345678);
}

TEST(BitManip, CombineDwords) {
    EXPECT_EQ(CombineDwords(0x01234567, 0x89ABCDEF), 0x0123456789ABCDEF);
}